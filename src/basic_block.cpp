#include "basic_block.hpp"

namespace custom {

void BasicBlock::pushback_instr(Instruction* inst) {
  inst->set_basic_block(this);

  if (_last_inst) {
    auto prev_id = _last_inst->get_id();
    _last_inst->set_next(inst);
    inst->set_prev(_last_inst);
    inst->set_id(prev_id + 1);
    _last_inst = inst;
  } else {
    _first_inst = inst;
    _last_inst = inst;
    inst->set_prev(nullptr);
    size_t begin_id = 0;
    inst->set_id(begin_id);
  }

  inst->set_next(nullptr);
}

std::size_t BasicBlock::instructions_amount() const {
  return _last_inst->get_id() + 1;
}

Instruction* BasicBlock::getInstruction(size_t index) const {
  auto cur_sz = instructions_amount();
  if (index >= cur_sz) {
    std::cout << "trying to get non-existant instruction" << std::endl;
    return nullptr;
  }

  Instruction* cur = _first_inst;
  for (size_t i = 0; i < index; ++i) {
    cur = cur->get_next();
  }

  return cur;
}

bool BasicBlock::isPhiInstruction(Instruction* inst) {
  return inst->getOpcode() == Opcode::PHI;
}

std::vector<BasicBlock*> BasicBlock::get_preds() {
  return _preds;
}

BasicBlock* BasicBlock::get_succs_true() {
  return _succs_true;
}

BasicBlock* BasicBlock::get_succs_false() {
  return _succs_false;
}

Instruction* BasicBlock::get_first_Phi() {
  return _first_Phi;
}

Instruction* BasicBlock::get_first_inst() {
  return _first_inst;
}

Instruction* BasicBlock::get_last_inst() {
  return _last_inst;
}

Graph* BasicBlock::get_graph() {
  return _graph;
}

void BasicBlock::set_graph(Graph* other) {
  _graph = other;
}

void BasicBlock::push_preds_back(BasicBlock* el) {
  if (!is_root()) {
    _preds.push_back(el);
  }
}

void BasicBlock::add_succs_false(BasicBlock* el) {
  _succs_false = el;
  el->push_preds_back(this);
}

void BasicBlock::add_succs_true(BasicBlock* el) {
  _succs_true = el;
  el->push_preds_back(this);
}

void BasicBlock::set_id(std::size_t id) {
  _basic_block_id = id;
}

std::size_t BasicBlock::get_id() {
  return _basic_block_id;
}

bool BasicBlock::is_root() {
  return (_basic_block_id == 0);
}

void BasicBlock::set_dfs_marker(bool value) {
  _markers.set_dfs_m(value);
}

void BasicBlock::set_rpo_marker(bool value) {
  _markers.set_rpo_m(value);
}

bool BasicBlock::is_dfs_marker() {
  return _markers.is_dfs_m();
}

bool BasicBlock::is_rpo_marker() {
  return _markers.is_rpo_m();
}

void BasicBlock::set_loop_gray_marker(bool value) {
  _markers.set_loop_gray_m(value);
}

bool BasicBlock::is_loop_gray_marker() {
  return _markers.is_loop_gray_m();
}

void BasicBlock::set_loop_black_marker(bool value) {
  _markers.set_loop_black_m(value);
}

bool BasicBlock::is_loop_black_marker() {
  return _markers.is_loop_black_m();
}

BasicBlock::~BasicBlock() {
  Instruction* current = _first_inst;
  while (current) {
    Instruction* next = current->get_next();
    delete current;
    current = next;
  }
}

void BasicBlock::set_liverange_start(std::size_t val) {
  _live_range.set_start(val);
}

void BasicBlock::set_liverange_end(std::size_t val) {
  _live_range.set_end(val);
}

void BasicBlock::set_liverange(LiveRange lr) {
  _live_range = lr;
}

std::size_t BasicBlock::get_liverange_start() {
  return _live_range.get_start();
}

std::size_t BasicBlock::get_liverange_end() {
  return _live_range.get_end();
}

LiveRange BasicBlock::get_liverange() {
  return _live_range;
}

void BasicBlock::append_liverange(LiveRange lr) {
  _live_range.append(lr);
}

void BasicBlock::append_liverange(std::size_t st, std::size_t ed) {
  _live_range.append(st, ed);
}

LiveInterval BasicBlock::get_liveIn() {
  return _liveIn;
}

void BasicBlock::set_liveIn(LiveInterval liveIn) {
  _liveIn = liveIn;
}

}  // namespace custom
