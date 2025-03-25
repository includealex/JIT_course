#include "graph.hpp"

namespace custom {

Graph::~Graph() {
  for (BasicBlock* block : _blocks) {
    if (block == nullptr) {
      continue;
    }

    delete block;
  }
  _blocks.clear();
}

void Graph::addBasicBlock(BasicBlock* block) {
  block->set_graph(this);
  if (basic_blocks_num() == 0) {
    _root = block;
    block->set_id(0);
  } else {
    block->set_id(basic_blocks_num());
  }
  _blocks.push_back(block);
  _blocks_size++;
}

BasicBlock* Graph::get_block(size_t index) const {
  if (index < _blocks.size()) {
    return _blocks[index];
  }
  return nullptr;
}

BasicBlock* Graph::get_root() const {
  return get_block(0);
}

std::size_t Graph::basic_blocks_num() const {
  return _blocks_size;
}

BasicBlock* Graph::split_BasicBlock(Instruction* instr, BasicBlock* cur_block) {
  auto cur_instr = cur_block->get_first_inst();

  if (cur_instr->get_next() == nullptr) {
    std::cerr << "Basic Block can not be split" << std::endl;
    return nullptr;
  }

  BasicBlock* new_block = new BasicBlock(this);

  while (cur_instr != instr) {
    cur_instr = cur_instr->get_next();

    if (cur_instr == nullptr) {
      std::cerr << "Invalid instruction provided" << std::endl;
      return nullptr;
    }
  }

  auto tmp_instr = cur_instr->get_next();
  while (tmp_instr != nullptr) {
    new_block->pushback_instr(tmp_instr);
    tmp_instr = tmp_instr->get_next();
  }

  cur_block->set_last_instr(cur_instr);
  cur_instr->set_next(nullptr);

  // FIXME:
  // What about successors?
  return new_block;
}

void Graph::add_return_block(BasicBlock* basicBlock) {
  _return_blocks.push_back(basicBlock);
}

void Graph::copy_graph() const {
  // TODO: implement for cases of inline and noinline
  return;
}

std::vector<BasicBlock*> Graph::get_ret_blocks() const {
  return _return_blocks;
}

std::vector<BasicBlock*> Graph::get_blocks() const {
  return _blocks;
}

}  // namespace custom
