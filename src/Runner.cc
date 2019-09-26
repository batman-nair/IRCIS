#include <Runner.h>

#include <cctype>

namespace PTrain {

  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());

    if (stack_mode_ && current_char != '"') {
      Data data(current_char);
      st_.push(data);
    }
    else if (!process_char(current_char)) {
      // Character could not be processed
      return false;
    }

    position_.update();
    if (!grid_->is_inside(position_.get_x(), position_.get_y()))
      return false;

    return true;
  }

  bool Runner::process_char(char current_char) {
    switch(current_char) {
    case '"':
      stack_mode_ = !stack_mode_;
      break;
    case '<':
      position_.change_dir(Direction::EAST);
      break;
    case '^':
      position_.change_dir(Direction::NORTH);
      break;
    case '>':
      position_.change_dir(Direction::WEST);
      break;
    case 'v':
      position_.change_dir(Direction::SOUTH);
      break;
    case '$':
      log_->print_line();
      break;
    case '!':
      return false;
    case '#':
      if (st_.empty()) {
	log_->print_line();
	DBG("Stack is empty, forcing exit.");
	return false;
      }
      auto top = st_.top();
      st_.pop();
      if (top.is_integer) {
	log_->print(top.value);
      }
      else {
	log_->print((char)top.value);
      }
      break;
    }
    return true;
  }

}
