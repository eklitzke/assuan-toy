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

#include <unistd.h>

#include <getopt.h>

#include <iostream>
#include <sstream>

#include "./config.h"
#include "./gpghelper.h"
#include "./sexp.h"

int main(int argc, char **argv) {
  bool gen_key = false, trace = false;
  std::string socket_path;
  static const char short_opts[] = "khs:t";
  static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                      {"key", no_argument, 0, 'k'},
                                      {"socket", required_argument, 0, 's'},
                                      {"trace", no_argument, 0, 't'},
                                      {0, 0, 0, 0}};
  for (;;) {
    int c = getopt_long(argc, argv, short_opts, long_opts, nullptr);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 'h':
        std::cout << PACKAGE_STRING << "\n\n";
        std::cout << "usage: " << PACKAGE << " [-h] [-s SOCKET]\n";
        return 0;
        break;
      case 'k':
        gen_key = true;
        break;
      case 's':
        socket_path = optarg;
        break;
      case 't':
        trace = true;
        break;
      case 'v':
        socket_path = optarg;
        break;
      case '?':
        // getopt_long should already have printed an error message
        break;
      default:
        std::cerr << "unrecognized command line flag: " << optarg << "\n";
        abort();
    }
  }
#if 0

  Assuan ctx;
  ctx.SocketConnect(socket_path.empty() ? GuessGPGSocket() : socket_path);
  if (trace) {
    ctx.SetTrace();
  }

  std::cout << "connected to agent version " << ctx.GetVersion() << "\n";
  if (gen_key) {
    std::cout << "key data is: " << ctx.GenKey() << "\n";
  }
#endif
  try {
    Sexp txt("(genkey(ecc(curve 9:secp256k1)(flags nocomp)))");
    std::cout << txt << "\n";
    if (auto pval = std::get_if<std::string>(&txt.children().front()))
      std::cout << "first child is a string\n";
    else
      std::cout << "first child is a sexp\n";
    if (auto pval = std::get_if<std::string>(&car(txt)))
      std::cout << "car is a string\n";
    else
      std::cout << "car is a sexp\n";
    std::cout << "car is: " << car(txt) << "\n";
    std::cout << "cdr is: " << cdr(txt) << "\n";
#if 0
    Sexp bin(
        {"(10:public-key(3:ecc(5:curve9:secp256k1)(1:q65:"
         "\x04YU\x89\xa5\x05\xa4\xd8~\x07%0D^\x9a\xe4\x7f\xb4\xd8\x1c\xab)"
         "WH\xa1\x88\x84\xb0\x0b\xc6\xbb\xb4\xfdQ\xfc\x17\x15\x00\x85.\xcb("
         "\x1c\x1a\x0e\xdd\xa1S)\xa6wR\xce\x7f\x08\xe2\xb9hN\xccQ\xe0x\x16'"
         "\xe5?"
         ")))",
         117});
    std::cout << bin << " ";
#endif
  } catch (const std::exception &exc) {
    std::cerr << "ERROR: " << exc.what() << "\n";
    return 1;
  }

  return 0;
}
