#include <iostream>
#include <fstream>

using namespace std;

void printInstructions () {
    std::ifstream f("instructions.txt");

    if (f.is_open())
        cout << f.rdbuf();
}

