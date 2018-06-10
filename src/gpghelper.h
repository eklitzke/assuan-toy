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

#include <assuan.h>

#include <iostream>
#include <sstream>
#include <string>

// Ensure libgpg-error is intializied
void InitGpgerror();

// Guess the socket name.
std::string GuessGPGSocket();

enum class KeyType {
  BITCOIN = 1,
};

class Assuan {
 public:
  Assuan() : trace_(false) {
    InitGpgerror();

    assuan_log_cb_t log_cb;
    void *log_cb_data;
    assuan_get_log_cb(&log_cb, &log_cb_data);

    gpg_error_t err =
        assuan_new_ext(&ctx_, assuan_get_gpg_err_source(),
                       assuan_get_malloc_hooks(), log_cb, log_cb_data);
    if (err != 0) RaiseError("assuan_new_ext", err);
  }
  Assuan(const Assuan &other) = delete;
  ~Assuan() { assuan_release(ctx_); }

  void SocketConnect(const std::string &name,
                     pid_t server_pid = ASSUAN_INVALID_PID,
                     unsigned int flags = 0) {
    gpg_error_t err =
        assuan_socket_connect(ctx_, name.c_str(), server_pid, flags);
    if (err != 0) RaiseError("assuan_socket_connect", err);
  }

  std::string GetVersion() {
    WriteLine("GETINFO version");
    return ReadData();
  }

  std::string GenKey(KeyType type = KeyType::BITCOIN) {
    WriteLine("GENKEY");
    for (;;) {
      if (ReadLine() == "INQUIRE KEYPARAM") {
        break;
      }
    }
    WriteLine("D (genkey(ecc(curve 9:secp256k1)(flags nocomp)))");
    WriteLine("END");
    return ReadData();
  }

  void SetTrace() { trace_ = true; }

 private:
  assuan_context_t ctx_;
  bool trace_;

  std::string ReadLine() {
    char *line;
    size_t sz = 0;
    gpg_error_t err = assuan_read_line(ctx_, &line, &sz);
    if (err != 0) RaiseError("assuan_read_line", err);
    return std::string(line, sz);
  }

  void WriteLine(const std::string &line) {
    if (trace_) {
      std::cerr << "C: " << line << "\n";
    }
    gpg_error_t err = assuan_write_line(ctx_, line.c_str());
    if (err != 0) RaiseError("assuan_write_line", err);
  }

  std::string ReadData() {
    std::string data;
    for (;;) {
      std::string line = ReadLine();
      if (trace_) {
        std::cerr << "S: " << line << "\n";
      }
      if (line == "OK") {
        break;
      } else if (line.substr(0, 2) == "D ") {
        data = line.substr(2);
      }
    }
    return data;
  }

  void RaiseError(const char *string, gpg_error_t err) {
    std::ostringstream os;
    os << "error: " << string << ", gpg_errno = " << err;
    throw std::runtime_error(os.str());
  }
};
