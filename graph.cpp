#include "graph.hpp"
#include "basic_block.hpp"

namespace custom {

void Graph::addBasicBlock(BasicBlock* block) {
    block->set_graph(this);
    if (_blocks.empty()) {
        block->set_id(0);
    }
    else {
        block->set_id(_blocks.size());
    }
    _blocks.push_back(block);
}

BasicBlock* Graph::get_block(std::size_t idx) {
    return _blocks[idx];
}

} // namespace custom

