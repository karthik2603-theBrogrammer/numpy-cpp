#include <iostream>
#include "NdArray.hpp"

using namespace std;

int main()
{
  // Declare and initialize the array in one line
  NdArray<double> arr({2, 3}, 1.5, 2, 3, 4, 5, 6);
  arr.show();
  NdArray<double> arr2({2, 3}, 1, 2, 3, 4, 5, 9);
  arr2.show();
  auto result = NdArrayOp::add(arr, arr2);
  result.show();
  result = NdArrayOp::sub(arr, arr2);
  result.show();
  result = NdArrayOp::mul(arr, arr2);
  result.show();
  result = NdArrayOp::div(arr, arr2);
  result.show();
  result = NdArrayOp::pow(arr, arr2);
  result.show();
  cout << NdArrayOp::dot(arr, arr2) << endl;

  NdArray<double> ar({3, 3});

    // Call the z() function to initialize the array with zeros
  NdArray<double> zeroArray = ar.z();

    // Display the array
    zeroArray.show();


 
  // NdArray<double> md({3, 3, 3}, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  // cout << md.dot(md) << endl;

  // NdArray<int> z({2,3});
  // z = z.z();
  // z.show();

  // NdArray<int> arr({10, 10});
  // NdArray<int> arr2({10, 10});
  // // Initialize the arrays with random values for demonstration
  // for (int i = 0; i < 100; ++i)
  // {
  //   for (int j = 0; j < 100; j++)
  //   {
  //     arr({i, j}) = rand() % 10;
  //     arr2({i, j}) = rand() % 10;
  //   }
  // }
  // NdArray<double> arr3 = md / md;

  return 0;
}
