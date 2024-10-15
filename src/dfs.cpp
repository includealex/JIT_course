#include "dfs.hpp"

namespace custom {

void DFS::run_dfs(BasicBlock* block) {
    get_dfs_ids(block);
    clear_markers(block);
}

void DFS::run_dfs_excluded_block(BasicBlock* block, BasicBlock* excluded_block) {
    auto id_to_excl = excluded_block->get_id();
    get_dfs_ids(block, id_to_excl);
    clear_markers(block);
    return;
}

void DFS::get_dfs_ids(BasicBlock* block, std::size_t exclude_id) {
    if(block == nullptr) {
        return;
    }

    if(block->get_id() == exclude_id) {
        return;
    }

    if(!(block->is_dfs_marker())) {
        _dfs_ids.push_back(block->get_id());
        block->set_dfs_marker(true);
    }

    auto right = block->get_succs_true();
    if ((right != nullptr) && (!right->is_dfs_marker()) && (right->get_id() != exclude_id)) {
        get_dfs_ids(right);
    }

    auto left = block->get_succs_false();
    if ((left != nullptr) && (!left->is_dfs_marker()) && (left->get_id() != exclude_id)) {
        get_dfs_ids(left);
    }
}

void DFS::clear_markers(BasicBlock* block) {
    if(block == nullptr) {
        return;
    }
    block->set_dfs_marker(false);
    auto right = block->get_succs_true();
    if ((right != nullptr) && (right->is_dfs_marker())) {
        clear_markers(right);
    }
    auto left = block->get_succs_false();
    if ((left != nullptr) && (left->is_dfs_marker())) {
        clear_markers(left);
    }
}

std::vector<std::size_t> DFS::get_dfs_ids_arr() {
    return _dfs_ids;
}

} // namespace custom
