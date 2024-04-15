#!/bin/bash

echo -e "\033[0mStarting Linear Algebra Shell ...\033[0m"
echo -e "\033[0mChecking System Compatibility and granting permissions ...\033[0m"

chmod +x *.cpp
chmod +x *.hpp
cat << "EOF"

 _ __  _   _ _ __ ___  _ __  _   _        ___ _ __  _ __
| '_ \| | | | '_ ` _ \| '_ \| | | |_____ / __| '_ \| '_ \
| | | | |_| | | | | | | |_) | |_| |_____| (__| |_) | |_) |
|_| |_|\__,_|_| |_| |_| .__/ \__, |      \___| .__/| .__/
                      |_|    |___/           |_|   |_|
                                                                            
An end to end library for Linear Algebra using generic programming concepts
such as Templates, Variadic Templates, Fold Expressions and many more.

License: Apache 2.0.

By Karthik Namboori and Keerthan Gopalkrishnan.
----------------------------------------------------------------------------------------
EOF


g++ --std=c++20 index.cpp # change the file name here if needed.
if [ $? -eq 0 ]; then
  echo -e "\033[32mCompilation successful! Execute './a.out' to run the sample c++ code, i.e index.cpp\033[0m"
  echo -e "\033[34m\nFor documentation, refer 'doc.txt'"
  echo -e "\033[34mRun 'cat doc.txt' for the same.\n"

#  ./a.out # In case you have compiled into another object file using the -o file, this command has to manually changed.
#   echo -e "\033[32mExecution completed.\033[0m"
else
  echo -e "\033[31mCompilation failed. Terminated.\033[0m"
fi


