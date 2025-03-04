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

  optimizer.constant_fold(graph, &builder);

  ASSERT_EQ(A->instructions_amount(), 1);

  auto final_instr = A->get_first_inst();
  ASSERT_EQ(final_instr->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(final_instr->getDestRegs().size(), 1);
  ASSERT_EQ(final_instr->getDestRegs()[0], custom::VRegs::v2);

  delete graph;
}

// FIXME:
// TEST(OptimizerTest, ConstantFoldingSubgraph) {
//   custom::IRBuilder builder;

//   custom::Graph* graph = builder.createGraph();
//   custom::BasicBlock* A = builder.createBasicBlock(graph);
//   custom::BasicBlock* B = builder.createBasicBlock(graph);

//   A->add_succs_true(B);

//   builder.createInstruction(
//       custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v0}, 1);

//   builder.createInstruction(
//       custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v1}, 2);

//   builder.createInstruction(custom::Opcode::SUB,
//                             custom::Type::myu64,
//                             A,
//                             std::vector<size_t>{custom::VRegs::v2},
//                             std::vector<size_t>{custom::VRegs::v1, custom::VRegs::v0});

//   builder.createInstruction(custom::Opcode::XOR,
//                             custom::Type::myu64,
//                             B,
//                             std::vector<size_t>{custom::VRegs::v3},
//                             std::vector<size_t>{custom::VRegs::v2, custom::VRegs::v1});

//   custom::Optimizer optimizer;

//   optimizer.constant_fold(graph, &builder);

//   ASSERT_EQ(B->instructions_amount(), 1);
//   auto final_instr = B->get_first_inst();
//   ASSERT_EQ(final_instr->getOpcode(), custom::Opcode::MOVI);
//   ASSERT_EQ(final_instr->getDestRegs().size(), 1);
//   ASSERT_EQ(final_instr->getDestRegs()[0], custom::VRegs::v3);

//   delete graph;
// }

TEST(OptimizerTest, PeepholeOptimization) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  builder.createInstruction(custom::Opcode::MOVI, custom::Type::myu64, A, {custom::VRegs::v0}, 8);
  builder.createInstruction(custom::Opcode::MOVI, custom::Type::myu64, A, {custom::VRegs::v1}, 0);
  builder.createInstruction(custom::Opcode::ASHR,
                            custom::Type::myu64,
                            A,
                            {custom::VRegs::v2},
                            {custom::VRegs::v0, custom::VRegs::v1});

  builder.createInstruction(custom::Opcode::MOVI, custom::Type::myu64, A, {custom::VRegs::v3}, 5);
  builder.createInstruction(custom::Opcode::SUB,
                            custom::Type::myu64,
                            A,
                            {custom::VRegs::v4},
                            {custom::VRegs::v3, custom::VRegs::v1});

  builder.createInstruction(custom::Opcode::SUB,
                            custom::Type::myu64,
                            A,
                            {custom::VRegs::v5},
                            {custom::VRegs::v3, custom::VRegs::v3});

  builder.createInstruction(custom::Opcode::XOR,
                            custom::Type::myu64,
                            A,
                            {custom::VRegs::v6},
                            {custom::VRegs::v3, custom::VRegs::v1});

  builder.createInstruction(custom::Opcode::XOR,
                            custom::Type::myu64,
                            A,
                            {custom::VRegs::v7},
                            {custom::VRegs::v3, custom::VRegs::v3});

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  ASSERT_EQ(A->instructions_amount(), 5);

  auto final_instr = A->get_first_inst();
  ASSERT_EQ(final_instr->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(final_instr->getDestRegs().size(), 1);
  ASSERT_EQ(final_instr->getDestRegs()[0], custom::VRegs::v0);
  ASSERT_EQ(final_instr->getImmediateValue(), 8);

  final_instr = final_instr->get_next();
  ASSERT_EQ(final_instr->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(final_instr->getDestRegs().size(), 1);
  ASSERT_EQ(final_instr->getDestRegs()[0], custom::VRegs::v1);
  ASSERT_EQ(final_instr->getImmediateValue(), 0);

  delete graph;
}

}  // namespace custom
