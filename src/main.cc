#include <unistd.h>

#include <assuan.h>
#include <getopt.h>
#include <gpg-error.h>

#include <iostream>
#include <sstream>

#include "./config.h"

class Assuan {
 public:
  Assuan() {
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

  void WriteLine(const std::string &line) {
    gpg_error_t err = assuan_write_line(ctx_, line.c_str());
    if (err != 0) RaiseError("assuan_write_line", err);
  }

  std::string ReadLine() {
    char *line;
    size_t sz = 0;
    gpg_error_t err = assuan_read_line(ctx_, &line, &sz);
    if (err != 0) RaiseError("assuan_read_line", err);
    return std::string(line, sz);
  }

 private:
  assuan_context_t ctx_;

  void RaiseError(const char *string, gpg_error_t err) {
    std::ostringstream os;
    os << "error: " << string << ", gpg_errno = " << err;
    throw std::runtime_error(os.str());
  }
};

std::string GuessSocket() {
  std::ostringstream os;
  os << "/run/user/" << getuid() << "/gnupg/S.gpg-agent";
  return os.str();
}

int main(int argc, char **argv) {
  std::string socket_path;
  static const char short_opts[] = "hs:";
  static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
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

  // initialize libgpg-error
  gpgrt_check_version(nullptr);

  Assuan ctx;
  ctx.SocketConnect(socket_path.empty() ? GuessSocket() : socket_path);
  ctx.WriteLine("GETINFO version");
  std::cout << ctx.ReadLine() << "\n";
  return 0;
}
