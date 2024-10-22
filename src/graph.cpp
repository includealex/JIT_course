#include "graph.hpp"
#include "basic_block.hpp"

namespace custom {

BasicBlock* Graph::_root = nullptr;

void Graph::addBasicBlock(BasicBlock* block) {
    block->set_graph(this);
    if (basic_blocks_num() == 0) {
        _root = block;
        block->set_id(0);
    }
    else {
        block->set_id(basic_blocks_num());
    }

    _blocks_size++;
}

BasicBlock* Graph::get_block(std::size_t idx, BasicBlock* block) {
    if (block == nullptr) {
        block = _root;
    }

    if (idx == block->get_id()) {
        return block;
    }

    BasicBlock* false_block = block->get_succs_false();
    if ((false_block != nullptr) && (false_block->get_id() > block->get_id()) ){
        return(get_block(idx, false_block));
    }

    BasicBlock* true_block = block->get_succs_true();
    if ((true_block != nullptr) && (true_block->get_id() > block->get_id())) {
        return(get_block(idx, true_block));
    }

    return nullptr;
}

std::size_t Graph::basic_blocks_num() {
    return _blocks_size;
}

} // namespace custom

