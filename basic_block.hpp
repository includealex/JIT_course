#ifndef BASIC_BLOCK_HPP_
#define BASIC_BLOCK_HPP_

#include <cstddef>
#include <iostream>
#include <vector>

namespace custom {

class Instruction;
class Graph;

class BasicBlock final {
 public:
    BasicBlock(Graph* graph)
        : _first_Phi(nullptr), _first_inst(nullptr), _last_inst(nullptr), _graph(graph) { }

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

 private:
    std::vector<BasicBlock*> _preds; // many!

    BasicBlock* _succs_true = nullptr;
    BasicBlock* _succs_false = nullptr;

    Instruction* _first_Phi = nullptr;
    Instruction* _first_inst = nullptr;
    Instruction* _last_inst = nullptr;

    Graph* _graph = nullptr;

    std::size_t _basic_block_id;
};

} // namespace custom

#endif // BASIC_BLOCK_HPP_

