#ifndef IR_BUILDER_HPP_
#define IR_BUILDER_HPP_

#include "instruction.hpp"
#include "basic_block.hpp"
#include "graph.hpp"

namespace custom {

class IRBuilder final {
 public:
    static Instruction* createInstruction(Opcode opcode, Type type, BasicBlock* basic_block) {
        Instruction* inst = new Instruction(opcode, type, basic_block);
        basic_block->addInstruction(inst);
        return inst;
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

#endif // IR_BUILDER_HPP_

