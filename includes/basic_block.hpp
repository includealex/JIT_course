#ifndef INCLUDES_BASIC_BLOCK_HPP_
#define INCLUDES_BASIC_BLOCK_HPP_

#include <cstddef>
#include <iostream>
#include <vector>

#include "graph.hpp"
#include "instruction.hpp"

namespace custom {

class Instruction;
class Graph;

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

struct LiveRange final {
 public:
  LiveRange() {
    _start = -1;
    _end = -1;
  }
  LiveRange(std::size_t st, std::size_t ed) : _start(st), _end(ed) {}

  std::size_t get_start() {
    return _start;
  }
  std::size_t get_end() {
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

 private:
  std::size_t _start;
  std::size_t _end;
};

class BasicBlock final {
 public:
  BasicBlock(Graph* graph)
      : _first_Phi(nullptr), _first_inst(nullptr), _last_inst(nullptr), _graph(graph) {}

  ~BasicBlock();
  void pushback_instr(Instruction* inst);
  Instruction* getInstruction(size_t index) const;
  bool isPhiInstruction(Instruction* inst);

  std::vector<BasicBlock*> get_preds();
  BasicBlock* get_succs_true();
  BasicBlock* get_succs_false();

  Instruction* get_first_Phi();
  Instruction* get_first_inst();
  Instruction* get_last_inst();
  std::size_t instructions_amount() const;

  Graph* get_graph();

  void set_graph(Graph* other);
  void push_preds_back(BasicBlock* el);
  void add_succs_false(BasicBlock* el);
  void add_succs_true(BasicBlock* el);

  void set_id(std::size_t id);
  std::size_t get_id();
  bool is_root();

  void set_dfs_marker(bool value);
  void set_rpo_marker(bool value);
  void set_loop_gray_marker(bool value);
  void set_loop_black_marker(bool value);
  bool is_dfs_marker();
  bool is_rpo_marker();
  bool is_loop_gray_marker();
  bool is_loop_black_marker();

  void set_liverange_start(std::size_t val);
  void set_liverange_end(std::size_t val);
  void set_liverange(LiveRange lr);
  void append_liverange(LiveRange lr);
  void append_liverange(std::size_t st, std::size_t ed);
  std::size_t get_liverange_start();
  std::size_t get_liverange_end();
  LiveRange get_liverange();

 private:
  std::vector<BasicBlock*> _preds;  // many!

  BasicBlock* _succs_true = nullptr;   // right
  BasicBlock* _succs_false = nullptr;  // left

  Instruction* _first_Phi = nullptr;
  Instruction* _first_inst = nullptr;
  Instruction* _last_inst = nullptr;

  Graph* _graph = nullptr;

  BasicBlockMarker _markers;

  std::size_t _basic_block_id = -1;

  LiveRange _live_range;
};

}  // namespace custom

#endif  // INCLUDES_BASIC_BLOCK_HPP_
