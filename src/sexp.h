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

#include <exception>
#include <list>
#include <ostream>
#include <string>
#include <variant>

class bad_parse : public std::runtime_error {
 public:
  bad_parse(const std::string &why) : std::runtime_error(why) {}
};

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

  void push() { children_.push_back(Sexp()); }

  void push(const Sexp &sexp) { children_.push_back(sexp); }

  void push(const std::string &s) {
    std::get<Sexp>(children_.back()).children_.push_back(s);
  }

  void push_back(const Sexp &sexp) {
    std::get<Sexp>(children_.back()).push(sexp);
  }

  Node pop() {
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
