#ifndef INCLUDES_LIVEINTERVAL_HPP_
#define INCLUDES_LIVEINTERVAL_HPP_

#include "liverange.hpp"

namespace custom {

struct LiveInterval final {
 public:
  LiveInterval() = default;

  LiveInterval(std::initializer_list<LiveInterval> intervals) {
    for (const auto& interval : intervals) {
      for (const auto& [lin, lr] : interval._live) {
        add(lin, lr);
      }
    }
  }

  void add(std::size_t lin, LiveRange lr) {
    if (_live.count(lin)) {
      _live[lin].append(lr);
      return;
    }
    _live[lin] = lr;
  }

  void setFrom(std::size_t lin, std::size_t start) {
    if (_live.count(lin)) {
      _live[lin].set_start(start);
      return;
    }
    _live[lin] = LiveRange(start, start + 2);
  }

  void add_empty(std::size_t lin, std::size_t livenum) {
    _live[lin] = LiveRange(livenum, livenum + 2);
  }

  void remove(std::size_t lin) {
    _live.erase(lin);
  }

  void set_liveIn(std::map<std::size_t, LiveRange> other) {
    _live = other;
  }

  std::map<std::size_t, LiveRange> get_liveIn() {
    return _live;
  }

  void print() const {
    std::cout << "LiveInterval:\n";
    for (const auto& [lin, range] : _live) {
      std::cout << "  Line " << lin << ": ";
      range.print();
      std::cout << "\n";
    }
  }

  bool operator==(const LiveInterval& other) const {
    return _live == other._live;
  }

 private:
  std::map<std::size_t, LiveRange> _live;
};

}  // namespace custom

#endif  // INCLUDES_LIVEINTERVAL_HPP_