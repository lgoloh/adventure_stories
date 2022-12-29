#include "story.hpp"
  
#include <algorithm>
#include <limits>
#include <queue>
using std::map;

Story::Story() :
  pages(vector<Page *>()),
  page_refs(map<size_t, vector<size_t> >()),
  hasWinPage(false),
  hasLosePage(false),
  var_map(map<string, long>()) {
 }

Story::Story(const Story & rhs) :
  pages(rhs.pages),
  page_refs(rhs.page_refs),
  hasWinPage(rhs.hasWinPage),
  hasLosePage(rhs.hasLosePage),
  var_map(rhs.var_map) {
 }
 
Story & Story::operator=(const Story & rhs) {
  if (this != &rhs) {
    vector<Page *> temp;
    temp.resize(rhs.pages.size());
    for (size_t i = 0; i < rhs.pages.size(); i++) {
      temp[i] = new Page(*(rhs.pages[i]));
    }
    pages = temp;
    page_refs = rhs.page_refs;
    var_map = rhs.var_map;
    hasWinPage = rhs.hasWinPage;
    hasLosePage = rhs.hasLosePage;
  }
  return *this;
}

Story::~Story() {
  for (size_t i = 0; i < pages.size(); i++) {
    delete pages[i];
  }
}

Page * Story::getPage(size_t pg_num) const {
   vector<Page *>::const_iterator it = pages.begin();
   for (; it != pages.end(); ++it) {
     if ((*it)->getPageNumber() == pg_num) {
       return *it;
     }
   }
   return NULL;
}

map<size_t, vector<size_t> > & Story::getPageRefs() {
  return page_refs;
}

void Story::printStory() const {
  vector<Page *>::const_iterator it = pages.begin();
  while (it != pages.end()) {
    (*it)->printPage(false, NULL);
    ++it;
  }
}

void Story::addPage(Page * pg) {
  Page * p = getPage(pg->getPageNumber());
  if (p == NULL) {
    pages.push_back(pg);
    if (pg->isLosePage()) {
      hasLosePage = true;
    }
    else if (pg->isWinPage()) {
      hasWinPage = true;
    }
    vector<size_t> refs;
    page_refs.insert(std::pair<size_t, vector<size_t> >(pg->getPageNumber(), refs));
  }
}

void Story::addVar(string key, long val) {
  var_map[key] = val;
}

vector<Page *> Story::getPages() const {
  return pages;
}

void Story::updateRefsForPage(size_t pg_num, size_t ref_num) {
  assert(page_refs.find(pg_num) != page_refs.end());
  page_refs[pg_num].push_back(ref_num);
}


bool Story::isValidPage(size_t num) {
  return getPage(num) != NULL;
}

/*
 *Checks if a page is a choice for at least one other page
 *Iterates through the vector of references for each page to find the given page number
 *input: page, map of page number to a vector of pages referenced by its choices
 *return: true if it finds it. false otherwise                                                        */
bool isPageReferenced(Page * pg, std::map<size_t, vector<size_t> > & refs) {
  std::map<size_t, vector<size_t> >::iterator it = refs.begin();

  for (; it != refs.end(); ++it) {
    vector<size_t> ch_list = it->second;
    std::vector<size_t>::iterator ch_it;
    ch_it = std::find(ch_list.begin(), ch_list.end(), pg->getPageNumber());
    if (ch_it != ch_list.end()) {
      return true;
    }
  }
  return false;
}

/*
 * Checks if all page references in a given page are valid
 */
bool isPageRefValid(Page * pg, std::map<size_t, vector<size_t> > & refs) {
  vector<Choice *>::const_iterator choice_it = pg->getChoices().begin();
  std::map<size_t, vector<size_t> >::iterator refs_it;
  
  while (choice_it != pg->getChoices().end()) {
    refs_it = refs.find((*choice_it)->getDest());
    if (refs_it == refs.end()) {
      return false;
    }
    ++choice_it;
  }
  return true;
}


void Story::validateStory() {
  vector<Page *>::const_iterator it = pages.begin();

  if (hasLosePage == false || hasWinPage == false) {
    std::cerr << "Story has no win or lose page\n";
    exit(EXIT_FAILURE);
  }

  while (it != pages.end()) {
    if (!isPageRefValid(*it, page_refs)) {
      std::cerr << "Invalid page reference\n";
      exit(EXIT_FAILURE);
    }
    if ((*it)->getPageNumber() != 0 && !isPageReferenced(*it, page_refs)) {
      std::cerr << "Page is not referenced by any other page\n";
      exit(EXIT_FAILURE);
    }
    ++it;
  }
}


size_t readChoice(vector<size_t> & refs, const Page * page) {
  size_t input;
  while (true) {
    std::cin >> input;
    std::cin.ignore();
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "That is not a valid choice, please try again\n";
    }
    else {
      if (input <= 0 || input > refs.size()) {
        std::cout << "That is not a valid choice, please try again\n";
      }
      else if (!page->getChoice(input - 1)->isAvailable()) {
        std::cout << "That choice is not available at this time, please try again\n";
      }
      else {
        break;
      }
    }
  }
  return refs[input - 1];
}

//For step4, pass in var_map reference for pages to update or access to print conditional choices
void Story::playStory() {
  Page * cur_page = pages.front();
  while (!cur_page->isWinPage() && !cur_page->isLosePage()) {
    cur_page->printPage(true, &var_map);
    vector<size_t> curr_refs = page_refs[cur_page->getPageNumber()];
    size_t choice = readChoice(curr_refs, cur_page);
    cur_page = getPage(choice);
  }
  cur_page->printPage(true, &var_map);
}

/*
 * Uses BFS to find all cycle-free winning paths
 * Creates a new path object for each path in the graph that internally tracks visited pages
 * Discards a path with a cycle or lose page
 */
vector<Path *> Story::getCycleFreeWinningPaths() {
  vector<Path *> paths;
  std::queue<Path *> path_queue;
  map<size_t, vector<size_t> >::iterator refs_it = page_refs.begin();

  for (size_t i = 0; i < refs_it->second.size(); i++) {
    Path * n_path = new Path();
    n_path->add(new PathPoint(refs_it->first, i + 1, refs_it->second[i]));
    path_queue.push(n_path);
  }

  while (!path_queue.empty()) {
    Path * cur_path = path_queue.front();
    path_queue.pop();
    PathPoint * last_point = cur_path->getLastPoint();

    if (getPage(last_point->getDest())->isWinPage()) {
      Path * w_path = new Path(*cur_path);
      paths.push_back(w_path);
    }
    else {
      vector<size_t> refs = page_refs.at(last_point->getDest());
      for (size_t i = 0; i < refs.size(); i++) {
        PathPoint * pt = new PathPoint(last_point->getDest(), i + 1, refs[i]);
        Path * n_path = new Path(*cur_path);
        if (n_path->add(pt) == true) {
          path_queue.push(n_path);
        }
      }
    }
    delete cur_path;
  }
  return paths;
}


