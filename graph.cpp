#include "graph.hpp"
#include "basic_block.hpp"

namespace custom {

void Graph::addBasicBlock(BasicBlock* block) {
    block->set_graph(this);
    _blocks.push_back(block);
}

BasicBlock* Graph::get_block(std::size_t idx) {
    return _blocks[idx];
}

} // namespace custom

