#include "array2d.h"

#include <cstddef>
#include <functional>
#include <iostream>
#include <fstream>

template <class T>
array2D::Array2D<T>::Array2D(std::size_t length, std::size_t height,
                             T default_value)
    : length_(length), height_(height) {
  // Initialize empty, '0' doesn't appear
  vec_ = std::vector<T>(length_ * height_, default_value);
}

template <class T> T &array2D::Array2D<T>::at(std::size_t x, std::size_t y) {
  return vec_.at(y * length_ + x);
}
template <class T> const T & array2D::Array2D<T>::cat(std::size_t x, std::size_t y) const {
  return vec_[y * length_ + x];
}
template <class T> const std::size_t &array2D::Array2D<T>::height() const {
  return height_;
}
template <class T> const std::size_t &array2D::Array2D<T>::length() const {
  return length_;
}

template <class T> void array2D::print_array(const Array2D<T> &arr, std::function<char(T)> to_char){
    for (int y = 0; y < arr.height(); ++y) {
    for (int x = 0; x < arr.length(); ++x) {
      std::cout << to_char(arr.cat(x, y));
    }
    std::cout << "\n";
  }
}

template <class T> array2D::Array2D<T> array2D::read_array(std::string file_location, std::function<T(char)> convert) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::vector<T> elements {};
  std::string line;
  int length = 0, height = 0;
  while (std::getline(input_file, line) && line != "") {
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
template class array2D::Array2D<char>;
template class array2D::Array2D<int>;

template void array2D::print_array(const Array2D<char> &,std::function<char(char)> func);
template void array2D::print_array(const Array2D<int> &, std::function<char(int)> func);

template array2D::Array2D<char> array2D::read_array(std::string, std::function<char(char)>);
template array2D::Array2D<int> array2D::read_array(std::string, std::function<int(char)>);