#ifndef INCLUDES_LIVERANGE_HPP_
#define INCLUDES_LIVERANGE_HPP_

namespace custom {

struct LiveRange final {
 public:
  LiveRange() {
    _start = 666;
    _end = 666;
  }
  LiveRange(std::size_t st, std::size_t ed) : _start(st), _end(ed) {}

  std::size_t get_start() const {
    return _start;
  }
  std::size_t get_end() const {
    return _end;
  }
  void set_start(std::size_t st) {
    _start = st;
  }
  void set_end(std::size_t ed) {
    _end = ed;
  }

  void append(LiveRange other) {
    _start = std::min(_start, other.get_start());
    _end = std::max(_end, other.get_end());
  }

  void append(std::size_t st, std::size_t ed) {
    _start = std::min(_start, st);
    _end = std::max(_end, ed);
  }

  bool overlaps(const LiveRange& other) const {
    return !(_end < other._start || other._end < _start);
  }

  void print() const {
    std::cout << "[" << _start << ", " << _end << "]";
  }

  bool operator==(const LiveRange& other) const {
    return _start == other._start && _end == other._end;
  }

 private:
  std::size_t _start;
  std::size_t _end;
};

}  // namespace custom

#endif  // INCLUDES_LIVERANGE_HPP_