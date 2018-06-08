#include <fstream>
#include <iostream>
#include <string>
#include "instructions.h" 
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>

using namespace std; 
wchar_t brailread (int row, int bin);

int main(int argc, char* argv[]) {
    setlocale(LC_CTYPE,"");
    ifstream input(argv[1], ios::binary);

    char buffer[100];
    input.read(buffer, 100);
    int width = ((unsigned char)buffer[18] << 8) + (unsigned char)buffer[19];
    int height = ((unsigned char)buffer[22] << 8) + (unsigned char)buffer[23];
    cout << width << endl;
    cout << height << endl;
}
