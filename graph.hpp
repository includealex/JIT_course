#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>

namespace custom {

class BasicBlock;

class Graph final {
 public:
    void addBasicBlock(BasicBlock* block);

    // Public members as per your test code
    std::vector<BasicBlock*> _blocks;
};

} // namespace custom

#endif // GRAPH_HPP_

