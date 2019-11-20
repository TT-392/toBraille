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

    ////////////////////////////////
    ////     error handling     ////
    ////////////////////////////////

    //test if syntax is correct
    if (!argv[3] || argv[4]) { 
        printInstructions ();
        return 0;
    }
        
    //check if file exits
    if (!input.good()) { 
        wcout << argv[1] << ": No such file or directory" << endl;
        return 0;
    }
    
    //check if file is a directory
    struct stat pathStatus; stat(argv[1], &pathStatus);
    bool isDir = !S_ISREG(pathStatus.st_mode);
    if (isDir) {
        cout << argv[1] << ": Is a directory" << endl;
        return 0;
    }

    ////////////////////////////////
    //// end of error handling  ////
    ////////////////////////////////

    int width = stoi(argv[2]);
    int height = stoi(argv[3]);
    wcout << width << " " << height << endl;
    wcout << argv[1] << endl;

    int bytes = height * ((width + 7) / 8);
    char buffer[bytes];
    input.read(buffer, bytes);

    //print 1 char/pixel for testing
  /*  for (int i = 0; i < bytes; i++) {
        unsigned char ch = buffer[i];
        for (int l = 0; l < 8; l++) {
            //wcout << (ch & (1 << l));
            wcout << (((bool)(buffer[i] & (1 << l))) ? wchar_t(0x25cf) : wchar_t(32) << " ";
        }
        if (i % 7 == 6) wcout << endl;
    }
    wcout << endl; */
    //end testing

    wchar_t brailLine[(width + 1)/2]; //create array of width/2 rounded up chars

    for (int yOffset = 0; yOffset < (height + 3)/4; yOffset++) {
        for (int i = 0; i < (width + 1)/2; i++) brailLine[i] = 0x2800; //clear 

        //for the next 4 lines of pixels
        for (int y = yOffset * 4; y < 4 + yOffset * 4; y++) {   //for the next 4 line
            for (int x = 0; x < (width+1)/2; x++) { //for every second pixel
                int byte = x / 4 + y * ((width+7)/8);
                if (byte < bytes) {
                    brailLine[x] |= brailread(y - yOffset * 4, (buffer[byte] & (3 << ((x % 4) * 2))) >> ((x % 4) * 2));
                }
            }
        }
        for (int i = 0; i < (width + 1) / 2; i++) {
            wcout << brailLine[i];
        }
        wcout << endl;
            
    }

}

wchar_t brailread (int row, int bin) {  //return the braille character with bin represented in binary on row row
    int start = 0x2800;                 //first unicode braille character (all dots empty)
    if (row == 3) {
        return (start + (bin << 6));    //last 2 bits in unicode braille are normal left to right
    } else {
        int x = 0;
        if (bin & 1) x |= 1 << row;
        if (bin & 2) x |= 1 << (row + 3);
        return (x + start);
    }
}
