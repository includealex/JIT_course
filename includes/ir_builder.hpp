#ifndef INCLUDES_IR_BUILDER_HPP_
#define INCLUDES_IR_BUILDER_HPP_

#include "basic_block.hpp"
#include "dfs.hpp"
#include "graph.hpp"
#include "instruction.hpp"
#include "domin_tree.hpp"

namespace custom {

class IRBuilder final {
 public:
    static void createInstruction(Opcode opcode, Type type, BasicBlock* basic_block,
                                          std::size_t destReg = -1, const std::vector<std::size_t>& srcRegs = {}) {
        Instruction* inst = new Instruction(opcode, type, basic_block, destReg, srcRegs);
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

} // namespace custom

#endif // INCLUDES_IR_BUILDER_HPP_

