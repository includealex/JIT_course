#ifndef INSTRUCTION_HPP_
#define INSTRUCTION_HPP_

#include <cstddef>
#include <vector>
#include <string>

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
    u32 = 0,
    u64 = 1
};

class Instruction final {
 public:
    Instruction(Opcode opcode, Type type, BasicBlock* basicBlock)
        : _prev(nullptr), _next(nullptr), _opcode(opcode), _type(type), _basic_block(basicBlock) { }

    Opcode getOpcode() const {
        return _opcode;
    }

    // Intrusive linked list
    Instruction* _prev;
    Instruction* _next;

    // Common part
    Opcode _opcode;
    Type _type;
    BasicBlock* _basic_block;
};

} // namespace custom

#endif // INSTRUCTION_HPP_

