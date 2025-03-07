#include "rpo.hpp"

#include <gtest/gtest.h>

#include "basic_block.hpp"
#include "ir_builder.hpp"

namespace custom {

TEST(RPOTest, RPOsimplest) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  A->add_succs_false(C);
  C->add_succs_true(D);
  B->add_succs_true(D);

  custom::RPO rpo;
  rpo.run_rpo(graph->get_root());

  std::vector<std::size_t> rpo_ids = rpo.get_rpo_ids_arr();

  std::vector<std::size_t> expected_rpo = {A->get_id(), B->get_id(), C->get_id(), D->get_id()};

  EXPECT_EQ(rpo_ids.size(), expected_rpo.size());

  for (size_t i = 0; i < rpo_ids.size(); ++i) {
    EXPECT_EQ(rpo_ids[i], expected_rpo[i]);
  }

  delete graph;
}

TEST(RPOTest, RPOhuge) {
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
  custom::BasicBlock* K = builder.createBasicBlock(graph);
  custom::BasicBlock* L = builder.createBasicBlock(graph);
  custom::BasicBlock* O = builder.createBasicBlock(graph);
  custom::BasicBlock* Q = builder.createBasicBlock(graph);
  custom::BasicBlock* R = builder.createBasicBlock(graph);
  custom::BasicBlock* T = builder.createBasicBlock(graph);
  custom::BasicBlock* V = builder.createBasicBlock(graph);
  custom::BasicBlock* W = builder.createBasicBlock(graph);
  custom::BasicBlock* X = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  C->add_succs_true(D);
  C->add_succs_false(R);
  D->add_succs_true(E);
  E->add_succs_true(F);
  E->add_succs_false(L);
  F->add_succs_true(H);
  F->add_succs_false(G);
  G->add_succs_true(Q);
  H->add_succs_true(T);
  H->add_succs_false(O);
  I->add_succs_true(A);
  K->add_succs_true(A);
  L->add_succs_true(I);
  O->add_succs_true(E);
  Q->add_succs_true(W);
  R->add_succs_true(B);
  T->add_succs_true(V);
  V->add_succs_true(X);
  V->add_succs_false(K);
  W->add_succs_true(A);

  custom::RPO rpo;
  rpo.run_rpo(graph->get_root());

  std::vector<std::size_t> rpo_ids = rpo.get_rpo_ids_arr();

  std::vector<std::size_t> expected_rpo = {A->get_id(),
                                           B->get_id(),
                                           C->get_id(),
                                           D->get_id(),
                                           E->get_id(),
                                           F->get_id(),
                                           H->get_id(),
                                           T->get_id(),
                                           V->get_id(),
                                           X->get_id(),
                                           K->get_id(),
                                           O->get_id(),
                                           G->get_id(),
                                           Q->get_id(),
                                           W->get_id(),
                                           L->get_id(),
                                           I->get_id(),
                                           R->get_id()};

  EXPECT_EQ(rpo_ids.size(), expected_rpo.size());

  for (size_t i = 0; i < rpo_ids.size(); ++i) {
    EXPECT_EQ(rpo_ids[i], expected_rpo[i]);
  }

  delete graph;
}

TEST(RPOTest, ReversedRPOSimplest) {
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
  custom::BasicBlock* K = builder.createBasicBlock(graph);
  custom::BasicBlock* L = builder.createBasicBlock(graph);
  custom::BasicBlock* O = builder.createBasicBlock(graph);
  custom::BasicBlock* Q = builder.createBasicBlock(graph);
  custom::BasicBlock* R = builder.createBasicBlock(graph);
  custom::BasicBlock* T = builder.createBasicBlock(graph);
  custom::BasicBlock* V = builder.createBasicBlock(graph);
  custom::BasicBlock* W = builder.createBasicBlock(graph);
  custom::BasicBlock* X = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  C->add_succs_true(D);
  C->add_succs_false(R);
  D->add_succs_true(E);
  E->add_succs_true(F);
  E->add_succs_false(L);
  F->add_succs_true(H);
  F->add_succs_false(G);
  G->add_succs_true(Q);
  H->add_succs_true(T);
  H->add_succs_false(O);
  I->add_succs_true(A);
  K->add_succs_true(A);
  L->add_succs_true(I);
  O->add_succs_true(E);
  Q->add_succs_true(W);
  R->add_succs_true(B);
  T->add_succs_true(V);
  V->add_succs_true(X);
  V->add_succs_false(K);
  W->add_succs_true(A);

  custom::RPO rpo;

  rpo.run_reversed_rpo(B, R);
  std::vector<std::size_t> expected_rpo_BR = {R->get_id(), C->get_id(), B->get_id()};
  auto rpo_ids_BR = rpo.get_rpo_ids_arr();
  for (size_t i = 0; i < rpo_ids_BR.size(); ++i) {
    EXPECT_EQ(rpo_ids_BR[i], expected_rpo_BR[i]);
  }

  rpo.run_reversed_rpo(E, O);
  std::vector<std::size_t> expected_rpo_EO = {O->get_id(), H->get_id(), F->get_id(), E->get_id()};
  auto rpo_ids_EO = rpo.get_rpo_ids_arr();
  for (size_t i = 0; i < rpo_ids_EO.size(); ++i) {
    EXPECT_EQ(rpo_ids_EO[i], expected_rpo_EO[i]);
  }

  std::vector<size_t> A_all_latches_rpo;

  rpo.run_reversed_rpo(A, K);
  auto rpo_ids_AK = rpo.get_rpo_ids_arr();
  A_all_latches_rpo.insert(A_all_latches_rpo.end(), rpo_ids_AK.begin(), rpo_ids_AK.end());

  rpo.run_reversed_rpo(A, I);
  auto rpo_ids_AI = rpo.get_rpo_ids_arr();
  A_all_latches_rpo.insert(A_all_latches_rpo.end(), rpo_ids_AI.begin(), rpo_ids_AI.end());

  rpo.run_reversed_rpo(A, W);
  auto rpo_ids_AW = rpo.get_rpo_ids_arr();
  A_all_latches_rpo.insert(A_all_latches_rpo.end(), rpo_ids_AW.begin(), rpo_ids_AW.end());

  std::set<size_t> all_latches_rpo(A_all_latches_rpo.begin(), A_all_latches_rpo.end());
  std::set<size_t> expected_set = {A->get_id(),
                                   B->get_id(),
                                   C->get_id(),
                                   D->get_id(),
                                   E->get_id(),
                                   F->get_id(),
                                   G->get_id(),
                                   H->get_id(),
                                   I->get_id(),
                                   K->get_id(),
                                   L->get_id(),
                                   O->get_id(),
                                   Q->get_id(),
                                   R->get_id(),
                                   T->get_id(),
                                   V->get_id(),
                                   W->get_id()};

  EXPECT_EQ(all_latches_rpo, expected_set);

  delete graph;
}

}  // namespace custom
