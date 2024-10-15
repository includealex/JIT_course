#ifndef INCLUDES_INSTRUCTION_HPP_
#define INCLUDES_INSTRUCTION_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class BasicBlock;

enum class Opcode {
    ADD = 0,
    MUL = 1,
    JMP = 2,
    RET = 3,
    CONST = 4,
    MOV = 5,
    CAST = 6,
    CMP = 7,
    PHI = 8
};

enum class Type {
    myu32 = 0,
    myu64 = 1,
    myvd = 2,
};

class Instruction final {
 public:
    Instruction(Opcode opcode, Type type, BasicBlock* basicBlock, 
                std::size_t destReg = -1, const std::vector<std::size_t>& srcRegs = {})
        : _prev(nullptr), _next(nullptr), _opcode(opcode), _basic_block(basicBlock), 
          _type(type), _destReg(destReg), _srcRegs(srcRegs) { }

    Opcode getOpcode() const {
        return _opcode;
    }

    Type getType() const {
        return _type;
    }

    std::size_t getDestReg() const {
        return _destReg;
    }

    const std::vector<std::size_t>& getSrcRegs() const {
        return _srcRegs;
    }

    void set_basic_block(BasicBlock* other) {
        _basic_block = other;
    }

    void set_next(Instruction* other) {
        _next = other;
    }

    void set_prev(Instruction* other) {
        _prev = other;
    }

    Instruction* get_next() const{
        return _next;
    }

    Instruction* get_prev() const{
        return _prev;
    }

    void set_id(std::size_t id) {
        _instr_id = id;
    }

    std::size_t get_id() {
        return _instr_id;
    }

 private:
    // Intrusive linked list
    Instruction* _prev;
    Instruction* _next;

    // Common part
    Opcode _opcode;
    BasicBlock* _basic_block;
    Type _type;

    // Register information
    std::size_t _destReg;
    std::vector<std::size_t> _srcRegs;

    std::size_t _instr_id;
};

} // namespace custom

#endif // INCLUDES_INSTRUCTION_HPP_

