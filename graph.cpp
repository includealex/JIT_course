#include "graph.hpp"
#include "basic_block.hpp"

namespace custom {

void Graph::addBasicBlock(BasicBlock* block) {
    block->_graph = this;
    _blocks.push_back(block);
}

} // namespace custom

