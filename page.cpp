#include "page.hpp"

#include <fstream>
#include <sstream>
/*
 * Prints the page dividers: ==========
 */
void printPgDiv() {
   std::stringstream buff;
   for (int i = 0; i < 10; i++) {
     std::cout << "=";
   }
   std::cout << '\n';
}
/*
 * Prints choices
 * Marks a conditional choice as unavailable if value is not set
 */
void printChoices(const vector<Choice *> & list, const map<string, long> * vars) {
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << " " << i + 1 << ". ";
	if (list[i]->isConditional()) {
	  long val = vars->at(list[i]->getVarName());
	  if (val != list[i]->getValue()) {
	    list[i]->setAvailability(false);
	    std::cout << "<UNAVAILABLE>" << '\n';
	  }
	  else {
	    std::cout << list[i]->getText() << '\n';
	  }
	}
	else {
	  std::cout << list[i]->getText() << '\n';
	}
    }
}

bool printWinOrLose(const Page * pg) {
  if (pg->isWinPage()) {
    std::cout << "Congratulations! You have won. Hooray!\n";
    return true;
  }
  else if (pg->isLosePage()) {
    std::cout << "Sorry, you have lost. Better luck next time!\n";
    return true;
  }
  return false;
}

Pagetype strToPagetype(string str) {
  if (str == "N") {
    return N;
  }
  else if (str == "W") {
    return W;
  }
  else if (str == "L") {
    return L;
  }

  std::stringstream errorMsg;
  errorMsg << "Invalid page type: " << str << '\n';
  throw std::invalid_argument(errorMsg.str());
}

string pagetypeToStr(Pagetype type) {
  if (type == N) {
     return "N";
  }
  else if (type == W) {
    return "W";
  }
  else if (type == L) {
    return "L";
  }
  std::stringstream errorMsg;
  errorMsg << "Invalid page type to string conversion\n";
  throw std::invalid_argument(errorMsg.str());
}

//Default type of page is N
Page::Page() :
    page_num(0),
    type(N),
    filename(""),
    choices(vector<Choice *>()),
    page_vars(std::map<string, long>()) {
}

Page::Page(size_t p_num, Pagetype t, string file) :
    page_num(p_num),
    type(t),
    filename(file),
    choices(vector<Choice *>()),
    page_vars(std::map<string, long>()) {
}

Page::Page(const Page & rhs) :
    page_num(rhs.page_num),
    type(rhs.type),
    filename(rhs.filename),
    choices(rhs.choices),
    page_vars(rhs.page_vars) {
}

Page & Page::operator=(const Page & rhs) {
  if (this != &rhs) {
    vector<Choice *> temp;
    temp.resize(rhs.choices.size());
    for (size_t i = 0; i < rhs.choices.size(); i++) {
      temp[i] = new Choice(*(rhs.choices[i]));
    }
    choices = temp;
    page_num = rhs.page_num;
    page_vars = rhs.page_vars;
    type = rhs.type;
    filename = rhs.filename;
  }
  return *this;
}

Page::~Page() {
  for (size_t i = 0; i < choices.size(); i++) {
    delete choices[i];
  }
}

void Page::addChoice(Choice * choice) {
  choices.push_back(choice);
}

void Page::addVar(string key, long value) {
  page_vars[key] = value;
}

size_t Page::getPageNumber() const {
  return page_num;
}

Choice * Page::getChoice(size_t indx) const {
  assert(indx >= 0 && indx < choices.size());
  return choices[indx];
}

bool Page::isWinPage() const {
  return type == W;
}

bool Page::isLosePage() const {
  return type == L;
}

void updateStoryVars(map<string, long> & story_vars, const map<string, long> & page_vars) {
  map<string, long>::const_iterator pg_it = page_vars.begin();
  while (pg_it != page_vars.end()) {
    story_vars[pg_it->first] = pg_it->second;
    ++pg_it;
  }
}

void Page::printPage(bool isGamePlay, map<string, long> * story_vars) const {
  std::ifstream page;
  page.open(filename.c_str(), std::ifstream::in);
  if (!page.is_open()) {
    std::cerr << "Error opening page\n";
    exit(EXIT_FAILURE);
  }
  updateStoryVars(*story_vars, page_vars);
  string cur_line;
  if (!isGamePlay) {
    std::cout << "Page " << page_num << std::endl;
    printPgDiv();
  }

  while (std::getline(page, cur_line)) {
    std::cout << cur_line << '\n';
  }
  page.close();
  std::cout << '\n';
  if (!printWinOrLose(this)) {
    std::cout << "What would you like to do?\n";
    std::cout << '\n';
    map<string, long> * v_map = isGamePlay ? story_vars : NULL;
    printChoices(choices, v_map);
  }
}

const vector<Choice *> & Page::getChoices() const {
  return choices;
}

