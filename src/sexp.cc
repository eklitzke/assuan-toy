#include "./sexp.h"

#include <ctype.h>
#include <cassert>
#include <sstream>

Sexp::Sexp(const std::string &input) {
  push_sexp();

  bool all_digits = true;
  size_t word_size = 0;
  std::ostringstream word;
  size_t skip = 0;
  int stack_size = 0;

  for (size_t i = 0; i < input.size(); i++) {
    if (skip) {
      skip--;
      continue;
    }
    char c = input[i];
    if (c == '(') {
      push_sexp();
      all_digits = true;
      stack_size++;
    } else if (c == ')') {
      if (word_size) {
        push_string(word.str());
        word.str("");
        word_size = 0;
      }
      all_digits = true;
      Sexp temp = std::get<Sexp>(pop_back());
      push_sexp(temp);
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
        push_string(input.substr(i + i, skip));
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
}
