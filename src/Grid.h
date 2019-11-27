#pragma once

#include <DirVec.h>

#include <string>
#include <vector>

namespace Ircis {
  class Grid {
  public:
    Grid(std::string file_name);

    char get(const DirVec& pos) const {
      if (!is_inside(pos))  throw GridOutOfBoundsException(pos);
      return lines_[pos.get_y()][pos.get_x()];
    }
    char get(std::size_t xx, std::size_t yy) const {
      if (!is_inside(xx, yy))  throw GridOutOfBoundsException(xx, yy);
      return lines_[yy][xx];
    }
    bool is_inside(std::size_t xx, std::size_t yy) const {
      if (xx >= width_ || yy >= height_) // Unsigned,, so always >= 0
	return false;
      return true;
    }
    bool is_inside(const DirVec& pos) const {
      if (pos.get_x() >= width_ || pos.get_y() >= height_) // Unsigned,, so always >= 0
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

  public:
    struct GridFileNotFoundException : public std::exception {
      GridFileNotFoundException(std::string file_name)
	: file_name_(file_name) { }
      const char* what() const throw () {
	static std::string temp = std::string("File ") + file_name_ + " not found";
	return temp.c_str();
      }
      std::string file_name_;
    };
    struct GridOutOfBoundsException : public std::exception {
      GridOutOfBoundsException(const DirVec& pos)
	: xx_(pos.get_x()), yy_(pos.get_y()) { }
      GridOutOfBoundsException(std::size_t xx, std::size_t yy)
	: xx_(xx), yy_(yy) { }
      const char* what() const throw () {
      	static std::string temp = std::string("Point (") + std::to_string(xx_) + ", " + std::to_string(yy_) + ") is outside grid";
      	return temp.c_str();
      }
      std::size_t xx_;
      std::size_t yy_;
    };
  };

}
