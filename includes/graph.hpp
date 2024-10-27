#ifndef INCLUDES_GRAPH_HPP_
#define INCLUDES_GRAPH_HPP_

#include <cstddef>
#include <stack>
#include <set>
#include <vector>

namespace custom {

class BasicBlock;

class Graph final {
 public:
    Graph() = default;
    ~Graph();

    void addBasicBlock(BasicBlock* block);
    BasicBlock* get_block(size_t index) const;

    std::size_t basic_blocks_num() const;

 private:
   static BasicBlock* _root;
   std::size_t _blocks_size = 0;
   std::vector<BasicBlock*> _blocks;
};

} // namespace custom

#endif // INCLUDES_GRAPH_HPP_
