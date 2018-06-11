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

#include <ostream>
#include <string>

// Escape a string.
struct StringEscape : std::string {
  explicit StringEscape(const std::string &s) : std::string(s) {}
};
std::ostream &operator<<(std::ostream &os, const StringEscape &esc);

std::string AssuanEncode(const std::string &s);
std::string AssuanDecode(const std::string &s);
