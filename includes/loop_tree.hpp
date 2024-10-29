#ifndef INCLUDES_LOOP_TREE_HPP_
#define INCLUDES_LOOP_TREE_HPP_

#include "graph.hpp"
#include "dfs.hpp"

#include <algorithm>
#include <numeric>
#include <unordered_map>

namespace custom {

struct LTNode final{
 public:
    int idx;
    LTNode(int index) : idx(index) {}

    void set_header(size_t h_id) { _header_id = h_id;}
    void set_blocks_id(std::vector<size_t> b_id) {_blocks_id = b_id;}
    void set_latches_id(std::vector<size_t> l_id) {_latches_id = l_id;}

    std::vector<std::size_t>& get_blocks_id() {return _blocks_id;}
    std::vector<std::size_t>& get_latches_id() {return _latches_id;}

    std::vector<LTNode*> succs;

    void print_node(int level = 0) const {
        std::string indent(level * 2, ' ');
        std::cout << indent << "Node (Header ID: " << _header_id << ", ID: " << idx << ")\n";
        std::cout << indent << " - Blocks: ";
        for (size_t block_id : _blocks_id) {
            std::cout << block_id << " ";
        }
        std::cout << "\n" << indent << " - Latches: ";
        for (size_t latch_id : _latches_id) {
            std::cout << latch_id << " ";
        }
        std::cout << "\n";

        // Print successors recursively
        for (const auto& succ : succs) {
            succ->print_node(level + 1);
        }
    }
 private:
    size_t _header_id;
    std::vector<size_t> _latches_id;
    std::vector<size_t> _blocks_id;
};


class LoopTree final {
 public:
    LoopTree() : root(nullptr) {}
    ~LoopTree();
    void build_tree(Graph* graph);
    LTNode* root;

    void print_tree() const {
        if (root) {
            std::cout << "Loop Tree Structure:\n";
            root->print_node();
        } else {
            std::cout << "Loop Tree is empty.\n";
        }
    }
 private:
    void delete_tree(LTNode* node);
};

} // namespace custom


#endif //INCLUDES_LOOP_TREE_HPP_