#ifndef INCLUDES_DOMIN_TREE_HPP_
#define INCLUDES_DOMIN_TREE_HPP_

#include <algorithm>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#include "graph.hpp"
#include "dfs.hpp"

namespace custom {
struct DTNode {
    size_t idx;
    std::vector<DTNode*> succs;

    DTNode(size_t index) : idx(index) {}
};

class DominTree final {
 public:
    void build_tree(Graph* graph);
    std::vector<size_t> get_domin_succs(size_t id);
    DTNode* find_node_by_index(size_t target_idx);
    ~DominTree();

    bool dominates(size_t maybe_dominator_idx, size_t maybe_successor_idx);
    bool is_dominated(size_t maybe_successor_idx, size_t maybe_dominator_idx);

 private:
    DTNode* find_parent(DTNode* node);
    DTNode* convert_to_tree(const std::unordered_map<size_t, std::vector<size_t>>& aftermap, size_t rootIdx);
    DTNode* prebuild_tree(size_t idx, std::unordered_map<size_t, DTNode*>& nodeMap,
                    const std::unordered_map<size_t, std::vector<size_t>>& aftermap);

    std::unordered_map<size_t, std::vector<size_t>> get_premap(Graph* graph);
    DTNode* _domin_tree_root;

    void delete_tree(DTNode* node);
};


} //namespace custom

#endif //INCLUDES_DOMIN_TREE_HPP_

