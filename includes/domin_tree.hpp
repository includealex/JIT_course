#ifndef INCLUDES_DOMIN_TREE_HPP_
#define INCLUDES_DOMIN_TREE_HPP_

#include <algorithm>
#include <unordered_map>
#include <vector>


#include "graph.hpp"
#include "dfs.hpp"

namespace custom {

class DominTree final {
 public:
    struct Node {
        BasicBlock* cur;
        std::vector<Node*> next_gen;
    } node;

    void build_tree(Graph* graph);

 private:
    std::unordered_map<size_t, std::vector<size_t>> get_premap(Graph* graph);
};


} //namespace custom

#endif //INCLUDES_DOMIN_TREE_HPP_

