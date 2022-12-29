#include "cyoa.hpp"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
using std::cerr;
using std::cout;
using std::ifstream;

//Utility functions:
size_t getPageNum(string & str_num) {
  const char * str = str_num.c_str();
  size_t len_initwhitespaces = strspn(str, " \t");
  char * rem_str = strdup(str + len_initwhitespaces);

  for (size_t i = 0; i < str_num.size(); i++) {
    if (!isdigit(rem_str[i])) {
      throw std::invalid_argument("invalid page number\n");
    }
  }
  size_t pg_num = strtoull(rem_str, NULL, 10);
  free(rem_str);
  return pg_num;
}

//Splits string by a delimiter and returns a pair of the key and value  
std::pair<string, string> splitStrByDelim(const string & str, const string & delim) {
  size_t delim_pos = str.find_first_of(delim);
  if (delim_pos == string::npos) {
    throw std::invalid_argument("Invalid argument: no delimeter\n");
  }
  return make_pair(str.substr(0, delim_pos), str.substr(delim_pos + 1));
}

/*
 * Parses a page declaration if its valid
 * Returns a tuple of the page number, page type and page file name
 */
tuple<size_t, Pagetype, string> parsePageDeclaration(string & line, const string & path) {
  size_t page_num = 0;
  Pagetype page_type;
  string page_file = "";

  try {
    std::pair<string, string> init_split = splitStrByDelim(line, "@");
    page_num = getPageNum(init_split.first);
    std::pair<string, string> type_split = splitStrByDelim(init_split.second, ":");
    page_type = strToPagetype(type_split.first);
    page_file = string(path).append("/").append(type_split.second);
    if (page_file.size() == 0) {
      throw std::invalid_argument("Invalid argument: empty file name\n");
    }
  }
  catch (std::invalid_argument & error) {
    cerr << "Invalid page declaration: " << line << error.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return tuple<size_t, Pagetype, string>(page_num, page_type, page_file);
}

/*
 * Parses a valid choice declaration
 * return: tuple of source page, destination page and the choice text
 */
tuple<string, size_t, string> parseChoiceDeclaration(string line) {
   string source = "";
   size_t dest = 0;
   string text = "";
 
   try {
     std::pair<string, string> init = splitStrByDelim(line, ":");
     source = init.first;
     std::pair<string, string> nxt = splitStrByDelim(init.second, ":");
     dest = getPageNum(nxt.first);
     text = nxt.second;
     if (text.size() == 0) {
       throw std::invalid_argument("Invalid argument: empty choice text\n");
     }
   }
   catch (std::invalid_argument & error) {
     cerr << "Invalid choice declaration: " << line << std::endl;
     exit(EXIT_FAILURE);
   }
   return tuple<string, size_t, string>(source, dest, text);
 }

/*
 * Parses a varibale declaration
 * return: tuple of page number, variable name, and value
 */
tuple<size_t, string, long> parseVarDeclaration(string line) {
  size_t source = 0;
  string var_name = "";
  long val;

  try {
    std::pair<string, string> init = splitStrByDelim(line, "$");
    source = getPageNum(init.first);
    std::pair<string, string> nxt = splitStrByDelim(init.second, "=");
    var_name = nxt.first;
    val = atol(nxt.second.c_str());
  }
  catch (std::invalid_argument & error) {
    cerr << "Invalid choice declaration: " << line << std::endl;
    exit(EXIT_FAILURE);
  }

  return tuple<size_t, string, long>(source, var_name, val);
}

/*
 * Verifies that the pages are in order
 * return: true if pages are consective and first page is 0, false otherwise
 */
bool isMissingPages(Story * story, size_t page_num) {
  if (story->getPages().size() == 0) {
    return (page_num == 0) ? false : true;
  }
  else {
    size_t last_pg = story->getPages().back()->getPageNumber();
    return ((page_num - last_pg) == 1) ? false : true;
  }
}

/*
 * Parses a choice condition
 * returns: tuple of page number, variable name, value
 */
tuple<size_t, string, long> parseChoiceCond(string line) {
  size_t source = 0;
  string var_name = "";
  long val;

  try {
    std::pair<string, string> init = splitStrByDelim(line, "[");
    source = getPageNum(init.first);
    std::pair<string, string> nxt = splitStrByDelim(init.second, "=");
    var_name = nxt.first;
    std::pair<string, string> fin = splitStrByDelim(nxt.second, "]");
    val = atol(fin.first.c_str());
  }
  catch (std::invalid_argument & error) {
  cerr << "Invalid choice declaration: " << line << std::endl;
    exit(EXIT_FAILURE);
  }

  return tuple<size_t, string, long>(source, var_name, val);
}

/*
 * Creates a new page
 * return: Page pointer
 */
Page * createPage(string line, string dir, Story * story) {
  tuple<size_t, Pagetype, string> page_metadata = parsePageDeclaration(line, dir);

  if (isMissingPages(story, page_metadata.first())) {
    throw std::invalid_argument("Out of order page\n");
  }
  else {
    return new Page(page_metadata.first(), page_metadata.second(), page_metadata.third());
  }
}


/*
 * Creates a new condition
 * return: Choice pointer
 */
Choice * createChoice(string line) {
    tuple<string, size_t, string> choice_metadata = parseChoiceDeclaration(line);
    string ch_src = choice_metadata.first();
    if (ch_src.find("[") != string::npos) {
        tuple<size_t, string, long> cond = parseChoiceCond(ch_src);
    return new ConditionalChoice(cond.first(),
				 choice_metadata.second(),
				 choice_metadata.third(),
				 cond.second(),
				 cond.third());
      }
    else {
        size_t pg_num = getPageNum(ch_src);
        return new Choice(pg_num, choice_metadata.second(), choice_metadata.third());
    }
}


/*
 * Parses the story metadata in story.txt and creates a story object
 */
Story * createStory(string & storyDir) {
  string story_path = string(storyDir).append("/story.txt");
  ifstream story_mtdt;
  story_mtdt.open(story_path.c_str(), ifstream::in);
  if (!story_mtdt.is_open()) {
    std::cerr << "Error opening file\n" << story_path << std::endl;
    exit(EXIT_FAILURE);
  }

  Story * story = new Story();
  string cur_line;
  while (std::getline(story_mtdt, cur_line)) {
    if (cur_line.size() == 0) {
      continue;
    }
 
    if (cur_line.find("@") != string::npos) {
      Page * new_page = createPage(cur_line, storyDir, story);
      story->addPage(new_page);
    }
    else if (cur_line.find("$") != string::npos) {
      tuple<size_t, string, long> var = parseVarDeclaration(cur_line);
      Page * pg = story->getPage(var.first());
      if (pg == NULL) {
        throw std::invalid_argument("Invalid page for variable declaration\n");
      }
      else {
        pg->addVar(var.second(), var.third());
        story->addVar(var.second(), 0);
      }
    }
    else {
      Choice * ch = createChoice(cur_line);
      Page * pg = story->getPage(ch->getSource());
      if (pg == NULL || (pg->isWinPage() || pg->isLosePage())) {
        throw std::invalid_argument("Invalid page for choice\n");
      }
      else {
        pg->addChoice(ch);
        story->updateRefsForPage(ch->getSource(), ch->getDest());
      }
    }
  }
  story_mtdt.close();
  return story;
}
