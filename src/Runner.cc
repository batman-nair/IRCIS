#include <Runner.h>

#include <cctype>
#include <sstream>

namespace PTrain {
  void Runner::run_debug() {
    Logger::log_line("Runner ", id_, " died");
    Logger::log_line("Processed chars: ", processed_chars_);
    Logger::err_line(err_str_);
    std::ostringstream os;
    while (!st_.empty()) {
      os << st_.top().to_string() << ", ";
      Logger::log_line("Stack value popped", get_id(), ": ", st_.top());
      st_.pop();
    }
    Logger::log_line("Stack values: ", os.str());
  }

  // Step function processes the current char for Runner and moves it a step
  // Returns false if Runner is dead
  bool Runner::step() {
    int current_char = grid_->get(position_.get_x(), position_.get_y());
    processed_chars_.push_back(current_char);

    // --- Integer mode related processing ---
    if (integer_mode_ || current_char == CH_INT) {
      if (integer_mode_ && current_char == CH_INT) {
	set_error("Quote character is invalid in integer mode");
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
      Logger::err_line("Character could not be processed");
      return false;
    }

    // --- Update Runner position after processing ---
    position_.update();
    if (!grid_->is_inside(position_)) {
      set_error("Runner went outside grid");
      return false;
    }

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
    case CH_SPLIT:
      if (!process_split()) {
	return false;
      }
      break;
    case CH_CHECK:
      {
	if (st_.top().value) {
	  break;
	}
	// Check if char exists in right or left and go there
	auto templ = position_;
	templ.move(position_.get_left());
	if (grid_->is_inside(templ) && !is_blank(grid_->get(templ))) {
	  position_.change_dir(position_.get_left());
	  break;
	}
	auto tempr = position_;
	tempr.move(position_.get_right());
	if (grid_->is_inside(tempr) && !is_blank(grid_->get(tempr))) {
	  position_.change_dir(position_.get_right());
	  break;
	}
	set_error("False direction for check not found.");
	return false;
      }
    case CH_END:
      set_error("End character reached");
      return false;
    case CH_PRINT:
      if (st_.empty()) {
	log_->print_line();
	set_error("Stack is empty, forcing exit.");
	return false;
      }
      auto top = st_.top();
      Logger::log_line("Stack value popped", get_id(), ": ", st_.top());
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
	  set_error("Non integer character in integer processing.");
	  return false;
	}
	num = num*10 + (ch-'0');
      }
      Data dat(num, true);
      Logger::log_line("Pushing value to stack", get_id(), ": ", dat);
      st_.push(dat);
    }
    else {
      if (is_arith(start_ch)) {
	if (st_.size() < 2) {
	  set_error("Not enough elements for arithmetic operation");
	  return false;
	}
	Data num1 = st_.top();
	Logger::log_line("Stack value popped", get_id(), ": ", st_.top());
	st_.pop();
	Data num2 = st_.top();
	Logger::log_line("Stack value popped", get_id(), ": ", st_.top());
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
	    if (num2.value == 0) {
	      set_error("Division by zero error");
	      return false;
	    }
	    num1 = num1 / num2;
	    break;
	  case CH_MOD:
	    num1 = num1 % num2;
	    break;
	  default:
	    set_error("Unknown arithmetic op found");
	    return false;
	  };
	  Logger::log_line("Pushing value to stack", get_id(), ": ", num1);
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
      set_error("Shouldn't reach process_mode_buffer with NONE mode");
      return false;
    case STACK:
      {
	for (char ch: mode_buffer_) {
	  Data data(ch);
	  Logger::log_line("Pushing value to stack", get_id(), ": ", data);
	  st_.push(data);
	}
	break;
      }
    case STACK_PUSH:
      {
	int num = 0;
	for (char ch: mode_buffer_) {
	  if (!isdigit(ch)) {
	    set_error("Invalid character in Stack Pop mode!");
	    return false;
	  }
	  num = num*10 + (ch-'0');
	}
	Data temp = st_[-num];
	Logger::log_line("Pushing value to stack", get_id(), ": ", temp);
	st_.push(temp);
	break;
      }
    case STACK_POP:
      {
	int num = 0;
	for (char ch: mode_buffer_) {
	  if (!isdigit(ch)) {
	    set_error("Invalid character in Stack Pop mode!");
	    return false;
	  }
	  num = num*10 + (ch-'0');
	}
	for (int ii = 0; ii < num; ++ii) {
	  if (st_.empty()) {
	    Logger::log_line("Stack pop preemptive finish");
	    break;
	  }
	  Logger::log_line("Stack value popped", get_id(), ": ", st_.top());
	  st_.pop();
	}
	break;
      }
    default:
      set_error("Unknown mode_ value specified. mode_: ", mode_);
      return false;
    }
    mode_buffer_.clear();
    mode_ = Mode::NONE;
    return true;
  }

  bool Runner::process_split() {
    DirVec curr_position = position_;
    DirVec temp = curr_position;
    Logger::log_line("Got split at ", temp);
    bool create_runner = false;
    temp.change_dir(Direction::EAST);
    temp.update();
    if (grid_->is_inside(temp))
      Logger::log_line("EAST char: ", grid_->get(temp));
    if (grid_->is_inside(temp) && grid_->get(temp) == CH_EAST) {
      if (!create_runner) {
	position_.change_dir(Direction::EAST);
	create_runner = true;
      }
      else
	new_runners_list_->push({temp, st_});
    }
    temp = curr_position;
    temp.change_dir(Direction::NORTH);
    temp.update();
    if (grid_->is_inside(temp))
      Logger::log_line("NORTH char: ", grid_->get(temp));
    if (grid_->is_inside(temp) && grid_->get(temp) == CH_NORTH) {
      if (!create_runner) {
	position_.change_dir(Direction::NORTH);
	create_runner = true;
      }
      else
	new_runners_list_->push({temp, st_});
    }
    temp = curr_position;
    temp.change_dir(Direction::SOUTH);
    temp.update();
    if (grid_->is_inside(temp))
      Logger::log_line("SOUTH char: ", grid_->get(temp));
    if (grid_->is_inside(temp) && grid_->get(temp) == CH_SOUTH) {
      if (!create_runner) {
	position_.change_dir(Direction::SOUTH);
	create_runner = true;
      }
      else
	new_runners_list_->push({temp, st_});
    }
    temp = curr_position;
    temp.change_dir(Direction::WEST);
    temp.update();
    if (grid_->is_inside(temp))
      Logger::log_line("WEST char: ", grid_->get(temp));
    if (grid_->is_inside(temp) && grid_->get(temp) == CH_WEST) {
      if (!create_runner) {
	position_.change_dir(Direction::WEST);
	create_runner = true;
      }
      else
	new_runners_list_->push({temp, st_});
    }
    Logger::log_line("Finished split processing");
    return true;
  }

  template<typename T>
  std::string Runner::get_string_from_vars(T val) {
    std::stringstream os;
    os << val;
    return os.str();
  }
  template<typename T, typename... Types>
  std::string Runner::get_string_from_vars(T val, Types... vars) {
    std::stringstream os;
    os << val << get_string_from_vars(vars...);
    return os.str();
  }
  template<typename... Types>
  void Runner::set_error(Types... vars) {
    err_str_ = get_string_from_vars(vars...);
  }

  bool is_mode_end_char(Mode mode, char current_char) {
    if (mode_end_chars[mode].find(current_char) != std::string::npos)
      return true;
    return false;
  }

}
