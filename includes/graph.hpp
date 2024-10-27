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
   void addBasicBlock(BasicBlock* block);
   BasicBlock* get_block(std::size_t idx, BasicBlock* block = _root);
   std::size_t basic_blocks_num();

 private:
   static BasicBlock* _root;
   std::size_t _blocks_size = 0;
};

} // namespace custom

#endif // INCLUDES_GRAPH_HPP_

