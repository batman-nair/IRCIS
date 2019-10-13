#include <Runner.h>

#include <cctype>

namespace PTrain {

  // Step function processes the current char for Runner and moves it a step
  // Returns false if Runner is dead
  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());

    // --- Integer mode related processing ---
    if (integer_mode_ || current_char == INT_MODE) {
      if (integer_mode_ && current_char == INT_MODE) {
	DBG("Invalid character in integer mode");
	return false;
      }

      integer_mode_ = true;
      if (!is_blank(current_char))
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
    case STACK_MODE:
      mode_ = Mode::STACK;
      stack_mode_ = !stack_mode_;
      break;
    case MOVE_WEST:
      position_.change_dir(Direction::WEST);
      break;
    case MOVE_NORTH:
      position_.change_dir(Direction::NORTH);
      break;
    case MOVE_EAST:
      position_.change_dir(Direction::EAST);
      break;
    case MOVE_SOUTH:
      position_.change_dir(Direction::SOUTH);
      break;
    case PRINT_ENDL:
      log_->print_line();
      break;
    case END_PROC:
      return false;
    case PRINT_CHAR:
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

  bool Runner::process_integer_buffer() {
    auto it = integer_mode_buffer_.begin();
    ++it;			// Skip starting quote char
    char start_ch = *it;
    ++it;
    if (isdigit(start_ch)) {	// Integer processing
      int num = start_ch - '0';
      while (it != integer_mode_buffer_.end()) {
	char ch = *it++;
	if (!isdigit(ch)) {
	  DBG("Non integer character in integer processing.");
	  return false;
	}
	num = num*10 + (ch-'0');
      }
      Data dat(num, true);
      st_.push(dat);
    }
    else {
      const std::string arith_ops = "+-/*%";
      if (arith_ops.find(start_ch) != std::string::npos) {
	if (st_.size() < 2) {
	  DBG("Not enough elements for arithmetic operation");
	  return false;
	}
	Data num1 = st_.top();
	st_.pop();
	Data num2 = st_.top();
	st_.pop();
	if (num1.is_integer && num2.is_integer) {
	  switch(start_ch) {
	  case '+':
	    num1 = num1 + num2;
	    break;
	  case '-':
	    num1 = num1 - num2;
	    break;
	  case '*':
	    num1 = num1 * num2;
	    break;
	  case '/':
	    num1 = num1 / num2;
	    break;
	  case '%':
	    num1 = num1 % num2;
	    break;
	  default:
	    DBG("Unknown arithmetic op found");
	    return false;
	  };
	  st_.push(num1);
	}
      }
    }

    integer_mode_ = false;
    integer_mode_buffer_.clear();
    return true;
  }

  bool Runner::process_mode_buffer() {
    switch(mode_) {
    case NONE:
      DBG("Shouldn't reach process_mode_buffer with NONE mode");
      return false;
    case STACK:
      for (char ch: mode_buffer_) {
	Data data(ch);
	st_.push(data);
      }

      mode_buffer_.clear();
      mode_ = Mode::NONE;
      return true;
    default:
      DBG("Unknown mode_ value specified. mode_: " << mode_);
      return false;
    }
  }


  bool is_mode_end_char(Mode mode, char current_char) {
    if (mode_end_chars[mode].find(current_char) != std::string::npos)
      return true;
    return false;
  }

}
