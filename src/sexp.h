#pragma once

#include <string>
#include <variant>
#include <vector>

struct Sexp;

using Node = std::variant<std::string, Sexp>;

class Sexp {
 public:
  Sexp() {}
  explicit Sexp(const std::string &input);

  const std::vector<Node> &children() { return children_; }

  size_t size() const { return children_.size(); }

 private:
  std::vector<Node> children_;

  Node &push_sexp(const Sexp &sexp) {
    children_.push_back(sexp);
    return children_.back();
  }

  Node &push_sexp() { return push_sexp({}); }

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
