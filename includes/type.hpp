#ifndef INCLUDES_TYPE_HPP_
#define INCLUDES_TYPE_HPP_

#include <limits>

namespace custom {

enum class Type {
  myu32 = 0,
  myu64 = 1,
  myvoid = 2,
  myfloat = 3,
  mydouble = 4,
  myint32 = 5,
  myint64 = 6,
  mybool = 7,
};

using ImmType = uint32_t;
const ImmType IMMPOISON = std::numeric_limits<ImmType>::max();

}  // namespace custom

#endif  // INCLUDES_TYPE_HPP_