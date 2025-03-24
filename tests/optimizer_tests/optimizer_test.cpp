#include "optimizer.hpp"

#include <gtest/gtest.h>

#include <iostream>

#include "basic_block.hpp"
#include "ir_builder.hpp"

namespace custom {

TEST(OptimizerTest, ConstantFoldingTwoInstrs) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createMOVI(Type::myu64, A, 7);
  auto* v3 = builder.createSUB(Type::myu64, A, v0, v1);

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph, &builder);

  Instruction* first = A->get_first_inst();
  Instruction* last = A->get_last_inst();

  EXPECT_EQ(first->get_next(), nullptr);
  EXPECT_EQ(first, last);

  EXPECT_EQ(first->getOpcode(), Opcode::MOVI);
  EXPECT_EQ(first->get_imm(), ImmType(3));

  delete graph;
}

TEST(OptimizerTest, ConstantFoldingTwoImms) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createXORI(Type::myu64, A, 4, 2);

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph, &builder);

  Instruction* first = A->get_first_inst();
  Instruction* last = A->get_last_inst();

  EXPECT_EQ(first->get_next(), nullptr);
  EXPECT_EQ(first, last);

  EXPECT_EQ(first->getOpcode(), Opcode::MOVI);
  EXPECT_EQ(first->get_imm(), ImmType(6));

  delete graph;
}

TEST(OptimizerTest, ConstantFoldingOneInstrOneImm) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 8);
  auto* v1 = builder.createASHRI(Type::myu64, A, v0, 3);

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph, &builder);

  Instruction* first = A->get_first_inst();
  Instruction* last = A->get_last_inst();

  EXPECT_EQ(first->get_next(), nullptr);
  EXPECT_EQ(first, last);

  EXPECT_EQ(first->getOpcode(), Opcode::MOVI);
  EXPECT_EQ(first->get_imm(), ImmType(1));

  delete graph;
}

TEST(OptimizerTest, PeepholeOptimization) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 8);
  auto* v1 = builder.createMOVI(Type::myu64, A, 0);
  auto* v2 = builder.createASHR(Type::myu64, A, v0, v1);
  auto* v3 = builder.createMOVI(Type::myu64, A, 5);

  auto* v4 = builder.createSUB(Type::myu64, A, v3, v1);
  auto* v5 = builder.createSUB(Type::myu64, A, v3, v3);
  auto* v6 = builder.createXOR(Type::myu64, A, v3, v1);
  auto* v7 = builder.createXOR(Type::myu64, A, v3, v3);

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  // TODO: add assertions

  delete graph;
}

}  // namespace custom
