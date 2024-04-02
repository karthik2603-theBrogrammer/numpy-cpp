#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
template <typename T>
class MyArray
{
private:
     // Shape of the array
  std::vector<int> strides; // Strides for efficient element access
  std::vector<T> data;      // Data storage
  // unordered_map<string, string> colorMap;
  // colorMap["default"] = "\033[0m";
  //   colorMap["black"] = "\033[30m";
  //   colorMap["red"] = "\033[31m";
  //   colorMap["green"] = "\033[32m";
  //   colorMap["yellow"] = "\033[33m";
  //   colorMap["blue"] = "\033[34m";
  //   colorMap["magenta"] = "\033[35m";
  //   colorMap["cyan"] = "\033[36m";
  //   colorMap["white"] = "\033[37m";
  

public:
void showDim() const {
  cout << "\033[36m" <<  "\nShape{";
    for(auto d: shape) {
      cout << d << ",";
    }
    cout << "}" << endl;
  cout << "\033[32mEnd." << endl; 
}
void show() const {

    for (int i = 0; i < data.size(); ++i) {
        cout << data[i] << " ";
    }
    this->showDim();
    
}

std::vector<int> shape;
  // Constructor to initialize the array with a given shape
  MyArray(std::vector<int> shape) : shape(shape)
  {
    int size = 1;
    for (int dim : shape)
    {
      size *= dim;
    }
    data.resize(size);
    calculateStrides();
  }

  // Constructor to initialize the array with given shape and data
  template <typename... Args>
  MyArray(std::vector<int> shape, Args... args) : MyArray(shape)
  {
    if (sizeof...(args) != data.size())
    {
      std::cerr << "Error: Number of elements provided does not match the size "
                   "of the array\n";
      return;
    }
    int i = 0;
    initializeElements(i, args...);
  }

  // Function to calculate strides based on the shape
  void calculateStrides()
  {
    int stride = 1;
    for (int i = shape.size() - 1; i >= 0; --i)
    {
      strides.insert(strides.begin(), stride);
      stride *= shape[i];
    }
  }

  // Function to get the index in the 1D data array corresponding to given
  // multidimensional indices
  int getIndex(std::vector<int> indices)
  {
    int index = 0;
    for (int i = 0; i < shape.size(); ++i)
    {
      index += indices[i] * strides[i];
    }
    return index;
  }

  // Function to access elements using multidimensional indices
  T &operator()(std::vector<int> indices) { return data[getIndex(indices)]; }

  // Overload the + operator for addition
  MyArray<T> operator+(const MyArray<T> &other) const
  {
    if (shape != other.shape)
    {
      std::cerr << "Error: Arrays have different shapes\n";
      return MyArray<T>(shape); // Return empty array
    }

    MyArray<T> result(shape);
    // for (int i = 0; i < data.size(); ++i) {
    //   result.data[i] = data[i] + other.data[i];
    // }
    // return result;
    auto start = std::chrono::steady_clock::now();
    bool use_parallel = false;
    if (use_parallel)
    {
      std::vector<std::thread> threads;
      threads.reserve(data.size());
      for (int i = 0; i < data.size(); ++i)
      {
        threads.emplace_back([&result, this, &other, i]()
                             { result.data[i] = this->data[i] + other.data[i]; });
      }
      for (auto &thread : threads)
      {
        thread.join();
      }
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        result.data[i] = data[i] + other.data[i];
      }
    }
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;

    // Convert duration to milliseconds and output
    std::cout << "Time taken: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;

