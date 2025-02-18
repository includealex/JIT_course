#ifndef INCLUDES_LINORDER_HPP_
#define INCLUDES_LINORDER_HPP_

#include <bits/stdc++.h>

#include "domin_tree.hpp"
#include "loop_tree.hpp"

namespace custom {

class Linorder final {
 public:
  std::vector<size_t> get_linorder(Graph* graph);
  std::vector<size_t> get_reversed_linorder(Graph* graph);

 private:
  void fill_linorder(Graph* graph);
  std::vector<size_t> _linorder;
};

}  // namespace custom

#endif  // INCLUDES_LINORDER_HPP_
