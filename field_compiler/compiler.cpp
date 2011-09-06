#include<string>
#include<cstdio>
#include<iostream>

#include "FieldReader.h"

#define BRAIN "./Resources/dti_axial_6dir_dti_tensor"
#define BRAIN_CTF "./Resources/dti_axial_6dir_dti_tensor.ctf"
#define DHELIX "./Resources/dhelix_dti_tensor"
#define DHELIX_CTF "./Resources/dhelix_dti_tensor.ctf"

int main(int argc, char * argv[]) {

    std::string input_name;
    std::string output_name;

    int option;
    bool option_chosen = false;
    
    switch(argc) {
        case 1:
            while(!option_chosen) {
                std::cout << "Choose the field to be compiled:" << std::endl
                      << " (1) - Double helix" << std::endl 
                      << " (2) - Brain" << std::endl << std::endl;
                scanf("%d", &option);
                option_chosen = true;
                switch(option) {
                    case 1:
                        input_name = DHELIX;
                        output_name = DHELIX_CTF;
                        break;
                    case 2:
                        input_name = BRAIN;
                        output_name = BRAIN_CTF;
                        break;
                    default:
                        option_chosen = false;
                        std::cout << "Invalid option. Choose either 1 or 2." << std::endl;
                        break;
                }
            }
            break;    
        case 2:
            input_name = argv[1];
            output_name = std::string(argv[1]) + std::string(".ctf");
            break;
        case 3:
            input_name = argv[1];
            output_name = argv[2];
            break;
        default:
            return 1;
    }

    AnalyzeReader reader;
    reader.loadField(input_name);
    reader.generateField(output_name);

    std::cout << output_name << " compilation complete" << std::endl;

    return 0;
}