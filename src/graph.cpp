#include "graph.hpp"

namespace custom {

BasicBlock* Graph::_root = nullptr;

Graph::~Graph() {
  for (BasicBlock* block : _blocks) {
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

}  // namespace custom
