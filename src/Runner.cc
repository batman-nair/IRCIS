#include <Runner.h>

#include <cctype>
#include <sstream>
#include <algorithm>
#include <random>

namespace Ircis {
  void Runner::run_debug() {
    log_line("Runner ", id_, " died");
    log_line("Processed chars: ", processed_chars_);
    err_line(err_str_);
    std::ostringstream os;
    while (!st_.empty()) {
      os << st_.top().to_string() << ", ";
      st_.pop();
    }
    log_line("Stack values: ", os.str());
  }

  // Step function processes the current char for Runner and moves it a step
  // Returns false if Runner is dead
  bool Runner::step() {
    path_.push_back(position_);
    if (pause_time_) {
      log_line("Pausing. Pause time ", pause_time_);
      return pause_time_--;
    }

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
	  log_line("Condition check true");
	  break;
	}
	log_line("Condition check false");
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
    case CH_RAND_INT:
      {
	if (st_.empty()) {
	  set_error("Empty stack when looking for random limit");
	  return false;
	}
	Data dat = st_.top();
	if (!dat.is_integer or dat.value <= 0) {
	  set_error("Random limit must be an integer greater than 0");
	  return false;
	}
	st_.pop();
	push_random_number_to_stack(dat.value);
	break;
      }
    case CH_RAND:
      push_random_number_to_stack(1);
      break;
    case CH_PAUSE:
      {
	if (st_.empty()) {
	  set_error("Empty stack when looking for pause time");
	  return false;
	}
	Data dat = st_.top();
	if (!dat.is_integer or dat.value < 0) {
	  set_error("Pause time must be an integer greater than or equal to 0");
	  return false;
	}
	st_.pop();
	pause_time_ = dat.value;
	break;
      }
    case CH_END:
      set_error("End character reached");
      return false;
    case CH_PRINT:
      {
        if (st_.empty()) {
          log_->print_line();
          set_error("Stack is empty, forcing exit.");
          return false;
        }
        auto top = st_.top();
        log_line("Printing and popping: ", st_.top());
        st_.pop();
        log_->print(top.to_string());
        break;
      }
    case CH_PRINT_BASE64:
      {
        if (st_.empty()) {
          log_->print_line();
          set_error("Stack is empty, forcing exit.");
          return false;
        }
        auto top = st_.top();
        log_line("Printing and popping as Base 64: ", st_.top());
        st_.pop();
        if (top.is_integer) {
          log_->print(base64_encode_int(top.value));
        }
        else {
          log_->print(top.to_string());
        }
        break;
      }
    default:
      if (!is_blank(current_char)) {
	// Character could not be processed
	err_line("Character could not be processed: ", static_cast<char>(current_char));
      }
    }
    return true;
  }

  // if the first and second character of the buffer are both 
  // valid base64 characters, don't treat it as arith
  // (even if the first chars are / or +)
  bool is_not_arith(char first_char, char second_char) {
    return (!is_arith(first_char)) || (isbase64(first_char) && isbase64(second_char));
  }

  bool Runner::process_integer_buffer() {
    auto it = integer_mode_buffer_.begin();
    ++it;			// Skip starting quote char
    char start_ch = *it;
    ++it;
    char second_ch = *it;

    if (isbase64(start_ch) && is_not_arith(start_ch, second_ch)) {	// Integer processing
      //int num = start_ch - '0';
      std::string buffer;
      buffer.push_back(start_ch);
      if (!isdigit(start_ch) && isbase64(start_ch)) {
        base64_mode = true;
      }
      while (it != integer_mode_buffer_.end()) {
        char ch = *it++;
        if (!isdigit(ch) && isbase64(ch)) {
          base64_mode = true;
        }
        if (!isbase64(ch)) {
          set_error("Non integer character in integer processing.");
          return false;
        }
        buffer.push_back(ch);
      }
      int num;
      if (base64_mode) {
        num = base64_decode_int(buffer);
      }
      else {
        num = std::atoi(buffer.c_str());
      }
      Data dat(num, true);
      log_line("Pushing value to stack ", dat);
      st_.push(dat);
    }
    else {
      if (is_arith(start_ch)) {
        if (st_.size() < 2) {
          set_error("Not enough elements for arithmetic operation");
          return false;
        }
        Data num1 = st_.top();
        log_line("Stack value popped ", st_.top());
        st_.pop();
        Data num2 = st_.top();
        log_line("Stack value popped ", st_.top());
        st_.pop();
        if (num1.is_integer && num2.is_integer) {
          switch(start_ch) {
            case CH_ADD:
              log_line("Arith: ", num1, " + ", num2);
              num1 = num1 + num2;
              break;
            case CH_SUB:
              log_line("Arith: ", num1, " - ", num2);
              num1 = num1 - num2;
              break;
            case CH_MUL:
              log_line("Arith: ", num1, " * ", num2);
              num1 = num1 * num2;
              break;
            case CH_DIV:
              if (num2.value == 0) {
                set_error("Division by zero error");
                return false;
              }
              log_line("Arith: ", num1, " / ", num2);
              num1 = num1 / num2;
              break;
            case CH_MOD:
              log_line("Arith: ", num1, " % ", num2);
              num1 = num1 % num2;
              break;
            case CH_POW:
              log_line("Arith: ", num1, " ^ ", num2);
              num1 = num1 ^ num2;
              break;
            case CH_AND:
              log_line("Arith: ", num1, " & ", num2);
              num1 = num1 & num2;
              break;
            case CH_OR:
              log_line("Arith: ", num1, " | ", num2);
              num1 = num1 | num2;
              break;
            case CH_XOR:
              log_line("Arith: ", num1, " V ", num2);
              num1 = num1.V(num2);
              break;
            case CH_BL:
              log_line("Arith: ", num1, " < ", num2);
              num1 = num1 < num2;
              break;
            case CH_BR:
              log_line("Arith: ", num1, " > ", num2);
              num1 = num1 > num2;
              break;
            default:
              set_error("Unknown arithmetic op found: ", start_ch);
              return false;
          };
          log_line("Pushing value to stack ", num1);
          st_.push(num1);
        }
      }
    }

    integer_mode_ = false;
    base64_mode = false;
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
	  log_line("Pushing value to stack ", data);
	  st_.push(data);
	}
	break;
      }
    case STACK_PUSH:
      {
	if (islower(mode_buffer_[0])) {
	  if (!process_local_var_fetch())
	    return false;
	}
	else if (isupper(mode_buffer_[0])) {
	  if (!process_global_var_fetch())
	    return false;
	}
	else {
	  if (!process_stack_push())
	    return false;
	}
	break;
      }
    case STACK_POP:
      {
	if (islower(mode_buffer_[0])) {
	  if (!process_local_var_insert())
	    return false;
	}
	else if (isupper(mode_buffer_[0])) {
	  if (!process_global_var_insert())
	    return false;
	}
	else {
	  if (!process_stack_pop())
	    return false;
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

  constexpr char get_direction_char(Direction dir) {
    switch(dir) {
    case Direction::NORTH: return CH_NORTH;
    case Direction::EAST: return CH_EAST;
    case Direction::WEST: return CH_WEST;
    case Direction::SOUTH: return CH_SOUTH;
    }
    Logger::err_line("Got invalid Direction!");
    return CH_NORTH;
  }

  bool Runner::process_split() {
    DirVec curr_position = position_;
    bool create_runner = false;
    auto process_split_for_direction =
      [this, &create_runner, &curr_position] (Direction dir) {
	DirVec temp = curr_position;
	temp.move(dir);
	if (grid_->is_inside(temp) && grid_->get(temp) == get_direction_char(dir)) {
	  if (!create_runner) {
	    position_.change_dir(dir);
	    create_runner = true;
	  }
	  else {
	    new_runners_list_->push({temp, st_, var_map_, path_});
	  }
	}
      };

    for (Direction dir: {Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST}) {
      // log_line("Processing split for Direction: ", dir);
      process_split_for_direction(dir);
    }
    return true;
  }

  bool Runner::process_stack_push() {
    int num = 0;
    for (char ch: mode_buffer_) {
      if (!isdigit(ch)) {
	set_error("Invalid character in Stack Push mode!");
	return false;
      }
      num = num*10 + (ch-'0');
    }
    Data temp = st_[-num];
    log_line("Pushing value to stack ", temp);
    st_.push(temp);
    return true;
  }

  bool Runner::process_stack_pop() {
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
	log_line("Stack pop preemptive finish");
	return true;
      }
      log_line("Stack value popped ", st_.top());
      st_.pop();
    }
    return true;
  }

  bool Runner::process_global_var_fetch() {
    std::string var(mode_buffer_);
    if (std::find_if_not(var.begin(), var.end(), isalpha) != var.end()) { // Variable names only alphabets
      set_error("Variable name '", var, "' should contain only alphabets");
      return false;
    }
    if (global_var_map_->find(var) == global_var_map_->end()) {
      set_error("Couldn't find global variable ", var);
      return false;
    }
    Data temp = (*global_var_map_)[var];
    log_line("Pushing variable(", var, ") value to stack ", temp);
    st_.push(temp);
    return true;
  }

  bool Runner::process_global_var_insert() {
    std::string var(mode_buffer_);
    if (std::find_if_not(var.begin(), var.end(), isalpha) != var.end()) { // Variable names only alphabets
      set_error("Variable name '", var, "' should contain only alphabets");
      return false;
    }
    log_line("Saving value ", st_.top(), " to variable ", var);
    (*global_var_map_)[var] = st_.top();
    return true;
  }

  bool Runner::process_local_var_fetch() {
    std::string var(mode_buffer_);
    if (std::find_if_not(var.begin(), var.end(), isalpha) != var.end()) { // Variable names only alphabets
      set_error("Variable name '", var, "' should contain only alphabets");
      return false;
    }
    if (var_map_.find(var) == var_map_.end()) {
      set_error("Couldn't find local variable ", var);
      return false;
    }
    Data temp = var_map_[var];
    log_line("Pushing variable(", var, ") value to stack ", temp);
    st_.push(temp);
    return true;
  }

  bool Runner::process_local_var_insert() {
    std::string var(mode_buffer_);
    if (std::find_if_not(var.begin(), var.end(), isalpha) != var.end()) { // Variable names only alphabets
      set_error("Variable name '", var, "' should contain only alphabets");
      return false;
    }
    log_line("Saving value ", st_.top(), " to variable ", var);
    var_map_[var] = st_.top();
    return true;
  }

  void Runner::push_random_number_to_stack(int limit) {
    static std::random_device rd;
    static std::default_random_engine re(rd());
    std::uniform_int_distribution<int> dis(0, limit);

    Data dat(dis(re), true);
    log_line("Pushing random number to stack ", dat);
    st_.push(dat);
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

  bool isbase64(char current_char) {
    return base64_chars.find(current_char) != std::string::npos;
  }

  int base64_decode_int(std::string input) {
    // Remove leading As
    std::string str = input.erase(0, fmin(input.find_first_not_of('A'), input.size()-1));
    // Return 0 on overflow
    if ( str.empty() || str.size() > 5 ) return  0;
    int pos_val = 1; // positional value
    int result = 0;
    for( auto strpos = str.rbegin(); strpos < str.rend(); strpos++){
      size_t digit_b10 = base64_chars.find(*strpos);
      if ( digit_b10 == base64_chars.npos ) return 0;
      else {
        result += pos_val*static_cast<int>(digit_b10);
        pos_val *= 64;
      }
    }
    return result;
  }
  
  std::string base64_encode_int(int value) {
    std::string result;
    result.push_back(base64_chars.at((value & 1056964608) >> 24));
    result.push_back(base64_chars.at((value & 16515072) >> 18));
    result.push_back(base64_chars.at((value & 258048) >> 12));
    result.push_back(base64_chars.at((value & 4032) >> 6));
    result.push_back(base64_chars.at((value & 63)));
    return result.erase(0, fmin(result.find_first_not_of('A'), result.size()-1));
  }

}
