#include <Runner.h>

#include <cctype>

namespace PTrain {

  // Step function processes the current char for Runner and moves it a step
  // Returns false if Runner is dead
  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());

    // --- Integer mode related processing ---
    if (integer_mode_ || current_char == '\'') {
      if (integer_mode_ && current_char == '\'') {
	DBG("Invalid character in integer mode");
	return false;
      }

      integer_mode_ = true;
      if (current_char != '.')
	integer_mode_buffer_.push_back(current_char);
      else {
	if (!process_integer_buffer())
	  return false;
      }
    }
    else if (mode_ != Mode::NONE) {
      if (!is_mode_end_char(mode_, current_char))
	mode_buffer_.push_back(current_char);
      else {
	if (!process_mode_buffer())
	  return false;
      }
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

  // Simple character processing when not in any mode
  bool Runner::process_char(char current_char) {
    switch(current_char) {
    case '"':
      mode_ = Mode::STACK;
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
      log_->print(top.to_string());
      break;
    }
    return true;
  }

  bool Runner::process_mode_buffer() {
    switch(mode_) {
    case NONE:
      DBG("Shouldn't reach process_mode_buffer with NONE mode");
      DBG("mode value: " << mode_);
      return false;
    case STACK:
      // TODO: with custom stack
      return false;
    }
  }


  bool is_mode_end_char(Mode mode, char current_char) {
    if (mode_end_chars[mode].find(current_char) != std::string::npos)
      return true;
    return false;
  }

}
