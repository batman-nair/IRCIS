#pragma once

#include <DirVec.h>

#include <string>
#include <vector>

namespace PTrain {
  class Grid {
  public:
    Grid(std::string file_name);

    char get(const DirVec& pos) const {
      return lines_[pos.get_y()][pos.get_x()];
    }
    char get(std::size_t xx, std::size_t yy) const {
      return lines_[yy][xx];
    }
    bool is_inside(std::size_t xx, std::size_t yy) const {
      if (xx > width_ || yy > height_) // Unsigned,, so always > 0
	return false;
      return true;
    }
    bool is_inside(const DirVec& pos) const {
      if (pos.get_x() > width_ || pos.get_y() > height_) // Unsigned,, so always > 0
	return false;
      return true;
    }

    const std::vector< std::string >& get_lines() {
      return lines_;
    }
  private:
    // Read file as lines_ and set height and width
    void read_file(std::string file_name);
    // Sets all lines to be of same size
    void equalize_lines();

    std::vector< std::string > lines_;
    std::size_t height_, width_;
  };
}
