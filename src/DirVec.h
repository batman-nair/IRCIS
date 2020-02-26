#pragma once

#include <iostream>
#include <vector>

namespace Ircis {
  enum Direction {
		  NORTH,
		  EAST,
		  WEST,
		  SOUTH
  };
  Direction get_right(const Direction& dir);
  Direction get_left(const Direction& dir);
  Direction from_char(char start_direction_char);


  class DirVec {
  public:

    DirVec(int start_x, int start_y, Direction start_dir)
      : xx(start_x), yy(start_y), direction(start_dir) { }

    DirVec(int start_x, int start_y, char start_direction_char)
      : xx(start_x), yy(start_y), direction(from_char(start_direction_char)) { }

    void update();

    void set_position(int new_x, int new_y) {
      xx = new_x;
      yy = new_y;
    }
    void change_dir(const Direction new_dir) { direction = new_dir; }
    Direction get_direction() const { return direction; }
    unsigned int get_x() const { return xx; }
    unsigned int get_y() const { return yy; }

    Direction get_left() const { return Ircis::get_left(direction); }
    Direction get_right() const { return Ircis::get_right(direction); }


    void move(const Direction& dir) {
      change_dir(dir);
      update();
    }

    friend std::ostream& operator<<(std::ostream& os, const DirVec& pos);

  private:
    unsigned int xx, yy;
    Direction direction;
  };

  std::ostream& operator<<(std::ostream& os, const Direction& dir);
  std::ostream& operator<<(std::ostream& os, const DirVec& pos);

  struct MovementData {
    std::vector<DirVec> path;
    unsigned int alive_from = 0;
    unsigned int alive_till = 0;
  };
}
