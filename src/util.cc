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

#include "./util.h"

#include <sstream>

const char kHexdig[] = "0123456789ABCDEF";

std::ostream &operator<<(std::ostream &os, const StringEscape &esc) {
  for (unsigned char c : esc.s) {
    if (' ' <= c and c <= '~' and c != '\\' and c != '"') {
      os << c;
    } else {
      os << '\\';
      switch (c) {
        case '"':
          os << '"';
          break;
        case '\\':
          os << '\\';
          break;
        case '\t':
          os << 't';
          break;
        case '\r':
          os << 'r';
          break;
        case '\n':
          os << 'n';
          break;
        default:
          os << 'x' << kHexdig[c >> 4] << kHexdig[c & 0xF];
      }
    }
  }
  return os;
}

std::string AssuanDecode(const std::string &str) {
  std::ostringstream os;

  for (size_t i = 0; i < str.size(); i++) {
    char c = str[i];
    if (c == '%') {
      int ii;
      sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
      c = static_cast<char>(ii);
      i = i + 2;
    }
    os << c;
  }
  return os.str();
}
