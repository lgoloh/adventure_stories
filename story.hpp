#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "page.hpp"
//#include "path.hpp"                                                                                 
using std::map;

class PathPoint {
   size_t source;
   size_t choice_num;
   size_t dest;
 public:
  PathPoint() : source(0), choice_num(0), dest(0) {}

  PathPoint(size_t pg, size_t ch, size_t d) : source(pg), choice_num(ch), dest(d) {}

  PathPoint(const PathPoint & rhs) :
      source(rhs.source), choice_num(rhs.choice_num), dest(rhs.dest) {}

  void print() { std::cout << source << "(" << choice_num << ")"; }

  size_t getDest() const { return dest; }

  size_t getSource() const { return source; }
};

class Path {
  vector<PathPoint *> path_list;
  vector<size_t> pages;
 
  bool inPath(size_t pg) {
    vector<size_t>::iterator pages_it;
    pages_it = std::find(pages.begin(), pages.end(), pg);
    if (pages_it == pages.end()) {
      return false;
    }
    return true;
  }
 
 public:
  Path() : path_list(vector<PathPoint *>()), pages(vector<size_t>()) {}

  Path(vector<PathPoint *> points) : path_list(points) {
    for (size_t i = 0; i < path_list.size(); i++) {
      pages.push_back(path_list[i]->getSource());
    }
  }

  Path(const Path & rhs) {
    vector<PathPoint *> temp;
    for (size_t i = 0; i < rhs.path_list.size(); i++) {
      temp.push_back(new PathPoint(*(rhs.path_list[i])));
    }
    path_list = temp;
    pages = rhs.pages;
  }

  Path & operator=(const Path & rhs) {
    if (this != &rhs) {
      vector<PathPoint *> temp;
      for (size_t i = 0; i < rhs.path_list.size(); i++) {
	temp.push_back(new PathPoint(*(rhs.path_list[i])));
      }
      path_list = temp;
      pages = rhs.pages;
    }
    return *this;
  }

  ~Path() {
    for (size_t i = 0; i < path_list.size(); i++) {
      delete path_list[i];
    }
  }

  bool add(PathPoint * pg) {
    if (!inPath(pg->getDest())) {
      path_list.push_back(pg);
      pages.push_back(pg->getSource());
      return true;
    }
    return false;
  }

  size_t getSize() const { return path_list.size(); }

  PathPoint * getLastPoint() const { return path_list.back(); }

  vector<PathPoint *> getPoints() const { return path_list; }

  vector<size_t> getRefs() const { return pages; }

  void print() const {
    size_t i = 0;
    for (; i < path_list.size(); i++) {
      path_list[i]->print();
      std::cout << ",";
    }
    std::cout << path_list[i - 1]->getDest() << "(win)" << std::endl;
  }
};

class Story {
 private:
  vector<Page *> pages;
  map<size_t, vector<size_t> > page_refs;
  bool hasWinPage;
  bool hasLosePage;
  map<string, long> var_map;

 public:
  Story();
  Story(const Story & rhs);
  ~Story();
  Story & operator=(const Story & rhs);
  Page * getPage(size_t pg_num) const;

  void printStory() const;
  void addPage(Page * pg);
  void addVar(string key, long val);
  vector<Page *> getPages() const;
  map<size_t, vector<size_t> > & getPageRefs();

  void updateRefsForPage(size_t pg_num, size_t ref_num);
  bool isValidPage(size_t num);
  void validateStory();
  void playStory();
  vector<Path *> getCycleFreeWinningPaths();
};
#endif
