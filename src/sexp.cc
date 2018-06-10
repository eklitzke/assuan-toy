// Copyright (c) 2018 Evan Klitzke <evan@eklitzke.org>
//
// This file is part of assuan-toy.
//
// assuan-toy is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// assuan-toy is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// assuan-toy. If not, see <http://www.gnu.org/licenses/>.

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
  Sexp child = std::get<Sexp>(children_.front());
  *this = std::get<Sexp>(child.children_.front());
}

std::ostream &operator<<(std::ostream &os, const Sexp &sexp) {
  os << "(";
  const std::list<Node> &children = sexp.children();
  if (!children.empty()) {
    auto ultimate = children.end();
    ultimate--;
    for (auto it = children.begin(); it != ultimate; it++) {
      os << *it << " ";
    }
    os << children.back();
  }
  os << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
  std::visit([&os](auto &&arg) { os << arg; }, node);
  return os;
}

const Node &car(const Sexp &sexp) { return sexp.children().front(); }

const Node &car(const Node &sexp) {
  return car(std::get<Sexp>(sexp).children().front());
}

Sexp cdr(const Sexp &sexp) {
  std::list<Node> copy = sexp.children();
  copy.pop_front();
  return Sexp(copy);
}
