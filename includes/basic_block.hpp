#ifndef INCLUDES_BASIC_BLOCK_HPP_
#define INCLUDES_BASIC_BLOCK_HPP_

#include <cstddef>
#include <iostream>
#include <vector>

namespace custom {

class Instruction;
class Graph;

struct BasicBlockMarker final {
 private:
    bool _dfs_marker = false;
    bool _loop_gray_marker = false;
    bool _loop_black_marker = false;
    bool _rpo_marker = false;
 public:
    void set_dfs_m(bool val) {_dfs_marker = val;}
    void set_rpo_m(bool val) {_rpo_marker = val;}
    void set_loop_gray_m(bool val) {_loop_gray_marker = val;}
    void set_loop_black_m(bool val) {_loop_black_marker = val;}

    bool is_dfs_m() {return _dfs_marker;}
    bool is_rpo_m() {return _rpo_marker;}
    bool is_loop_gray_m() {return _loop_gray_marker;}
    bool is_loop_black_m() {return _loop_black_marker;}
};

class BasicBlock final {
 public:
    BasicBlock(Graph* graph)
        : _first_Phi(nullptr), _first_inst(nullptr), _last_inst(nullptr), _graph(graph) { }

    ~BasicBlock();
    void pushback_instr(Instruction* inst);
    Instruction* getInstruction(size_t index) const;
    bool isPhiInstruction(Instruction* inst);

    std::vector<BasicBlock*> get_preds();
    BasicBlock* get_succs_true();
    BasicBlock* get_succs_false();

    Instruction* get_first_Phi();
    Instruction* get_first_inst();
    Instruction* get_last_inst();
    std::size_t instructions_amount() const;

    Graph* get_graph();

    void set_graph(Graph* other);
    void push_preds_back(BasicBlock* el);
    void add_succs_false(BasicBlock* el);
    void add_succs_true(BasicBlock* el);

    void set_id(std::size_t id);
    std::size_t get_id();
    bool is_root();

    void set_dfs_marker(bool value);
    void set_rpo_marker(bool value);
    void set_loop_gray_marker(bool value);
    void set_loop_black_marker(bool value);
    bool is_dfs_marker();
    bool is_rpo_marker();
    bool is_loop_gray_marker();
    bool is_loop_black_marker();

 private:
    std::vector<BasicBlock*> _preds; // many!

    BasicBlock* _succs_true = nullptr;  // right
    BasicBlock* _succs_false = nullptr; // left 

    Instruction* _first_Phi = nullptr;
    Instruction* _first_inst = nullptr;
    Instruction* _last_inst = nullptr;

    Graph* _graph = nullptr;

    BasicBlockMarker _markers;

    std::size_t _basic_block_id = -1;
};

} // namespace custom

#endif // INCLUDES_BASIC_BLOCK_HPP_
