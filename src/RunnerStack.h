#pragma once

#include <DataType.h>

#include <vector>

namespace PTrain {
  // Custom stack implementation with relative indexing
  class RunnerStack {
  public:
    Data& operator[](int index) {
      if (index > 0)
	return container_[index];
      else {
	index -= 1;
	return container_.end()[index];
      }
    }

    // Stack functions
    void push(const Data data) { container_.push_back(data); }
    const Data& top() const { return container_.back(); }
    Data& top() { return container_.back(); }
    size_t size() { return container_.size(); }
    void pop() { container_.pop_back(); }
    bool empty() { return container_.empty(); }
  private:
    std::vector<Data> container_;
  };
}
