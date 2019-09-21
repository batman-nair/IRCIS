#pragma once

#include <iostream>
#include <string>

#ifndef NDEBUG
#define DBG(x) std::cout << "DEBUG: " << x << std::endl;
#define PR(x) (#x) << ": " << x << " "
#else
#define DBG(x) ;
#define PR(x)
#endif

namespace PTrain {
  class Logger {
  public:
    template <class T>
    void print(T val) {
      std::cout << val;
    }

    template <class T>
    void print_line(T val) {
      std::cout << val << std::endl;
    }
  };
}
