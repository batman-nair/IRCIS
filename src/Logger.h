#pragma once

#include <iostream>
#include <string>
#include <fstream>

template <typename T>
void DBG_slave(T val) {
  std::cout << val << std::endl;
}
template <typename T, typename... Types>
void DBG_slave(T val, Types... vars) {
  std::cout << val;
  DBG_slave(vars...);
}

#ifndef NDEBUG
#define DBG(...) DBG_slave( "", ## __VA_ARGS__ );
#define PN(x) (#x) << ": " << x << " "
#else
#define DBG(...)
#define PN(x)
#endif

namespace Ircis {
  class Logger {
  public:
    Logger(std::string file_name) : output_file(file_name) { }

    template <class T>
    void print(T val) {
      std::cout << val;
      output_file << val;
    }
    void print_line() {
      std::cout << std::endl;
      output_file << std::endl;
    }
    template <class T>
    void print_line(T val) {
      std::cout << val << std::endl;
      output_file << val << std::endl;
    }

  private:
    std::ofstream output_file;

  public:
    template <typename... Types>
    static void log_line_dbg(Types... vars) {
      log_line(vars...);
      DBG("DEBUG: ", vars...);
    }
    template <typename... Types>
    static void err_line_dbg(Types... vars) {
      err_line(vars...);
      DBG("ERROR: ", vars...);
    }
    template <typename... Types>
    static void log_line(Types... vars) {
      log_file << "DEBUG: ";
      log_vars_slave(vars...);
      log_file << std::endl;
    }
    template <typename... Types>
    static void err_line(Types... vars) {
      log_file << "ERROR: ";
      log_vars_slave(vars...);
      log_file << std::endl;
    }

  private:
    static std::ofstream log_file;
    template <typename T>
    static void log_vars_slave(T val) {
      log_file << val;
    }
    template <typename T, typename... Types>
    static void log_vars_slave(T val, Types... vars) {
      log_file << val;
      log_vars_slave(vars...);
    }
  };

}
