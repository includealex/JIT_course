#include "dfs.hpp"

namespace custom {

void DFS::run_dfs(BasicBlock* block, BasicBlock* excluded_block) {
  _dfs_ids.clear();
  int id_to_excl = (excluded_block != nullptr) ? excluded_block->get_id() : -1;
  get_dfs_ids(block, id_to_excl);
  clear_all_markers(block);
}

void DFS::add_loop(BasicBlock* header, BasicBlock* latch) {
  bool cond = true;

  for (auto& loop : _loops) {
    if (loop.get_header() == header) {
      cond = false;
      loop.add_latch(latch);
    }
  }

  if (cond) {
    Loop loop(header);
    loop.add_latch(latch);
    _loops.push_back(loop);
  }
}

const std::vector<Loop>& DFS::get_loops() const {
  return _loops;
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

    if (block == nullptr || block->get_id() == exclude_id) {
      continue;
    }

    if (block->is_loop_gray_marker()) {
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
          add_loop(succ, block);
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

std::vector<BasicBlock*> DFS::get_basic_blocks_between(BasicBlock* start, BasicBlock* end) {
  std::vector<BasicBlock*> path;
  std::unordered_set<BasicBlock*> visited;
  bool found = false;

  dfs_helper(start, end, path, visited, found);

  if (found) {
    return path;
  } else {
    return {};
  }
}

void DFS::dfs_helper(BasicBlock* block,
                     BasicBlock* end,
                     std::vector<BasicBlock*>& path,
                     std::unordered_set<BasicBlock*>& visited,
                     bool& found) {
  if (block == nullptr || found || visited.find(block) != visited.end()) {
    return;
  }

  visited.insert(block);
  path.push_back(block);

  if (block == end) {
    found = true;
    return;
  }

  for (BasicBlock* succ : {block->get_succs_true(), block->get_succs_false()}) {
    dfs_helper(succ, end, path, visited, found);
    if (found)
      return;
  }

  path.pop_back();
}

}  // namespace custom
