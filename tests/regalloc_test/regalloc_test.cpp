#include "regalloc.hpp"

#include <gtest/gtest.h>

#include <string>

#include "ir_builder.hpp"

namespace custom {
TEST(FloatRegallocTest, FirstExample) {
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
  auto* v6 = builder.createNEQ(Type::myu64, B, v5, static_cast<ImmType>(0));
  auto* v7 = builder.createMUL(Type::myu64, C, v3, v4);
  auto* v8 = builder.createSUB(Type::myu64, C, v4, v0);
  auto* v9 = builder.createADD(Type::myu64, D, v2, v3);
  auto* v10 = builder.createRET(Type::myu64, D);
  v3->AddPhiUsage(v7, v0);
  v4->AddPhiUsage(v8, v1);

  custom::FloatRegalloc rg;
  auto res = rg.run_analysis(graph);

  delete graph;
}

TEST(FloatRegallocTest, SecondExample) {
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

  custom::FloatRegalloc rg;
  auto res = rg.run_analysis(graph);

  delete graph;
}

TEST(FloatRegallocTest, ThirdExample) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  C->add_succs_true(D);
  D->add_succs_true(B);

  auto* v0 = builder.createMOVI(Type::myu64, A, 1);
  auto* v1 = builder.createMOVI(Type::myu64, A, 10);
  auto* v2 = builder.createMOVI(Type::myu64, A, 20);

  auto* v3 = builder.createADD(Type::myu64, B, v2, v1);
  auto* v4 = builder.createADD(Type::myu64, B, v1, v0);

  auto* v5 = builder.createMUL(Type::myu64, C, v4, v3);

  auto* v6 = builder.createMUL(Type::myu64, D, v5, v4);
  auto* v7 = builder.createADD(Type::myu64, D, v5, v4);

  custom::FloatRegalloc rg;
  auto res = rg.run_analysis(graph);

  delete graph;
}

}  // namespace custom