#include "dfs.hpp"

namespace custom {

void DFS::run_dfs(BasicBlock* block, BasicBlock* excluded_block) {
    _dfs_ids.clear();
    int id_to_excl = (excluded_block != nullptr) ? excluded_block->get_id() : -1;
    get_dfs_ids(block, id_to_excl);
    clear_all_markers(block);
}

void DFS::get_dfs_ids(BasicBlock* block, std::size_t exclude_id) {
    if (block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(block);

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        if (block == nullptr || block->get_id() == exclude_id ) {
            continue;
        }

        if(block->is_loop_gray_marker()) {
            std::cout << "Black marker setting for id: " << block->get_id() << std::endl;
            block->set_loop_black_marker(true);
        }

        if (block->is_dfs_marker()) {
            continue;
        }

        _dfs_ids.push_back(block->get_id());
        block->set_dfs_marker(true);

        block->set_loop_gray_marker(true);

        for (BasicBlock* succ : {block->get_succs_true(), block->get_succs_false()}) {
            if (succ != nullptr) {
                if (succ->is_loop_gray_marker()) {
                    std::cout << "latch is block with id: " << block->get_id() << std::endl;
                }
            stack.push(succ);
            }
        }
    }
}

void DFS::clear_dfs_markers(BasicBlock* block) {
    if (block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(block);

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        block->set_dfs_marker(false);

        for (BasicBlock* succ : {block->get_succs_true(), block->get_succs_false()}) {
            if (succ != nullptr && (succ->is_dfs_marker())) {
                stack.push(succ);
            }
        }
    }
}

void DFS::clear_loop_gray_black_markers(BasicBlock* block) {
    if (block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(block);

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        block->set_loop_gray_marker(false);
        block->set_loop_black_marker(false);

        for (BasicBlock* succ : {block->get_succs_true(), block->get_succs_false()}) {
            if (succ != nullptr && (succ->is_loop_gray_marker())) {
                stack.push(succ);
            }
        }    
    }
}

void DFS::clear_all_markers(BasicBlock* block) {
    clear_dfs_markers(block);
    clear_loop_gray_black_markers(block);
}

std::vector<std::size_t> DFS::get_dfs_ids_arr() {
    return _dfs_ids;
}

} // namespace custom
