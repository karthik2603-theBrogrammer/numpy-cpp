#include <iostream>
#include <vector>

template <typename T> class MyArray {
private:
  std::vector<int> shape;   // Shape of the array
  std::vector<int> strides; // Strides for efficient element access
  std::vector<T> data;      // Data storage

public:
  // Constructor to initialize the array with a given shape
  MyArray(std::vector<int> shape) : shape(shape) {
    int size = 1;
    for (int dim : shape) {
      size *= dim;
    }
    data.resize(size);
    calculateStrides();
  }

  // Constructor to initialize the array with given shape and data
  template <typename... Args>
  MyArray(std::vector<int> shape, Args... args) : MyArray(shape) {
    if (sizeof...(args) != data.size()) {
      std::cerr << "Error: Number of elements provided does not match the size "
                   "of the array\n";
      return;
    }
    int i = 0;
    initializeElements(i, args...);
  }

  // Function to calculate strides based on the shape
  void calculateStrides() {
    int stride = 1;
    for (int i = shape.size() - 1; i >= 0; --i) {
      strides.insert(strides.begin(), stride);
      stride *= shape[i];
    }
  }

  // Function to get the index in the 1D data array corresponding to given
  // multidimensional indices
  int getIndex(std::vector<int> indices) {
    int index = 0;
    for (int i = 0; i < shape.size(); ++i) {
      index += indices[i] * strides[i];
    }
    return index;
  }

  // Function to access elements using multidimensional indices
  T &operator()(std::vector<int> indices) { return data[getIndex(indices)]; }

  // Overload the + operator for addition
  MyArray<T> operator+(const MyArray<T> &other) const {
    if (shape != other.shape) {
      std::cerr << "Error: Arrays have different shapes\n";
      return MyArray<T>(shape); // Return empty array
    }

    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
      result.data[i] = data[i] + other.data[i];
    }
    return result;
  }

private:
  // Helper function to initialize elements of the array
  template <typename Arg, typename... Args>
  void initializeElements(int &index, Arg value, Args... args) {
    data[index++] = value;
    initializeElements(index, args...);
  }

  // Base case for recursive initialization
  void initializeElements(int &) {}
};

int main() {
  // Declare and initialize the array in one line
  MyArray<int> arr({2, 3}, 1, 2, 3, 4, 5, 6);
  MyArray<int> arr2({2, 3}, 1, 2, 3, 4, 5, 5);
  MyArray<int> arr3 = arr + arr2;

  // Access elements using multidimensional indices
  std::cout << "Element at (1, 2): " << arr({1, 2}) << "\n";

  return 0;
}
