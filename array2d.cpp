#include "array2d.h"

#include <cstddef>
#include <functional>
#include <iostream>
#include <fstream>

template <class T>
Array2D::Array2D<T>::Array2D(std::size_t length, std::size_t height,
                             T default_value)
    : length_(length), height_(height) {
  // Initialize empty, '0' doesn't appear
  vec_ = std::vector<T>(length_ * height_, default_value);
}

template <class T> T &Array2D::Array2D<T>::at(std::size_t x, std::size_t y) {
  return vec_.at(y * length_ + x);
}
template <class T> const T &Array2D::Array2D<T>::at(int x, int y) const {
  return vec_[y * length_ + x];
}
template <class T> const std::size_t &Array2D::Array2D<T>::height() const {
  return height_;
}
template <class T> const std::size_t &Array2D::Array2D<T>::length() const {
  return length_;
}

template <class T> void Array2D::print_array(const Array2D<T> &arr){
    for (int y = 0; y < arr.height(); ++y) {
    for (int x = 0; x < arr.length(); ++x) {
      std::cout << arr.at(x, y);
    }
    std::cout << "\n";
  }
}

template <class T> Array2D::Array2D<T> Array2D::read_array(std::string file_location, std::function<T(char)> convert) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::vector<T> elements {};
  std::string line;
  int length = 0, height = 0;
  while (std::getline(input_file, line)) {
    ++height;
    length = line.size();
    for(auto iter = line.begin(); iter != line.end(); ++iter){
        elements.push_back(convert(*iter));
    }
  }
  Array2D<T> array(length, height, elements);
  return array;
}

// Explicit instantiation
template class Array2D::Array2D<char>;
template class Array2D::Array2D<int>;

template void Array2D::print_array(const Array2D<char> &);
template void Array2D::print_array(const Array2D<int> &);

template Array2D::Array2D<char> Array2D::read_array(std::string, std::function<char(char)>);
template Array2D::Array2D<int> Array2D::read_array(std::string, std::function<int(char)>);