#ifndef INCLUDES_BASIC_BLOCK_MARKER_HPP_
#define INCLUDES_BASIC_BLOCK_MARKER_HPP_

namespace custom {

struct BasicBlockMarker final {
 private:
  bool _dfs_marker = false;
  bool _loop_gray_marker = false;
  bool _loop_black_marker = false;
  bool _rpo_marker = false;

 public:
  void set_dfs_m(bool val) {
    _dfs_marker = val;
  }
  void set_rpo_m(bool val) {
    _rpo_marker = val;
  }
  void set_loop_gray_m(bool val) {
    _loop_gray_marker = val;
  }
  void set_loop_black_m(bool val) {
    _loop_black_marker = val;
  }

  bool is_dfs_m() {
    return _dfs_marker;
  }
  bool is_rpo_m() {
    return _rpo_marker;
  }
  bool is_loop_gray_m() {
    return _loop_gray_marker;
  }
  bool is_loop_black_m() {
    return _loop_black_marker;
  }
};

}  // namespace custom

#endif  // INCLUDES_BASIC_BLOCK_MARKER_HPP_