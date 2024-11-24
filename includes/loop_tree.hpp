#ifndef INCLUDES_LOOP_TREE_HPP_
#define INCLUDES_LOOP_TREE_HPP_

#include <algorithm>
#include <memory>
#include <numeric>
#include <unordered_map>

#include "dfs.hpp"
#include "domin_tree.hpp"
#include "graph.hpp"
#include "rpo.hpp"

namespace custom {

struct LTNode final {
 public:
  int idx;
  LTNode(int index) : idx(index) {}

  void set_header(int h_id) {
    _header_id = h_id;
  }
  void set_blocks_id(std::vector<size_t> b_id) {
    _blocks_id = b_id;
  }
  void append_blocks_id(std::vector<size_t>& b_id) {
    for (auto& el : b_id) {
      auto it = std::find(_blocks_id.begin(), _blocks_id.end(), el);
      if (it != _blocks_id.end()) {
        continue;
      }

      _blocks_id.push_back(el);
    }
  }
  void add_latch_id(std::size_t l_id) {
    _latches_id.push_back(l_id);
  }

  std::size_t get_header_id() {
    return _header_id;
  }
  std::vector<std::size_t>& get_blocks_id() {
    return _blocks_id;
  }
  std::vector<std::size_t>& get_latches_id() {
    return _latches_id;
  }

  std::vector<LTNode*> succs;
  std::vector<LTNode*> preds;

  void print_node(int level = 0) const {
    std::string indent(level * 2, ' ');
    std::cout << indent << "Node (ID: " << idx << ")\n";
    std::cout << indent << " - Header: " << _header_id << std::endl;
    std::cout << indent << " - Blocks: ";
    for (size_t block_id : _blocks_id) {
      std::cout << block_id << " ";
    }
    std::cout << "\n" << indent << " - Latches: ";
    for (size_t latch_id : _latches_id) {
      std::cout << latch_id << " ";
    }
    std::cout << "\n";

    for (const auto& succ : succs) {
      succ->print_node(level + 1);
    }
  }

 private:
  int _header_id = -1;
  std::vector<size_t> _latches_id;
  std::vector<size_t> _blocks_id;
};

class LoopTree final {
 public:
  LoopTree() : _root(nullptr) {}
  ~LoopTree();
  void build_tree(Graph* graph);

  LTNode* get_root() {
    return _root;
  }

  void print_tree() const {
    if (_root) {
      std::cout << "Loop Tree Structure:\n";
      _root->print_node();
    } else {
      std::cout << "Loop Tree is empty.\n";
    }
  }

 private:
  LTNode* _root;

  LTNode* build_loop_tree(std::vector<LTNode*>& nodes);
  std::vector<LTNode*> build_loop_vector(Graph* graph);

  void delete_tree(LTNode* node);
};

}  // namespace custom

#endif  // INCLUDES_LOOP_TREE_HPP_