#include <gtest/gtest.h>

#include "../includes/ir_builder.hpp"
#include "../includes/loop_tree.hpp"

TEST(LoopAnalyzerTest, FirstExampleDFSCheck) {
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
    df.run_dfs(graph->get_root());

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

    delete graph;
}

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

    custom::LoopTree lt;
    lt.build_tree(graph);

    ASSERT_NE(lt.root, nullptr) << "Root node should be initialized";

    ASSERT_FALSE(lt.root->get_blocks_id().empty()) << "Root should contain non-loop blocks";
    ASSERT_EQ(lt.root->succs.size(), 1);

    auto& first_child = lt.root->succs[0];
    ASSERT_EQ(first_child->idx, A->get_id());
    std::vector<size_t> expected_blocks = {L->get_id(), V->get_id(), D->get_id(), Q->get_id(), T->get_id(), G->get_id()};
    std::vector<size_t> expected_latches = {I->get_id(), W->get_id(), K->get_id()};

    ASSERT_EQ(first_child->get_blocks_id(), expected_blocks);
    ASSERT_EQ(first_child->get_latches_id(), expected_latches);

    ASSERT_EQ(first_child->succs.size(), C->get_id());
    for (const auto& successor : first_child->succs) {
        if (successor->idx == static_cast<int>(E->get_id())) {
            ASSERT_EQ(successor->get_blocks_id(), (std::vector<size_t>{F->get_id(), H->get_id()}));
            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{O->get_id()});
        } else if (successor->idx == static_cast<int>(B->get_id())) {
            ASSERT_EQ(successor->get_blocks_id(), std::vector<size_t>{C->get_id()});
            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{R->get_id()});
        }
    }

    delete graph;
}

TEST(LoopAnalyzerTest, SecondExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_false(C);
    B->add_succs_true(F);
    C->add_succs_true(D);
    F->add_succs_false(E);
    F->add_succs_true(G);
    G->add_succs_true(D);

    custom::LoopTree lt;
    lt.build_tree(graph);

    ASSERT_NE(lt.root, nullptr);

    ASSERT_FALSE(lt.root->get_blocks_id().empty());

    ASSERT_EQ(lt.root->succs.size(), 0);

    std::vector<std::size_t> expected = {A->get_id(), B->get_id(), C->get_id(), D->get_id(), E->get_id(), F->get_id(), G->get_id()};
    ASSERT_EQ(lt.root->get_blocks_id(), expected);

    delete graph;
}

TEST(LoopAnalyzerTest, ThirdExample) {
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
    custom::BasicBlock* J = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* K = custom::IRBuilder::createBasicBlock(graph);
    
    A->add_succs_true(B);
    B->add_succs_true(J);
    B->add_succs_false(C);
    C->add_succs_true(D);
    D->add_succs_true(E);
    D->add_succs_false(C);
    E->add_succs_true(F);
    F->add_succs_true(G);
    F->add_succs_false(E);
    G->add_succs_true(I);
    G->add_succs_false(H);
    // H->add_succs_true(A);
    I->add_succs_true(K);
    J->add_succs_true(C);

    custom::LoopTree lt;
    lt.build_tree(graph);

    delete graph;
}

TEST(LoopAnalyzerTest, FourthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(D);
    B->add_succs_false(C);
    D->add_succs_true(E);
    E->add_succs_true(A);

    custom::LoopTree lt;
    lt.build_tree(graph);

    ASSERT_EQ(lt.root->succs.size(), 1);
    ASSERT_EQ(lt.root->get_blocks_id(), (std::vector<size_t>{C->get_id()}));

    ASSERT_EQ(lt.root->succs[0]->get_blocks_id(), (std::vector<size_t>{D->get_id(), B->get_id()}));
    ASSERT_EQ(lt.root->succs[0]->get_latches_id(), (std::vector<size_t>{E->get_id()}));

    delete graph;
}

TEST(LoopAnalyzerTest, FifthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(C);
    C->add_succs_true(D);
    C->add_succs_false(F);
    D->add_succs_true(F);
    D->add_succs_false(E);
    E->add_succs_true(A);

    custom::LoopTree lt;
    lt.build_tree(graph);

    ASSERT_EQ(lt.root->succs.size(), 1);
    ASSERT_EQ(lt.root->get_blocks_id(), (std::vector<size_t>{F->get_id()}));

    ASSERT_EQ(lt.root->succs[0]->get_blocks_id(), (std::vector<size_t>{D->get_id(), B->get_id(), C->get_id()}));
    ASSERT_EQ(lt.root->succs[0]->get_latches_id(), (std::vector<size_t>{E->get_id()}));

    delete graph;
}

TEST(LoopAnalyzerTest, SixthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* H = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_false(C);
    B->add_succs_true(D);
    C->add_succs_true(F);
    C->add_succs_false(E);
    D->add_succs_true(F);
    F->add_succs_true(G);
    G->add_succs_true(H);
    // G->add_succs_false(B);
    // H->add_succs_true(A);

    custom::LoopTree lt;
    lt.build_tree(graph);

    delete graph;
}
