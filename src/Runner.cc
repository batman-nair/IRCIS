#include <Runner.h>

#include <cctype>

namespace PTrain {

  // Step function processes the current char for Runner and moves it a step
  // Returns false if Runner is dead
  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());

    // --- Integer mode related processing ---
    if (integer_mode_ || current_char == CH_INT) {
      if (integer_mode_ && current_char == CH_INT) {
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
    // --- Mode processing ---
    else if (mode_ != Mode::NONE) {
      if (!is_mode_end_char(mode_, current_char))
	mode_buffer_.push_back(current_char);
      else {
	if (!process_mode_buffer())
	  return false;
      }
    }
    // --- Any other char processing ---
    else if (!process_char(current_char)) {
      // Character could not be processed
      return false;
    }

    // --- Update Runner position after processing ---
    position_.update();
    if (!grid_->is_inside(position_.get_x(), position_.get_y()))
      return false;

    return true;
  }

  // Simple character processing when not in any mode
  bool Runner::process_char(char current_char) {
    switch(current_char) {
    case CH_STACK:
      mode_ = Mode::STACK;
      stack_mode_ = !stack_mode_;
      break;
    case CH_WEST:
      position_.change_dir(Direction::WEST);
      break;
    case CH_NORTH:
      position_.change_dir(Direction::NORTH);
      break;
    case CH_EAST:
      position_.change_dir(Direction::EAST);
      break;
    case CH_SOUTH:
      position_.change_dir(Direction::SOUTH);
      break;
    case CH_PUSH:
      mode_ = Mode::STACK_PUSH;
      break;
    case CH_POP:
      mode_ = Mode::STACK_POP;
      break;
    case CH_ENDL:
      log_->print_line();
      break;
    case CH_END:
      return false;
    case CH_PRINT:
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
      if (is_arith(start_ch)) {
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
	  case CH_ADD:
	    num1 = num1 + num2;
	    break;
	  case CH_SUB:
	    num1 = num1 - num2;
	    break;
	  case CH_MUL:
	    num1 = num1 * num2;
	    break;
	  case CH_DIV:
	    num1 = num1 / num2;
	    break;
	  case CH_MOD:
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
      {
	for (char ch: mode_buffer_) {
	  Data data(ch);
	  st_.push(data);
	}
	break;
      }
    case STACK_PUSH:
      {
	int num = 0;
	for (char ch: mode_buffer_) {
	  if (!isdigit(ch)) {
	    DBG("Invalid character in Stack Pop mode!");
	    return false;
	  }
	  num = num*10 + (ch-'0');
	}
	Data temp = st_[-num];
	st_.push(temp);
	break;
      }
    case STACK_POP:
      {
	int num = 0;
	for (char ch: mode_buffer_) {
	  if (!isdigit(ch)) {
	    DBG("Invalid character in Stack Pop mode!");
	    return false;
	  }
	  num = num*10 + (ch-'0');
	}
	for (int ii = 0; ii < num; ++ii) {
	  if (st_.empty()) {
	    DBG("Stack pop preemptive finish");
	    break;
	  }
	  st_.pop();
	}
	break;
      }
    default:
      DBG("Unknown mode_ value specified. mode_: " << mode_);
      return false;
    }
    mode_buffer_.clear();
    mode_ = Mode::NONE;
    return true;
  }


  bool is_mode_end_char(Mode mode, char current_char) {
    if (mode_end_chars[mode].find(current_char) != std::string::npos)
      return true;
    return false;
  }

}
