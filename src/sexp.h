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

#pragma once

#include <list>
#include <ostream>
#include <string>
#include <variant>

struct Sexp;

using Node = std::variant<std::string, Sexp>;

class Sexp {
 public:
  Sexp() {}
  explicit Sexp(std::list<Node> children) : children_(children) {}
  explicit Sexp(const std::string &input);

  const std::list<Node> &children() const { return children_; }
  size_t size() const { return children_.size(); }
  bool empty() const { return children_.empty(); }

 private:
  std::list<Node> children_;

  Node &push_sexp(const Sexp &sexp) {
    children_.push_back(sexp);
    return children_.back();
  }

  Node &push_sexp() { return push_sexp({}); }

  Node &push_back_sexp(const Sexp &sexp) {
    Sexp &back = std::get<Sexp>(children_.back());
    back.push_sexp(sexp);
    return back.children_.back();
  }

  void push_string(const std::string &s) {
    Sexp &back = std::get<Sexp>(children_.back());
    back.append_string(s);
  }

  void append_string(const std::string &s) { children_.push_back(s); }

  Node pop_back() {
    Node ret = children_.back();
    children_.pop_back();
    return ret;
  }
};

// print a sexp
std::ostream &operator<<(std::ostream &os, const Sexp &sexp);

// print a node
std::ostream &operator<<(std::ostream &os, const Node &node);

// standard car/cdr routines
const Node &car(const Sexp &sexp);
const Node &car(const Node &node);
Sexp cdr(const Sexp &sexp);
