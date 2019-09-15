#include <Runner.h>

#include <cctype>

namespace PTrain {

  bool Runner::step() {
    int current_char = grid->get(position.get_x(), position.get_y());

    position.update();
    if (!grid->is_inside(position.get_x(), position.get_y()))
      return false;

    log->print((char)current_char);

    return true;
  }

}
