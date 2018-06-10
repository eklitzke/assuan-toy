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

#include "./gpghelper.h"

#include <gpg-error.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <cstring>

static std::atomic<bool> did_gpg_error_init(false);
static const char socketkey[] = "agent-socket:";

// The fallback option, this could be a little smarter.
static std::string DefaultSocket() {
  std::ostringstream os;
  os << "/run/user/" << getuid() << "/gnupg/S.gpg-agent";
  return os.str();
}

void InitGpgerror() {
  if (!did_gpg_error_init.load()) {
    gpgrt_check_version(nullptr);
    did_gpg_error_init = true;
  }
}

// The full logic in GnuPG to choose the socket dir is complicated and depends
// on various compile time options. As a hack, we call "gpgconf --list-dirs" and
// use the agent-socket listed in the output.
std::string GuessGPGSocket() {
  // shell out to gpgconf --list-dirs
  FILE* f = popen("gpgconf --list-dirs", "re");
  if (f == nullptr) {
    return DefaultSocket();
  }

  // find the line of output
  char* line = nullptr;
  size_t len;
  ssize_t nread;
  std::string result;
  const size_t keylen = strlen(socketkey);
  while ((nread = getline(&line, &len, f)) != -1) {
    if (strncmp(line, socketkey, keylen) == 0) {
      result = {&line[keylen], nread - keylen - 1};
      break;
    }
  }
  free(line);
  fclose(f);

  return result.empty() ? DefaultSocket() : result;
}
