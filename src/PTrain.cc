#include <PTrain.h>

namespace PTrain {
  bool PTrain::update() {
      bool keep_moving = false;
      for (auto it = runner_list_.begin(); it != runner_list_.end(); ++it) {
	if (it->step()) {
	  keep_moving = true;
	}
	else {
	  runner_list_.erase(it--); /* iter-- so the loop proceeds properly */
	}
      }

      while (!new_runners_list_->empty()) {
	keep_moving = true;
	runner_list_.emplace_back(new_runners_list_->front(), grid_, log_, new_runners_list_);
	new_runners_list_->pop();
      }

      if (!keep_moving) {
	log_->print_line(" ");
	DBG("PTrain has finished running!");
      }
      return keep_moving;
  }
}
