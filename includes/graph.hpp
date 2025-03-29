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

  void clear_blocks_pointers();
  void copy_graph() const;
  void addBasicBlock(BasicBlock* block);
  BasicBlock* get_block(size_t index) const;
  std::vector<BasicBlock*> get_blocks() const;
  BasicBlock* get_root() const;
  std::vector<BasicBlock*> get_ret_blocks() const;

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
