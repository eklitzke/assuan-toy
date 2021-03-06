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

#include <getopt.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <sstream>

#include "./config.h"
#include "./gpghelper.h"
#include "./sexp.h"
#include "./util.h"

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
        std::cout << "usage: " << PACKAGE
                  << " [-h|--help] [-k|--key] [-t|--trace] [-s SOCKET]\n";
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

  Assuan ctx;
  ctx.SocketConnect(socket_path.empty() ? GuessGPGSocket() : socket_path);
  if (trace) {
    ctx.SetTrace();
  }

  std::cout << "connected to agent version " << ctx.GetVersion() << "\n";
  if (gen_key) {
    const std::string key_str = ctx.GenKey();
    try {
      Sexp key(key_str);
      std::cout << "key sexp: " << key << "\n";
      assert(scar(key) == "public-key");
      key = xcar(cdr(key));
      assert(scar(key) == "ecc");
      key = cdr(key);
      assert(key.size() == 2);
      assert(snth(xcar(key), 0) == "curve");
      assert(snth(xcar(key), 1) == "secp256k1");
      key = xcar(cdr(key));
      assert(key.size() == 2);
      assert(scar(key) == "q");
      assert(scar(cdr(key)).size() == 65);
    } catch (bad_parse &exc) {
      std::cerr << "ERROR: " << exc.what() << "\n";
      return 1;
    }
  }
  return 0;
}
