#include <unistd.h>

#include <getopt.h>

#include <iostream>
#include <sstream>

#include "./config.h"
#include "./gpghelper.h"

int main(int argc, char **argv) {
  bool gen_key = false;
  std::string socket_path;
  static const char short_opts[] = "khs:";
  static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                      {"key", no_argument, 0, 'k'},
                                      {"socket", required_argument, 0, 's'},
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
      case 's':
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

  std::cout << "connected to agent version " << ctx.GetVersion() << "\n";
  if (gen_key) {
    std::cout << "key data is: " << ctx.GenKey() << "\n";
  }

  return 0;
}
