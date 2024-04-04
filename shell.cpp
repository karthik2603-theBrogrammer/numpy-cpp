#include <iostream>
#include <vector>
#include <sstream>
#include "NdArray.hpp" // Include your NdArray class implementation

using namespace std;

// Function to parse input string into tokens
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    cout << "Welcome to the NdArray shell! Enter 'exit' to quit." << endl;

    vector<NdArray<double>> arrays; // Store declared arrays

    while (true) {
        cout << "> ";
        string input;
        getline(cin, input);

        if (input == "exit") {
            cout << "Exiting the shell. Goodbye!" << endl;
            break;
        }

        vector<string> tokens = tokenize(input);
        cout << tokens.size() << endl;
        cout << input << endl;
        for(auto e: tokens) cout << e << "-";

        if (tokens.empty()) {
            cout << "Invalid command. Please try again." << endl;
            continue;
        }

        if (tokens[0] == "declare") {
            if (tokens.size() < 3) {
                cout << "Invalid syntax for 'declare' command. Usage: declare <name> <shape> <values>" << endl;
                // command = tokens[0];
                string name = tokens[1];
                vector<int> shape; 
                continue;
            }

        } else if (tokens.size() == 4 && tokens[1] == "add") {



        } else if (tokens.size() == 4 && tokens[1] == "dot") {
            // Perform dot product
            // Example: dot array1 array2 result_scalar
        } else {
            cout << "Invalid command. Please try again." << endl;
        }
    }

    return 0;
}
