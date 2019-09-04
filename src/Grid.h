#pragma once

#include <string>
#include <vector>

class Grid {
 public:
  Grid(std::string file_name);

  char get(std::size_t xx, std::size_t yy) {
    return lines_[yy][xx];
  }

  const std::vector< std::string >& get_lines() {
    return lines_;
  }
 private:
  // Read file as lines_ and set length and width
  void read_file(std::string file_name);
  // Sets all lines to be of same size
  void equalize_lines();

  std::vector< std::string > lines_;
  std::size_t length_, width_;
};
