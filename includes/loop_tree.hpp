#ifndef INCLUDES_LOOP_TREE_HPP_
#define INCLUDES_LOOP_TREE_HPP_

#include "graph.hpp"
#include "dfs.hpp"

#include <algorithm>
#include <numeric>

namespace custom {

struct LTNode final{
 public:
    size_t idx;
    LTNode(size_t index) : idx(index) {}

    void set_header(size_t h_id) { _header_id = h_id;}
    void set_blocks_id(std::vector<size_t> b_id) {_blocks_id = b_id;}
    void set_latches_id(std::vector<size_t> l_id) {_latches_id = l_id;}

    std::vector<LTNode*> succs;
 private:
    size_t _header_id;
    std::vector<size_t> _latches_id;
    std::vector<size_t> _blocks_id;
};


class LoopTree final {
 public:
    void build_tree(Graph* graph);
};

} // namespace custom


#endif //INCLUDES_LOOP_TREE_HPP_