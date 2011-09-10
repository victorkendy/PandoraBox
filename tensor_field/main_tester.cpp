#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "pbge/pbge.h"
#include "CompiledFieldReader.h"
#include "EventHandlers.h"
#include "SceneInitialization.h"

const std::string BRAIN_CTF = "./Resources/dti_axial_6dir_dti_tensor.ctf";
const std::string DHELIX_CTF = "./Resources/dhelix_dti_tensor.ctf";

std::string choose_field(int argc, char ** argv) {
    int option;
    bool option_chosen = false;
    
    switch(argc) {
        case 1:
            while(!option_chosen) {
                std::cout << "Choose the field to be read:" << std::endl
                      << " (1) - Double helix" << std::endl 
                      << " (2) - Brain" << std::endl << std::endl;
                scanf("%d", &option);
                option_chosen = true;
                switch(option) {
                    case 1:
                        return DHELIX_CTF;
                    case 2:
                        return BRAIN_CTF;
                    default:
                        option_chosen = false;
                        std::cout << "Invalid option. Choose either 1 or 2." << std::endl;
                        break;
                }
            }
            break;
        case 2:
            return argv[1];
        default:
            std::cout << "Illegal command line arguments" << std::endl;
            exit(1);
    }
    std::cout << "Illegal command line arguments" << std::endl;
    exit(1);
    return std::string("");
}

int main(int argc, char ** argv) {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(1024, 768);
    manager->setFullscreen(false);
    manager->setWindowTitle("tensor_field");
    manager->setSceneInitializer(new CustomSceneInitializer(choose_field(argc, argv)));
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}