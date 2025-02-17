#ifndef INCLUDES_LIVENESS_HPP_
#define INCLUDES_LIVENESS_HPP_

#include "linorder.hpp"

namespace custom {

class Liveness {
 public:
    void run_analysis(Graph* graph);
 private:
    void set_bb_liveranges(Graph* graph);
};

} // namespace custom

#endif //INCLUDES_LIVENESS_HPP_