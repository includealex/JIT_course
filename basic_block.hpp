#ifndef BASIC_BLOCK_HPP_
#define BASIC_BLOCK_HPP_

#include <cstddef>
#include <vector>

namespace custom {

class Instruction;
class Graph;

class BasicBlock final {
 public:
    BasicBlock(Graph* graph)
        : _first_Phi(nullptr), _first_inst(nullptr), _last_inst(nullptr), _graph(graph) { }

    void pushback_instr(Instruction* inst);
    size_t getInstructionCount() const;
    Instruction* getInstruction(size_t index) const;
    bool isPhiInstruction(Instruction* inst);

    std::vector<BasicBlock*> get_preds();
    std::vector<BasicBlock*> get_succs();
    BasicBlock* get_succs(std::size_t idx);
    std::vector<Instruction*> get_instructions();
    Instruction* get_first_Phi();
    Instruction* get_first_inst();
    Instruction* get_last_inst();
    Graph* get_graph();

    void set_graph(Graph* other);
    void push_preds_back(BasicBlock* el);
    void push_succs_back(BasicBlock* el);

 private:
    std::vector<BasicBlock*> _preds; // many!
    std::vector<BasicBlock*> _succs; // <= 2

    std::vector<Instruction*> _instructions;

    Instruction* _first_Phi;
    Instruction* _first_inst;
    Instruction* _last_inst;

    Graph* _graph;
};

} // namespace custom

#endif // BASIC_BLOCK_HPP_

