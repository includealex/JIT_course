#include "loop_tree.hpp"

namespace custom {

void LoopTree::build_tree(Graph* graph) {
  build_loop_vector(graph);
  _root = build_loop_tree(_loop_vector);
}

const std::vector<LTNode*>& LoopTree::get_loop_vector() const {
  return _loop_vector;
}

LTNode* LoopTree::build_loop_tree(std::vector<LTNode*>& nodes) {
  std::unordered_map<int, LTNode*> node_map;
  for (auto* node : nodes) {
    node_map[node->get_header_id()] = node;
  }

  LTNode* root = nullptr;
  for (auto& node : nodes) {
    if (node->idx == -1) {
      root = node;
      break;
    }
  }

  if (!root) {
    throw std::runtime_error("No loop tree root node found!");
  }

  for (auto& node : nodes) {
    if (node->idx == -1) {
      continue;
    }

    for (auto& potential_parent : nodes) {
      if (node->get_header_id() == potential_parent->get_header_id()) {
        continue;
      }

      auto& parent_blocks = potential_parent->get_blocks_id();
      if (std::find(parent_blocks.begin(), parent_blocks.end(), node->get_header_id()) !=
          parent_blocks.end()) {
        potential_parent->succs.push_back(node);
        node->preds.push_back(potential_parent);
        break;
      }
    }
  }

  for (auto& node : nodes) {
    auto& blocks = node->get_blocks_id();
    std::set<size_t> valid_blocks(blocks.begin(), blocks.end());

    for (auto& child : node->succs) {
      valid_blocks.erase(child->get_header_id());
      for (auto& latch_id : child->get_latches_id()) {
        valid_blocks.erase(latch_id);
      }
      for (auto& block_id : child->get_blocks_id()) {
        valid_blocks.erase(block_id);
      }
    }

    blocks.assign(valid_blocks.begin(), valid_blocks.end());
  }

  for (auto& node : nodes) {
    if (node->idx == -1) {
      continue;
    }

    if (node->preds.size() == 0) {
      root->succs.push_back(node);
      node->preds.push_back(root);
    }
  }

  return root;
}

void LoopTree::build_loop_vector(Graph* graph) {
  custom::DFS dfs;
  custom::RPO rpo;
  custom::DominTree domin_tree;
  auto graph_root = graph->get_root();

  domin_tree.build_tree(graph);
  dfs.run_dfs(graph_root);
  rpo.run_rpo(graph_root);

  std::vector<LTNode*> res_loop_vector;

  std::vector<size_t> all_ids = rpo.get_rpo_ids_arr();
  std::set<size_t> looped_ids;

  std::size_t loop_num = 0;
  auto loops = dfs.get_loops();
  for (auto& loop : loops) {
    LTNode* ltnode = new LTNode(loop_num);

    auto header = loop.get_header();
    auto header_id = header->get_id();
    ltnode->set_header(header_id);

    auto latches = loop.get_latches();
    for (auto& cur_latch : latches) {
      auto latch_id = cur_latch->get_id();
      if (!domin_tree.is_dominated(latch_id, header_id)) {
        loop.set_irreducible(true);
        continue;
      }
      looped_ids.insert(latch_id);
      ltnode->add_latch_id(latch_id);

      rpo.run_reversed_rpo(header, cur_latch);
      auto rpo_ids = rpo.get_rpo_ids_arr();

      rpo_ids.erase(std::remove(rpo_ids.begin(), rpo_ids.end(), latch_id), rpo_ids.end());
      rpo_ids.erase(std::remove(rpo_ids.begin(), rpo_ids.end(), header_id), rpo_ids.end());

      for (const auto& elem : rpo_ids) {
        looped_ids.insert(elem);
      }

      ltnode->append_blocks_id(rpo_ids);
    }
    ++loop_num;

    if (ltnode->get_header_id() && (ltnode->get_latches_id()).size() == 0) {
      delete ltnode;
      continue;
    }

    looped_ids.insert(header_id);

    res_loop_vector.push_back(ltnode);
  }

  std::vector<std::size_t> non_looped_ids;
  std::copy_if(all_ids.begin(),
               all_ids.end(),
               std::back_inserter(non_looped_ids),
               [&looped_ids](size_t elem) { return looped_ids.find(elem) == looped_ids.end(); });

  int32_t root_id = -1;
  LTNode* root_node = new LTNode(root_id);
  root_node->set_blocks_id(non_looped_ids);
  res_loop_vector.push_back(root_node);
  _loop_vector = res_loop_vector;
}

LoopTree::~LoopTree() {
  delete_tree(_root);
}

void LoopTree::delete_tree(LTNode* node) {
  if (node == nullptr)
    return;

  for (auto& child : node->succs) {
    delete_tree(child);
  }

  delete node;
}

}  // namespace custom
