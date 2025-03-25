#ifndef INCLUDES_GRAPH_HPP_
#define INCLUDES_GRAPH_HPP_

#include <cstddef>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class BasicBlock;

class Graph final {
 public:
  Graph() = default;
  ~Graph();

  void addBasicBlock(BasicBlock* block);
  BasicBlock* get_block(size_t index) const;
  BasicBlock* get_root() const;

  BasicBlock* split_BasicBlock(Instruction* instr, BasicBlock* cur_block);
  std::size_t basic_blocks_num() const;
  void add_return_block(BasicBlock* basicBlock);

 private:
  BasicBlock* _root;
  std::size_t _blocks_size = 0;
  std::vector<BasicBlock*> _blocks;
  std::vector<BasicBlock*> _return_blocks;
};

}  // namespace custom

#endif  // INCLUDES_GRAPH_HPP_
