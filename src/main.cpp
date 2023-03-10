#include <iostream>
#include <thread>
#include "fractales.hpp"
#include "ez-draw++.hpp"
#include <cstring>

int main(int argc, char **argv) {

    int power = 2, max_iterations = 30; // Default values

    if (argc > 1) { // If there are arguments, we change the default values
        if (argc > 2 && strcmp(argv[1],"-p") == 0) power = std::atoi(argv[2]);
        if (argc > 3 && strcmp(argv[3], "-i") == 0) max_iterations = std::atoi(argv[4]);
    }

    // We create the application and execute it
    App myApp(power, max_iterations);
    myApp.mainLoop();

    return 0;
}
