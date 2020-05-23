#include <DirVec.h>
#include <Logger.h>

namespace Ircis {
  Direction get_right(const Direction& dir) {
    switch(dir) {
    case NORTH:  return EAST;
    case EAST:   return SOUTH;
    case SOUTH:  return WEST;
    case WEST:   return NORTH;
    default:     return NORTH;
    }
  }
  Direction get_left(const Direction& dir) {
    switch(dir) {
    case NORTH:  return WEST;
    case EAST:   return NORTH;
    case SOUTH:  return EAST;
    case WEST:   return SOUTH;
    default:     return NORTH;
    }
  }
  Direction from_char(char start_direction_char) {
    switch(start_direction_char) {
    case 'N':  return NORTH;
    case 'E':  return EAST;
    case 'S':  return SOUTH;
    case 'W':  return WEST;
    default:
      Logger::err_line_dbg("Got invalid direction value: ", start_direction_char, ", using default EAST");
      return EAST;
    }
  }

  std::ostream& operator<<(std::ostream& os, const Direction& dir) {
    char ch = 'X';
    switch (dir) {
    case NORTH: ch = 'N';    break;
    case EAST:  ch = 'E';    break;
    case WEST:  ch = 'W';    break;
    case SOUTH: ch = 'S';    break;
    }
    return os << ch;
  }

  void DirVec::update() {
    int dx = 0, dy = 0;
    switch (direction) {
    case NORTH: dy = -1;    break;
    case WEST:  dx = -1;    break;
    case EAST:  dx = 1;     break;
    case SOUTH: dy = 1;     break;
    }

    xx += dx;
    yy += dy;
  }

  std::ostream& operator<<(std::ostream& os, const DirVec& pos) {
    return os << "(" << pos.xx << "," << pos.yy << "," << pos.direction << ")";
  }
}
