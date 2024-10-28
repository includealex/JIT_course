#include <gtest/gtest.h>
#include "../includes/ir_builder.hpp"

TEST(LoopAnalyzerTest, FirstExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* H = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* I = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* K = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* L = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* O = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* Q = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* R = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* T = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* V = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* W = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* X = custom::IRBuilder::createBasicBlock(graph);

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

    custom::DFS df;
    df.run_dfs(A);

    std::vector<std::pair<std::size_t, std::vector<std::size_t>>> expected_loops = {
        {A->get_id(), {I->get_id(), W->get_id(), K->get_id()}},
        {B->get_id(), {R->get_id()}},
        {E->get_id(), {O->get_id()}}
    };

    auto loops = df.get_loops();

    ASSERT_EQ(loops.size(), expected_loops.size());

    for (const auto& [expected_header, expected_latches] : expected_loops) {
        bool found = false;

        for (auto& loop : loops) {
            if (loop.get_header()->get_id() == expected_header) {
                found = true;

                std::vector<std::size_t> actual_latches;
                for (const auto& latch : loop.get_latches()) {
                    actual_latches.push_back(latch->get_id());
                }

                std::sort(actual_latches.begin(), actual_latches.end());
                auto sorted_expected_latches = expected_latches;
                std::sort(sorted_expected_latches.begin(), sorted_expected_latches.end());

                EXPECT_EQ(actual_latches, sorted_expected_latches);
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    df.populate_graph();

    delete graph;
}
