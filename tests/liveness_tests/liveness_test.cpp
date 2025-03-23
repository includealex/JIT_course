#include "liveness.hpp"

#include <gtest/gtest.h>

#include "ir_builder.hpp"
namespace custom {
TEST(LivenessTest, FirstExample) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);
  custom::BasicBlock* E = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  B->add_succs_false(D);
  C->add_succs_true(B);
  D->add_succs_false(E);

  auto* v0 = builder.createMOVI(Type::myu64, A, 1);
  auto* v1 = builder.createMOVI(Type::myu64, A, 10);
  auto* v2 = builder.createMOVI(Type::myu64, A, 20);

  auto* v3 = builder.createPHI(Type::myu64, B);
  auto* v4 = builder.createPHI(Type::myu64, B);
  auto* v5 = builder.createCMP(Type::myu64, B, v4, v0);
  auto* v6 = builder.createNEQ(Type::myu64, B, v5);

  auto* v7 = builder.createMUL(Type::myu64, C, v3, v4);
  auto* v8 = builder.createSUB(Type::myu64, C, v4, v0);

  auto* v9 = builder.createADD(Type::myu64, D, v2, v3);
  auto* v10 = builder.createRET(Type::myu64, D);

  v3->AddPhiUsage(v7, v0);
  v4->AddPhiUsage(v8, v1);

  LiveInterval expected;

  expected.add(0, LiveRange(2, 20));
  expected.add(1, LiveRange(4, 20));
  expected.add(2, LiveRange(6, 22));
  expected.add(3, LiveRange(8, 22));
  expected.add(4, LiveRange(8, 18));
  expected.add(5, LiveRange(10, 12));
  expected.add(6, LiveRange(12, 14));
  expected.add(7, LiveRange(16, 20));
  expected.add(8, LiveRange(18, 20));
  expected.add(9, LiveRange(22, 24));
  expected.add(10, LiveRange(24, 26));

  custom::Liveness liveness;
  auto res = liveness.run_analysis(graph);

  ASSERT_EQ(res, expected);

  delete graph;
}

TEST(LivenessTest, SecondExample) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);

  auto* v0 = builder.createMOVI(Type::myu64, A, 1);
  auto* v1 = builder.createMOVI(Type::myu64, A, 10);
  auto* v2 = builder.createMOVI(Type::myu64, A, 20);

  auto* v3 = builder.createADD(Type::myu64, B, v2, v1);
  auto* v4 = builder.createADD(Type::myu64, B, v1, v0);

  auto* v5 = builder.createMUL(Type::myu64, C, v4, v3);

  LiveInterval expected;

  expected.add(0, LiveRange(2, 10));
  expected.add(1, LiveRange(4, 10));
  expected.add(2, LiveRange(6, 8));
  expected.add(3, LiveRange(8, 14));
  expected.add(4, LiveRange(10, 14));
  expected.add(5, LiveRange(14, 16));

  custom::Liveness liveness;
  auto res = liveness.run_analysis(graph);

  ASSERT_EQ(res, expected);

  delete graph;
}

}  // namespace custom