#ifndef INCLUDES_TYPE_HPP_
#define INCLUDES_TYPE_HPP_

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

using ImmType =
    std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t>;

}  // namespace custom

#endif  // INCLUDES_TYPE_HPP_