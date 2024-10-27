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

BasicBlock* Graph::get_block(std::size_t idx, BasicBlock* start_block) {
    if (start_block == nullptr) {
        start_block = _root;
    }

    std::stack<BasicBlock*> stack;
    std::set<size_t> tmp_ids;
    stack.push(start_block);
    tmp_ids.insert(start_block->get_id());

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        if (block->get_id() == idx) {
            return block;
        }

        tmp_ids.insert(block->get_id());

        BasicBlock* false_block = block->get_succs_false();
        if (false_block!=nullptr && (!tmp_ids.count(false_block->get_id()))) {
            stack.push(false_block);
        }

        BasicBlock* true_block = block->get_succs_true();\
        if (true_block!=nullptr && (!tmp_ids.count(true_block->get_id()))) {
            stack.push(true_block);
        }
    }

    return nullptr;
}

std::size_t Graph::basic_blocks_num() {
    return _blocks_size;
}

} // namespace custom

