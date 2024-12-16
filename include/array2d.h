#ifndef _ARRAY2D_H_
#define _ARRAY2D_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <ranges>
#include <vector>

namespace array2D {

template <class T> class Array2D {
public:
  Array2D(std::size_t length, std::size_t height, T default_value = {});
  Array2D(std::size_t length, std::size_t height,
          std::ranges::input_range auto &&range) : Array2D(length, height) {
    int idx = 0;
    std::ranges::for_each(range, [&](auto &val) {
      vec_.at(idx) = val;
      ++idx;
    });
  }

  T &at(std::size_t x, std::size_t y);
  const T &cat(std::size_t x, std::size_t y) const;
  const std::size_t &height() const;
  const std::size_t &length() const;

  auto operator<=>(const Array2D<T>&) const = default;

private:
  std::vector<T> vec_;
  const std::size_t length_;
  const std::size_t height_;
};

template <class T>
void print_array(const Array2D<T>&, std::function<char(T)> = [](const auto& v){return v;});

template <class T>
Array2D<T> read_array(std::string, std::function<T(char)>);

} // namespace Array2D

#endif // _ARRAY2D_H_