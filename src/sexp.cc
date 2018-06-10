#include "./sexp.h"

#include <ctype.h>
#include <cassert>
#include <sstream>

Sexp::Sexp(const std::string &input) {
  bool all_digits = true;
  size_t word_size = 0;
  std::ostringstream word;
  size_t skip = 0;
  int stack_size = 0;

  push_sexp();

  for (size_t i = 0; i < input.size(); i++) {
    char c = input[i];
    if (skip) {
      skip--;
      continue;
    }
    if (c == '(') {
      stack_size++;
      if (word_size) {
        push_string(word.str());
        word.str("");
        word_size = 0;
      }
      all_digits = true;
      push_sexp();
    } else if (c == ')') {
      if (stack_size == 0) {
        break;
      }
      if (word_size) {
        push_string(word.str());
        word.str("");
        word_size = 0;
      }
      all_digits = true;
      Sexp temp = std::get<Sexp>(pop_back());
      push_back_sexp(temp);
      stack_size--;
    } else if (isspace(c)) {
      if (word_size) {
        push_string(word.str());
        word.str("");
        word_size = 0;
      }
      all_digits = true;
    } else {
      if (c == ':' && all_digits) {
        long lskip = std::stol(word.str());
        assert(lskip > 0);
        skip = lskip;
        std::string s = input.substr(i + 1, skip);
        push_string(s);
        word.str("");
        word_size = 0;
        continue;
      }
      if (all_digits && !isdigit(c)) {
        all_digits = false;
      }
      word << c;
      word_size++;
    }
  }
  assert(stack_size == 0);
  assert(word_size == 0);
  Sexp child = std::get<Sexp>(children_[0]);
  *this = std::get<Sexp>(child.children_[0]);
}

std::ostream &operator<<(std::ostream &os, const Sexp &sexp) {
  if (sexp.empty()) {
    os << "()";
    return os;
  }
  const std::vector<Node> &children = sexp.children();
  os << "(";
  for (size_t i = 0; i < sexp.size() - 1; i++) {
    std::visit([&os](auto &&arg) { os << arg; }, children[i]);
    os << " ";
  }
  std::visit([&os](auto &&arg) { os << arg; }, children.back());
  os << ")";
  return os;
}
