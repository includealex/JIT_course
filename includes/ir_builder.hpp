#ifndef INCLUDES_IR_BUILDER_HPP_
#define INCLUDES_IR_BUILDER_HPP_

#include "basic_block.hpp"
#include "dfs.hpp"
#include "domin_tree.hpp"
#include "graph.hpp"
#include "instruction.hpp"
#include "regs.hpp"

namespace custom {

class IRBuilder final {
 public:
  static void createInstruction(Opcode opcode,
                                Type type,
                                BasicBlock* basic_block,
                                const std::vector<std::size_t>& destRegs = {},
                                const std::vector<std::size_t>& srcRegs = {}) {
    Instruction* inst = new Instruction(opcode, type, basic_block, destRegs, srcRegs);
    basic_block->pushback_instr(inst);
    return;
  }

  static void createInstruction(Opcode opcode,
                                Type type,
                                BasicBlock* basic_block,
                                const std::vector<std::size_t>& destRegs = {},
                                const std::size_t value = 0) {
    Instruction* inst = new Instruction(opcode, type, basic_block, destRegs, value);
    basic_block->pushback_instr(inst);
    return;
  }

  static BasicBlock* createBasicBlock(Graph* graph) {
    BasicBlock* basic_block = new BasicBlock(graph);
    graph->addBasicBlock(basic_block);
    return basic_block;
  }

  static Graph* createGraph() {
    return new Graph();
  }
};

}  // namespace custom

#endif  // INCLUDES_IR_BUILDER_HPP_
