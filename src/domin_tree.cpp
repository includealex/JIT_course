#include "domin_tree.hpp"

namespace custom {

std::unordered_map<size_t, std::vector<size_t>> DominTree::get_premap(Graph* graph) {
  size_t root_idx = 0;
  auto root = graph->get_block(root_idx);
  DFS root_dfs;
  root_dfs.run_dfs(root);
  auto root_ids_arr = root_dfs.get_dfs_ids_arr();
  std::sort(root_ids_arr.begin(), root_ids_arr.end());

  std::unordered_map<size_t, std::vector<size_t>> premap;

  for (size_t idx = 0; idx < graph->basic_blocks_num(); ++idx) {
    auto el = graph->get_block(idx);
    DFS tmp_dfs;
    tmp_dfs.run_dfs(root, el);
    auto tmp_root_ids_arr = tmp_dfs.get_dfs_ids_arr();
    std::sort(tmp_root_ids_arr.begin(), tmp_root_ids_arr.end());

    std::vector<size_t> difference;
    std::set_difference(root_ids_arr.begin(),
                        root_ids_arr.end(),
                        tmp_root_ids_arr.begin(),
                        tmp_root_ids_arr.end(),
                        std::back_inserter(difference));
    auto it = std::find(difference.begin(), difference.end(), idx);
    if (it != difference.end()) {
      difference.erase(it);
    }

    premap.insert({idx, difference});
  }

  return premap;
}

void DominTree::build_tree(Graph* graph) {
  auto premap = get_premap(graph);

  std::unordered_map<size_t, std::vector<size_t>> aftermap;
  for (auto& kev : premap) {
    int id = kev.first;
    std::vector<size_t> start_near_succs = kev.second;

    std::set<size_t> far_succs;
    for (auto& elem : start_near_succs) {
      auto it = premap.find(elem);
      auto sz = it->second.size();
      for (size_t idx = 0; idx < sz; ++idx) {
        far_succs.insert(it->second[idx]);
      }
    }

    for (auto& elem : far_succs) {
      auto it = std::find(start_near_succs.begin(), start_near_succs.end(), elem);
      start_near_succs.erase(it);
    }

    aftermap.insert({id, start_near_succs});
  }

  _domin_tree_root = convert_to_tree(aftermap, 0);
}

DTNode* DominTree::prebuild_tree(size_t idx,
                                 std::unordered_map<size_t, DTNode*>& nodeMap,
                                 const std::unordered_map<size_t, std::vector<size_t>>& aftermap) {
  if (nodeMap.find(idx) != nodeMap.end()) {
    return nodeMap[idx];
  }

  DTNode* node = new DTNode(idx);
  nodeMap[idx] = node;

  if (aftermap.find(idx) != aftermap.end()) {
    for (size_t succIdx : aftermap.at(idx)) {
      DTNode* succNode = prebuild_tree(succIdx, nodeMap, aftermap);
      node->succs.push_back(succNode);
    }
  }

  return node;
}

DTNode* DominTree::convert_to_tree(const std::unordered_map<size_t, std::vector<size_t>>& aftermap,
                                   size_t rootIdx) {
  std::unordered_map<size_t, DTNode*> nodeMap;
  return prebuild_tree(rootIdx, nodeMap, aftermap);
}

DTNode* DominTree::find_node_by_index(size_t target_idx) {
  if (!_domin_tree_root) {
    return nullptr;
  }

  std::stack<DTNode*> node_stack;
  node_stack.push(_domin_tree_root);

  while (!node_stack.empty()) {
    DTNode* current_node = node_stack.top();
    node_stack.pop();

    if (current_node->idx == target_idx) {
      return current_node;
    }

    for (auto succ : current_node->succs) {
      node_stack.push(succ);
    }
  }

  return nullptr;
}

std::vector<size_t> DominTree::get_domin_succs(size_t id) {
  DTNode* node = find_node_by_index(id);

  if (node) {
    std::vector<size_t> succ_idxs;
    for (DTNode* succ : node->succs) {
      succ_idxs.push_back(succ->idx);
    }
    return succ_idxs;
  }

  return {};
}

void DominTree::delete_tree(DTNode* node) {
  if (node) {
    for (DTNode* child : node->succs) {
      delete_tree(child);
    }
    delete node;
  }
}

DominTree::~DominTree() {
  delete_tree(_domin_tree_root);
}

bool DominTree::dominates(size_t maybe_dominator_idx, size_t maybe_successor_idx) {
  DTNode* current_node = find_node_by_index(maybe_successor_idx);
  if (!current_node) {
    return false;
  }

  while (current_node) {
    if (current_node->idx == maybe_dominator_idx) {
      return true;
    }

    current_node = find_parent(current_node);
  }

  return false;
}

bool DominTree::dominates_instr(Instruction* first, Instruction* second) {
  auto* bb_first = first->getBB();
  auto* bb_second = second->getBB();
  if (bb_first != bb_second) {
    auto first_idx = find_node_by_index(bb_first->get_id())->idx;
    auto second_idx = find_node_by_index(bb_second->get_id())->idx;
    return dominates(first_idx, second_idx);
  }

  auto* next_instr = first->get_next();
  while (next_instr != nullptr) {
    if (next_instr == second) {
      return true;
    }
    next_instr = next_instr->get_next();
  }

  return false;
}

bool DominTree::is_dominated(size_t maybe_successor_idx, size_t maybe_dominator_idx) {
  return dominates(maybe_dominator_idx, maybe_successor_idx);
}

DTNode* DominTree::find_parent(DTNode* node) {
  if (node == _domin_tree_root) {
    return nullptr;
  }

  std::stack<DTNode*> stack;
  stack.push(_domin_tree_root);

  while (!stack.empty()) {
    DTNode* current = stack.top();
    stack.pop();

    for (DTNode* child : current->succs) {
      if (child == node) {
        return current;
      }
      stack.push(child);
    }
  }

  return nullptr;
}

void DominTree::print_tree() {
  if (!_domin_tree_root) {
    std::cout << "The dominator tree is empty." << std::endl;
    return;
  }

  print_tree_helper(_domin_tree_root, 0);
}

void DominTree::print_tree_helper(DTNode* node, int depth) {
  if (!node) {
    return;
  }

  for (int i = 0; i < depth; ++i) {
    std::cout << "  ";
  }

  std::cout << "Node " << node->idx << std::endl;

  for (DTNode* child : node->succs) {
    print_tree_helper(child, depth + 1);
  }
}

std::vector<size_t> DominTree::preorder_traversal() {
  std::vector<size_t> result;
  if (!_domin_tree_root) {
    return result;
  }

  std::stack<DTNode*> node_stack;
  node_stack.push(_domin_tree_root);

  while (!node_stack.empty()) {
    DTNode* current_node = node_stack.top();
    node_stack.pop();

    result.push_back(current_node->idx);

    for (auto it = current_node->succs.rbegin(); it != current_node->succs.rend(); ++it) {
      node_stack.push(*it);
    }
  }

  return result;
}

std::vector<size_t> DominTree::get_subtree(size_t start_idx) {
  std::vector<size_t> subtree_nodes;

  DTNode* start_node = find_node_by_index(start_idx);
  if (!start_node) {
    return subtree_nodes;
  }

  std::stack<DTNode*> node_stack;
  node_stack.push(start_node);

  while (!node_stack.empty()) {
    DTNode* current_node = node_stack.top();
    node_stack.pop();

    subtree_nodes.push_back(current_node->idx);

    for (DTNode* succ : current_node->succs) {
      node_stack.push(succ);
    }
  }

  return subtree_nodes;
}

}  // namespace custom
