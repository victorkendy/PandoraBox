#include<string>
#include<iostream>

#include "FieldReader.h"

int main(int argc, char * argv[]) {
    if(argc < 3) {
        return 1;
    }

    std::string input_name = argv[1];
    std::string output_name = argv[2];

    AnalyzeReader reader;
    reader.loadField(input_name);
    reader.generateField(output_name);

    return 0;
}