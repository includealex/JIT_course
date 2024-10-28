#ifndef ICNLUDES_LOOP_HPP_
#define ICNLUDES_LOOP_HPP_

#include "basic_block.hpp"

#include <set>

namespace custom {

struct Loop final {
 public:
    Loop(BasicBlock* header) : _header(header) {}
    
    void set_header(BasicBlock* header) {_header = header;}
    void add_latch(BasicBlock* latch) {_latches.push_back(latch);}
    
    BasicBlock* get_header() { return _header; }
    std::vector<BasicBlock*> get_latches() { return _latches; }
    std::set<BasicBlock*> get_blocks() const { return _blocks; }
    void add_block(BasicBlock* block) { _blocks.insert(block);}

    bool is_irreducible() const { return _irreducible; }
    void set_irreducible(bool value) { _irreducible = value; }

 private:
    BasicBlock* _header = nullptr;
    std::vector<BasicBlock*> _latches;
    std::set<BasicBlock*> _blocks;
    bool _irreducible = false;
};

} // namespace custom

#endif //ICNLUDES_LOOP_HPP_