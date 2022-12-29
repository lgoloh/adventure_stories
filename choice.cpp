#include "choice.hpp"
#include <sstream>

Choice::Choice() : source(0), destination(0), text(""), available(true) {
}
Choice::Choice(size_t src, size_t dest, string text) :
    source(src), destination(dest), text(text), available(true) {
}
Choice::Choice(const Choice & rhs) :
    source(rhs.source), destination(rhs.destination), text(rhs.text) {
}

Choice::~Choice() {}

Choice & Choice::operator=(const Choice & rhs) {
  if (this != &rhs) {
    string temp = rhs.text;
    std::swap(temp, text);
    source = rhs.source;
    destination = rhs.destination;
  }
  return *this;
}
size_t Choice::getSource() const {
  return source;
}

size_t Choice::getDest() const {
  return destination;
}

string Choice::getText() const {
  return text;
}

bool Choice::isAvailable() const {
  return available;
}

void Choice::setSource(size_t src) {
  source = src;
}

void Choice::setDestination(size_t dest) {
  destination = dest;
}

void Choice::setText(string choiceText) {
  text = choiceText;
}

void Choice::setAvailability(bool is_avail) {
  available = is_avail;
}

void Choice::printChoice() {
  std::cout << " " << source << ". ";
  std::cout << text << '\n';
}

bool Choice::isConditional() const {
  return false;
}

string Choice::getVarName() const {
  return "";
}

long Choice::getValue() const {
  return 0;
}

ConditionalChoice::ConditionalChoice() : var_name(""), value(0) {
}

ConditionalChoice ::ConditionalChoice(size_t src,
                                      size_t dest,
                                      string text,
                                      string var,
                                      long val) :
    Choice(src, dest, text), var_name(var), value(val) {
}

ConditionalChoice::~ConditionalChoice() {
}

