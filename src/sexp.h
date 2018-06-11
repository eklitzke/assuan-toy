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

// Sexp are lists of nodes, and nodes are strings or sexps.
struct Sexp;
using Node = std::variant<std::string, Sexp>;

// Sexp is a list of Nodes.
class Sexp : public std::list<Node> {
 public:
  explicit Sexp(const std::string &str);

  // Create a one-element Sexp composed of a string.
  static Sexp S(const std::string &s);

  friend std::ostream &operator<<(std::ostream &os, const Sexp &sexp);
  friend const Node &car(const Sexp &sexp);
  friend Sexp cdr(const Sexp &sexp);

 private:
  Sexp() = default;

#if 0
  // FIXME: Ideally copy constructor should be allowed but private, but
  // uncommenting this breaks compilation...
  Sexp(const Sexp &other) = default;
#endif

  void push() { push_back(Sexp()); }
  void push(const Sexp &sexp) { push_back(sexp); }
  void push(const std::string &s) { std::get<Sexp>(back()).push_back(s); }
  void push_sexp(const Sexp &sexp) { std::get<Sexp>(back()).push(sexp); }
  Node pop() {
    Node tmp = back();
    pop_back();
    return tmp;
  }

  Sexp cdr() const {
    Sexp copy = *this;
    copy.pop_front();
    return copy;
  }
};

// Functional interface for car.
inline const Node &car(const Sexp &s) { return s.front(); }

// Functional cdr.
inline Sexp cdr(const Sexp &s) { return s.cdr(); }

// Typed car.
template <typename T>
const T &car(const Sexp &sexp) {
  return std::get<T>(car(sexp));
}

// String car.
inline const std::string &scar(const Sexp &sexp) {
  return car<std::string>(sexp);
}

// Sexp car.
inline const Sexp &xcar(const Sexp &sexp) { return car<Sexp>(sexp); }

// Functional nth;
const Node &nth(const Sexp &sexp, size_t n);

// Typed nth.
template <typename T>
const T &nth(const Sexp &s, size_t n) {
  return std::get<T>(nth(s, n));
}

// String nth;
inline const std::string &snth(const Sexp &s, size_t n) {
  return nth<std::string>(s, n);
}

// Sexp nth;
inline const Sexp &xnth(const Sexp &s, size_t n) { return nth<Sexp>(s, n); }

// Functional cons.
Sexp cons(const Node &node, const Sexp &sexp);

// for iostream printing
std::ostream &operator<<(std::ostream &os, const Sexp &sexp);
std::ostream &operator<<(std::ostream &os, const Node &node);

// thrown when parse fails
class bad_parse : public std::runtime_error {
 public:
  bad_parse(const std::string &why) : std::runtime_error(why) {}
};
