#include "rpo.hpp"

namespace custom {

void RPO::run_rpo(BasicBlock* block, BasicBlock* excluded_block) {
  _rpo_ids.clear();
  int id_to_excl = (excluded_block != nullptr) ? excluded_block->get_id() : -1;
  get_rpo_ids(block, id_to_excl);
  clear_all_markers(block);
}

void RPO::get_rpo_ids(BasicBlock* block, std::size_t exclude_id) {
  if (block == nullptr) {
    return;
  }

  std::stack<BasicBlock*> stack;
  std::unordered_set<BasicBlock*> visited;
  std::vector<std::size_t> postorder;

  stack.push(block);

  while (!stack.empty()) {
    BasicBlock* curr = stack.top();

    if (visited.find(curr) == visited.end()) {
      visited.insert(curr);

      if (curr->get_succs_true() && visited.find(curr->get_succs_true()) == visited.end() &&
          curr->get_succs_true()->get_id() != exclude_id) {
        stack.push(curr->get_succs_true());
      }

      if (curr->get_succs_false() && visited.find(curr->get_succs_false()) == visited.end() &&
          curr->get_succs_false()->get_id() != exclude_id) {
        stack.push(curr->get_succs_false());
      }
    } else {
      stack.pop();
      postorder.push_back(curr->get_id());
    }
  }

  _rpo_ids.assign(postorder.rbegin(), postorder.rend());
}

void RPO::clear_all_markers(BasicBlock* block) {
  clear_rpo_markers(block);
}

void RPO::clear_rpo_markers(BasicBlock* block) {
  if (block == nullptr) {
    return;
  }

  std::stack<BasicBlock*> stack;
  stack.push(block);

  while (!stack.empty()) {
    BasicBlock* curr = stack.top();
    stack.pop();

    curr->set_rpo_marker(false);

    for (BasicBlock* succ : {curr->get_succs_true(), curr->get_succs_false()}) {
      if (succ != nullptr && succ->is_rpo_marker()) {
        stack.push(succ);
      }
    }
  }
}

void RPO::run_reversed_rpo(BasicBlock* start, BasicBlock* end) {
  _rpo_ids.clear();
  std::unordered_set<BasicBlock*> visited;
  std::vector<BasicBlock*> post_order;

  reversed_dfs(end, visited, post_order);

  for (auto it = post_order.rbegin(); it != post_order.rend(); ++it) {
    _rpo_ids.push_back((*it)->get_id());
    if (*it == start)
      break;
  }
}

void RPO::reversed_dfs(BasicBlock* block,
                       std::unordered_set<BasicBlock*>& visited,
                       std::vector<BasicBlock*>& post_order) {
  if (block == nullptr || visited.find(block) != visited.end()) {
    return;
  }

  visited.insert(block);

  for (BasicBlock* pred : block->get_preds()) {
    reversed_dfs(pred, visited, post_order);
  }

  post_order.push_back(block);
}

std::vector<std::size_t> RPO::get_rpo_ids_arr() {
  return _rpo_ids;
}

}  // namespace custom
