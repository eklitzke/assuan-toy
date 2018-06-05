#pragma once

#include <assuan.h>

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
  Assuan() {
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

 private:
  assuan_context_t ctx_;

  std::string ReadLine() {
    char *line;
    size_t sz = 0;
    gpg_error_t err = assuan_read_line(ctx_, &line, &sz);
    if (err != 0) RaiseError("assuan_read_line", err);
    return std::string(line, sz);
  }

  void WriteLine(const std::string &line) {
    gpg_error_t err = assuan_write_line(ctx_, line.c_str());
    if (err != 0) RaiseError("assuan_write_line", err);
  }

  std::string ReadData() {
    std::string data;
    for (;;) {
      std::string line = ReadLine();
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
