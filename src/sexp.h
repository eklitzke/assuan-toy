#pragma once

#include <ostream>
#include <string>
#include <variant>
#include <vector>

struct Sexp;

using Node = std::variant<std::string, Sexp>;

class Sexp {
 public:
  Sexp() {}
  explicit Sexp(const std::string &input);

  const std::vector<Node> &children() const { return children_; }
  size_t size() const { return children_.size(); }
  bool empty() const { return children_.empty(); }

 private:
  std::vector<Node> children_;

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

// std::ostream &operator<<(std::ostream &os, const Node &node);
std::ostream &operator<<(std::ostream &os, const Sexp &sexp);
