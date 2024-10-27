#include "dfs.hpp"

namespace custom {

void DFS::run_dfs(BasicBlock* block, BasicBlock* excluded_block) {
    int id_to_excl = (excluded_block != nullptr) ? excluded_block->get_id() : -1;
    get_dfs_ids(block, id_to_excl);
    clear_markers(block);
}

void DFS::get_dfs_ids(BasicBlock* start_block, std::size_t exclude_id) {
    if (start_block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(start_block);

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        if (block == nullptr || block->get_id() == exclude_id || block->is_dfs_marker()) {
            continue;
        }

        _dfs_ids.push_back(block->get_id());
        block->set_dfs_marker(true);

        BasicBlock* right = block->get_succs_true();
        if (right != nullptr) {
            stack.push(right);
        }
        BasicBlock* left = block->get_succs_false();
        if (left != nullptr) {
            stack.push(left);
        }
    }
}

void DFS::clear_markers(BasicBlock* start_block) {
    if (start_block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(start_block);

    while (!stack.empty()) {
        BasicBlock* block = stack.top();
        stack.pop();

        block->set_dfs_marker(false);

        BasicBlock* right = block->get_succs_true();
        if (right != nullptr && right->is_dfs_marker()) {
            stack.push(right);
        }

        BasicBlock* left = block->get_succs_false();
        if (left != nullptr && left->is_dfs_marker()) {
            stack.push(left);
        }
    }
}

std::vector<std::size_t> DFS::get_dfs_ids_arr() {
    return _dfs_ids;
}

} // namespace custom
