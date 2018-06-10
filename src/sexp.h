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
#include <stdexcept>
#include <string>
#include <variant>

// An S-expression is a list of nodes.
struct Sexp;
using Node = std::variant<std::string, Sexp>;

// Functional interfaces for car and cdr.
const Node &car(const Sexp &sexp);
const Node &car(const Node &node);
Sexp cdr(const Sexp &sexp);

class Sexp {
 public:
  // Create a Sexp from a parse string.
  explicit Sexp(const std::string &input);

  size_t size() const { return nodes_.size(); }
  bool empty() const { return nodes_.empty(); }

  friend std::ostream &operator<<(std::ostream &, const Sexp &);
  friend const Node &car(const Sexp &);
  friend Sexp cdr(const Sexp &);

 private:
  std::list<Node> nodes_;

  Sexp() {}
  explicit Sexp(std::list<Node> children) : nodes_(children) {}

  void push() { nodes_.push_back(Sexp()); }

  void push(const Sexp &sexp) { nodes_.push_back(sexp); }

  void push(const std::string &s) {
    std::get<Sexp>(nodes_.back()).nodes_.push_back(s);
  }

  void push_back(const Sexp &sexp) { std::get<Sexp>(nodes_.back()).push(sexp); }

  Node pop() {
    Node ret = nodes_.back();
    nodes_.pop_back();
    return ret;
  }
};

// for iostream printing
std::ostream &operator<<(std::ostream &os, const Sexp &sexp);
std::ostream &operator<<(std::ostream &os, const Node &node);

// thrown when parse fails
class bad_parse : public std::runtime_error {
 public:
  bad_parse(const std::string &why) : std::runtime_error(why) {}
};
