#include "./sexp.h"

#include <iostream>
#include <stdexcept>

Sexp SexpParseString(std::string input) {
  std::cout << "INPUT[" << input << "]\n;";
  if (input.empty()) {
    throw std::runtime_error("finished parse");
  }
  if (input.front() != '(') {
    throw std::runtime_error("expected (");
  }
  if (input.back() != ')') {
    std::cerr << input.back() << "\n";
    throw std::runtime_error("expected )");
  }

  std::string data = input.substr(1, data.size() - 2);  // ?
  std::cout << "DATA[" << data << "]\n;";

  size_t idx;
  long len = std::stol(data, &idx);
  std::cout << "len = " << len << ", consumed " << idx << " bytes\n";
  if (len <= 0) {
    throw std::runtime_error("bad count");
  }

  if (data[idx] != ':') {
    throw std::runtime_error("expected :");
  }

  std::string key = data.substr(idx + 1, len);
  std::cout << "KEY[" << key << "]\n;";

  char c = data[idx + 1 + len];
  if (c == '(') {
    std::cout << "subtree\n";  // recurse
  } else if (c == ':') {
    std::cout << "data\n";  // recurse
  } else {
    throw std::runtime_error("expected : or (");
  }
  return {};
}
