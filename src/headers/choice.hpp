#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <cstdlib>
#include <iostream>
#include <string>
using std::string;

class Choice {
  private:
    size_t source;
    size_t destination;
    string text;
    bool available;

  public:
    Choice();
    Choice(size_t source, size_t dest, string text);
    Choice(const Choice & rhs);
    virtual ~Choice();
    Choice & operator=(const Choice & rhs);
    //Accessors                                                                                                                                    
    size_t getSource() const;
    size_t getDest() const;
    string getText() const;
    bool isAvailable() const;
    //Settors                                                                                                                                      
    void setSource(size_t source);
    void setDestination(size_t des);
    void setText(string text);
    void setAvailability(bool is_avail);
    void printChoice();
    virtual bool isConditional() const;
    virtual string getVarName() const;
    virtual long getValue() const;
  };

  class ConditionalChoice : public Choice {
    private:
      string var_name;
      long value;
    public:
      ConditionalChoice();
      ConditionalChoice(size_t src, size_t dest, string text, string var, long val);
      virtual ~ConditionalChoice();
      virtual string getVarName() const { return var_name; }
      virtual long getValue() const { return value; }
      virtual bool isConditional() const { return true; }
    };
 #endif
