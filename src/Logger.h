#pragma once

#include <iostream>

#include <string>

#ifdef DEBUG
#define DBG(x) std::cout << "DEBUG: " << x << std::endl;
#define PR(x) (#x) << ": " << x << " "
#else
#define DBG(x) ;
#define PR(x) ;
#endif

class Logger {
 public:
  void print(std::string str) {
    std::cout << str;
  }
  void print(char ch) {
    std::cout << ch;
  }

  void printl(std::string str) {
    std::cout << str << std::endl;
  }
};
