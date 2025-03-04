#include "optimizer.hpp"

#include <gtest/gtest.h>

#include "basic_block.hpp"
#include "ir_builder.hpp"

namespace custom {

TEST(OptimizerTest, ConstantFolding) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v0}, 1);

  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v1}, 2);

  builder.createInstruction(custom::Opcode::SUB,
                            custom::Type::myu64,
                            A,
                            std::vector<size_t>{custom::VRegs::v2},
                            std::vector<size_t>{custom::VRegs::v1, custom::VRegs::v0});

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph);

  ASSERT_EQ(A->instructions_amount(), 1);

  auto final_instr = A->get_first_inst();
  ASSERT_EQ(final_instr->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(final_instr->getDestRegs().size(), 1);
  ASSERT_EQ(final_instr->getDestRegs()[0], custom::VRegs::v2);

  delete graph;
}

}  // namespace custom