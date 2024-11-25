#include <gtest/gtest.h>

#include "ir_builder.hpp"
#include "loop_tree.hpp"

namespace custom {

TEST(DominTreeTest, SecondExample) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);
  custom::BasicBlock* E = builder.createBasicBlock(graph);
  custom::BasicBlock* F = builder.createBasicBlock(graph);
  custom::BasicBlock* G = builder.createBasicBlock(graph);
  custom::BasicBlock* H = builder.createBasicBlock(graph);
  custom::BasicBlock* I = builder.createBasicBlock(graph);
  custom::BasicBlock* J = builder.createBasicBlock(graph);
  custom::BasicBlock* K = builder.createBasicBlock(graph);
  custom::BasicBlock* L = builder.createBasicBlock(graph);
  custom::BasicBlock* M = builder.createBasicBlock(graph);
  custom::BasicBlock* N = builder.createBasicBlock(graph);
  custom::BasicBlock* O = builder.createBasicBlock(graph);
  custom::BasicBlock* P = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  B->add_succs_false(I);
  C->add_succs_true(D);
  C->add_succs_false(I);
  D->add_succs_true(E);
  E->add_succs_true(G);
  E->add_succs_false(F);
  F->add_succs_false(C);
  G->add_succs_true(H);
  I->add_succs_true(J);
  J->add_succs_true(K);
  K->add_succs_true(L);
  L->add_succs_true(O);
  L->add_succs_false(M);
  M->add_succs_true(N);
  N->add_succs_true(J);
  O->add_succs_true(P);
  P->add_succs_true(I);

  custom::DominTree dt;
  custom::LoopTree lt;

  dt.build_tree(graph);
  lt.build_tree(graph);

  delete graph;
}

} // namespace custom
