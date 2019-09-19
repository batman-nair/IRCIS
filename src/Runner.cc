#include <Runner.h>

#include <cctype>

namespace PTrain {

  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());

    position_.update();
    if (!grid_->is_inside(position_.get_x(), position_.get_y()))
      return false;

    log_->print((char)current_char);

    return true;
  }

}
