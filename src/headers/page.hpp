#ifndef __PAGE_HPP__
#define __PAGE_HPP__
 
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "choice.hpp"
using std::map;
using std::string;
using std::vector;

enum Pagetype { N, W, L };
Pagetype strToPagetype(string str);
string pagetypeToStr(Pagetype type);

class Page {
 private:
  size_t page_num;
  Pagetype type;
  string filename;
  vector<Choice *> choices;
  map<string, long> page_vars;
 public:
  Page();
  Page(size_t page_num, Pagetype type, string filename);
  Page(const Page & rhs);
  ~Page();
  Page & operator=(const Page & rhs);
  void printPage(bool isGamePlay, map<string, long> * story_map) const;
  //Accessors
  size_t getPageNumber() const;
  Choice * getChoice(size_t indx) const;
  bool isWinPage() const;
  bool isLosePage() const;
  const vector<Choice *> & getChoices() const;

  void addChoice(Choice * choice);
  void addVar(string key, long value);
};
#endif
