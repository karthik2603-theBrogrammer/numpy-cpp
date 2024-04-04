#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

/*
----------------------------------------------------------------------------------------
 _   _ _   _ __  __ ______   __     ____ ____  ____
| \ | | | | |  \/  |  _ \ \ / /    / ___|  _ \|  _ \
|  \| | | | | |\/| | |_) \ V /____| |   | |_) | |_) |
| |\  | |_| | |  | |  __/ | |_____| |___|  __/|  __/
|_| \_|\___/|_|  |_|_|    |_|      \____|_|   |_|

An end to end, shell and API set for Linear Algebra using generic programming concepts
such as Templates, Variadic Templates, Fold Expressions and many more.

License: Apache 2.0.

By Karthik Namboori and Keerthan Gopalkrishnan.


Requirements:
1. Equal and active participation from the team members (done)
2. Variadic templates                                   (done)
3. Template specialization
4. Lambda templates
5. Fold expression                                      (done)
6. Use of relevant type traits
7. Template friendship
8. Use of relevant concepts and constraints
9. Minimum (or no) use of STL                            (done)
10. Well commented code                                 (done)
----------------------------------------------------------------------------------------
*/

using namespace std;
template <typename T>
class NdArray
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
    void showDim() const
    {
        cout << "\033[36m"
             << "\nShape{";
        for (auto d : shape)
        {
            cout << d << ",";
        }
        cout << "}" << endl;
        cout << "\033[32mEnd." << endl;
    }
    void show() const
    {

        for (int i = 0; i < data.size(); ++i)
        {
            cout << data[i] << " ";
        }
        this->showDim();
    }

    std::vector<int> shape;
    // Constructor to initialize the array with a given shape
    NdArray(std::vector<int> shape) : shape(shape)
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
    NdArray(std::vector<int> shape, Args... args) : NdArray(shape)
    {
        if (sizeof...(args) != data.size())
        {
            std::cerr << "\033[31mError: Number of elements provided does not match the size "
                         "of the array\n";
            exit(1);
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
    // int getIndex(std::vector<int> indices)
    // // {
    // //     int index = 0;
    // //     ((index += indices[i] * strides[i]), ...);
    // //     return index;
    // // }

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
    NdArray<T> operator+(const NdArray<T> &other) const
    {
        if (shape != other.shape)
        {
            std::cerr << "Error: Arrays have different shapes\n";
            return NdArray<T>(shape); // Return empty array
        }

        NdArray<T> result(shape);
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
    T dot(const NdArray<T> &other) const
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
    NdArray<T> z()
    {
        int numZeroes = 1;
        for (auto e : shape)
            numZeroes *= e;
        NdArray<T> res(shape);
        for (int i = 0; i < numZeroes; i++)
        {
            res.data.push_back(static_cast<T>(0));
        }

        return res;
    }
    NdArray<T> operator-(const NdArray<T> &other) const
    {
        cout << shape << " And the other vectors shape: " << other.shape << endl;
        if (shape != other.shape)
        {
            cerr << "Error: Arrays have different shapes, Aborting ...\n";
            return NdArray<T>(shape);
        }

        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }
    NdArray<T> operator*(const NdArray<T> &other) const
    {
        if (shape != other.shape)
        {
            std::cerr << "Error: Arrays have different shapes\n";
            return NdArray<T>(shape); // Return empty array
        }

        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = data[i] * other.data[i];
        }
        return result;
    }
    NdArray<T> operator/(const NdArray<T> &other) const
    {
        if (shape != other.shape)
        {
            std::cerr << "Error: Arrays have different shapes\n";
            return NdArray<T>(shape); // Return empty array
        }

        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            if (other.data[i] == 0)
            {
                std::cerr << "Error: Division by zero encountered\n";
                return NdArray<T>(shape); // Return empty array
            }
            result.data[i] = data[i] / other.data[i];
        }
        return result;
    }
    NdArray<T> pow(const NdArray<T> &other) const
    {
        if (shape != other.shape)
        {
            std::cerr << "Error: Arrays have different shapes\n";
            return NdArray<T>(shape); // Return empty array
        }

        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::pow(data[i], other.data[i]);
        }
        return result;
    }
    NdArray<T> exp() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::exp(data[i]);
        }
        return result;
    }
    NdArray<T> sqrt() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::sqrt(data[i]);
        }
        return result;
    }
    NdArray<T> abs() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::abs(data[i]);
        }
        return result;
    }
    NdArray<T> sin() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::sin(data[i]);
        }
        return result;
    }
    NdArray<T> cos() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::cos(data[i]);
        }
        return result;
    }
    NdArray<T> log() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::log(data[i]);
        }
        return result;
    }
    NdArray<T> log10() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::log10(data[i]);
        }
        return result;
    }
    NdArray<T> floor() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::floor(data[i]);
        }
        return result;
    }
    NdArray<T> ceil() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
            result.data[i] = std::ceil(data[i]);
        }
        return result;
    }
    NdArray<T> round() const
    {
        NdArray<T> result(shape);
        for (int i = 0; i < data.size(); ++i)
        {
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