    return result;
  }
  T dot(const MyArray<T> &other) const
  {
    if (shape != other.shape)
    {
      std::cerr << "Error: Arrays have different shapes\n";
      return T(); // Return default value for type T
    }

    T result = 0;
    for (int i = 0; i < data.size(); ++i)
    {
      result += data[i] * other.data[i];
    }
    return result;
  }
  MyArray<T> z(){
    int numZeroes = 1;
    for(auto e: shape) numZeroes *= e;
    MyArray<T> res(shape);
    for(int i = 0; i < numZeroes; i++){
      res.data.push_back(static_cast<T>(0));
    }

    return res;
  }
  MyArray<T> operator-(const MyArray<T> &other) const
  {
    cout << shape << " And the other vectors shape: " << other.shape << endl;
    if (shape != other.shape)
    {
      cerr << "Error: Arrays have different shapes, Aborting ...\n";
      return MyArray<T>(shape);
    }

    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i)
    {
      result.data[i] = data[i] - other.data[i];
    }
    return result;
  }
  MyArray<T> operator*(const MyArray<T>& other) const {
    if (shape != other.shape) {
        std::cerr << "Error: Arrays have different shapes\n";
        return MyArray<T>(shape); // Return empty array
    }

    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] * other.data[i];
    }
    return result;
}
  MyArray<T> operator/(const MyArray<T>& other) const {
    if (shape != other.shape) {
        std::cerr << "Error: Arrays have different shapes\n";
        return MyArray<T>(shape); // Return empty array
    }

    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        if (other.data[i] == 0) {
            std::cerr << "Error: Division by zero encountered\n";
            return MyArray<T>(shape); // Return empty array
        }
        result.data[i] = data[i] / other.data[i];
    }
    return result;
}
  MyArray<T> pow(const MyArray<T>& other) const {
    if (shape != other.shape) {
        std::cerr << "Error: Arrays have different shapes\n";
        return MyArray<T>(shape); // Return empty array
    }

    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = std::pow(data[i], other.data[i]);
    }
    return result;
}
  MyArray<T> exp() const {
    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = std::exp(data[i]);
    }
    return result;
}
  MyArray<T> sqrt() const { 
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::sqrt(data[i]);
      }
      return result;
  }
  MyArray<T> abs() const {
    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = std::abs(data[i]);
    }
    return result;
}
  MyArray<T> sin() const {
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::sin(data[i]);
      }
      return result;
  }
  MyArray<T> cos() const {
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::cos(data[i]);
      }
      return result;
  }
  MyArray<T> log() const {
    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = std::log(data[i]);
    }
    return result;
}
  MyArray<T> log10() const {
    MyArray<T> result(shape);
    for (int i = 0; i < data.size(); ++i) {
        result.data[i] = std::log10(data[i]);
    }
    return result;
}
  MyArray<T> floor() const {
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::floor(data[i]);
      }
      return result;
  }
  MyArray<T> ceil() const {
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::ceil(data[i]);
      }
      return result;
  }
  MyArray<T> round() const {
      MyArray<T> result(shape);
      for (int i = 0; i < data.size(); ++i) {
          result.data[i] = std::round(data[i]);
      }
      return result;
  }
private:
  // Helper function to initialize elements of the array
  template <typename Arg, typename... Args>
  void initializeElements(int &index, Arg value, Args... args)
  {
    data[index++] = value;
    initializeElements(index, args...);
  }

  // Base case for recursive initialization
  void initializeElements(int &) {}
};

int main()
{
  // Declare and initialize the array in one line
  MyArray<double> arr({2, 3}, 1, 2, 3, 4, 5, 6);
  MyArray<double> arr2({2, 3}, 1, 2, 3, 4, 5, 5);

  MyArray<double> md({3, 3, 3}, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  cout << md.dot(md) << endl;

  MyArray<int> z({2,3});
  z = z.z();
  z.show();

  // MyArray<int> arr({10, 10});
  // MyArray<int> arr2({10, 10});
  // // Initialize the arrays with random values for demonstration
  // for (int i = 0; i < 100; ++i)
  // {
  //   for (int j = 0; j < 100; j++)
  //   {
  //     arr({i, j}) = rand() % 10;
  //     arr2({i, j}) = rand() % 10;
  //   }
  // }
  MyArray<double> arr3 = md / md;



  return 0;
}
