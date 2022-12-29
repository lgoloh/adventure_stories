#ifndef __CYOA_HPP__
#define __CYOA_HPP__

#include <cstdlib>
#include <istream>

#include "choice.hpp"
#include "page.hpp"
#include "story.hpp"
using std::string;

//Utility class    
template<typename T, typename P, typename Q>
class tuple {
 private:
  T val1;
  P val2;
  Q val3;

 public:
 tuple() : val1(NULL), val2(NULL), val3(NULL) {}
 tuple(T v1, P v2, Q v3) : val1(v1), val2(v2), val3(v3) {}

 T first() { return val1; }

 P second() { return val2; }

 Q third() { return val3; }

 void print() {
   std::cout << "val1: " << val1 << " val2: " << val2 << " val3: " << val3 << std::endl;
 }
};

//Takes the directory of story files and returns a story object   
Story * createStory(string & storyDir);
#endif
