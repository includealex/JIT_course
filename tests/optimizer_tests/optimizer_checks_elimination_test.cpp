#include <gtest/gtest.h>

#include <iostream>

#include "basic_block.hpp"
#include "ir_builder.hpp"
#include "optimizer.hpp"

namespace custom {

TEST(OptimizerTest, CheckNullEliminationSimple) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createNULLCHECK(Type::myu64, A, v0);
  auto* v2 = builder.createNULLCHECK(Type::myu64, A, v0);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(A->get_last_inst()->get_id(), 1);
  delete graph;
}

TEST(OptimizerTest, CheckNullEliminationTwoBBs) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createNULLCHECK(Type::myu64, A, v0);
  auto* v2 = builder.createNULLCHECK(Type::myu64, A, v0);

  A->add_succs_false(B);
  auto* v3 = builder.createMOVI(Type::myu64, B, 42);
  auto* v4 = builder.createNULLCHECK(Type::myu64, B, v0);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(B->get_last_inst(), B->get_first_inst());
  delete graph;
}

TEST(OptimizerTest, CheckNullEliminationDiamondBBs) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createNULLCHECK(Type::myu64, A, v0);
  auto* v2 = builder.createNULLCHECK(Type::myu64, A, v0);

  A->add_succs_false(B);
  auto* v3 = builder.createMOVI(Type::myu64, B, 42);
  auto* v4 = builder.createNULLCHECK(Type::myu64, B, v0);

  A->add_succs_true(C);
  auto* v5 = builder.createNULLCHECK(Type::myu64, C, v0);
  auto* v6 = builder.createMOVI(Type::myu64, C, 13);
  auto* v7 = builder.createNULLCHECK(Type::myu64, C, v0);

  C->add_succs_false(D);
  B->add_succs_false(D);
  auto* v8 = builder.createMOVI(Type::myu64, D, 0);
  auto* v9 = builder.createNULLCHECK(Type::myu64, D, v0);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(B->get_last_inst(), B->get_first_inst());
  ASSERT_EQ(C->get_last_inst(), C->get_first_inst());
  ASSERT_EQ(D->get_last_inst(), D->get_first_inst());
  delete graph;
}

TEST(OptimizerTest, CheckBoundsEliminationSimple) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 8);
  auto* v2 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 8);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(A->get_last_inst()->get_id(), 1);
  delete graph;
}

TEST(OptimizerTest, CheckBoundsEliminationSmaller) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 8);
  auto* v2 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 7);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(A->get_last_inst()->get_id(), 1);
  delete graph;
}

TEST(OptimizerTest, CheckBoundsEliminationDiamondBBs) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);
  custom::BasicBlock* E = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 10);
  auto* v1 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 100);
  auto* v2 = builder.createBOUNDSCHECK(Type::myu64, A, v0, 8);

  A->add_succs_false(B);
  auto* v3 = builder.createMOVI(Type::myu64, B, 42);
  auto* v4 = builder.createBOUNDSCHECK(Type::myu64, B, v0, 44);

  A->add_succs_true(C);
  auto* v5 = builder.createBOUNDSCHECK(Type::myu64, C, v0, 99);
  auto* v6 = builder.createMOVI(Type::myu64, C, 13);
  auto* v7 = builder.createBOUNDSCHECK(Type::myu64, C, v0, 99);

  C->add_succs_false(D);
  B->add_succs_false(D);
  auto* v8 = builder.createMOVI(Type::myu64, D, 0);
  auto* v9 = builder.createBOUNDSCHECK(Type::myu64, D, v0, 0);

  D->add_succs_false(E);
  auto* v10 = builder.createBOUNDSCHECK(Type::myu64, E, v0, 101);

  custom::Optimizer optimizer;
  optimizer.eliminate_checks(graph, &builder);

  ASSERT_EQ(B->get_last_inst(), B->get_first_inst());
  ASSERT_EQ(C->get_last_inst(), C->get_first_inst());
  ASSERT_EQ(D->get_last_inst(), D->get_first_inst());
  ASSERT_EQ(E->get_last_inst(), E->get_first_inst());
  delete graph;
}

}  // namespace custom