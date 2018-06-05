#pragma once

#include <string>
#include <variant>
#include <vector>

struct Sexp {
  std::vector<std::variant<std::string, Sexp>> children;
};

Sexp SexpParseString(std::string input);
