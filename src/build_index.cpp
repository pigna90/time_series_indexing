#pragma once

#include <iostream>
#include <string.h>
#include <chrono>
#include <ctime>
#include "Baseline.hpp"

int main(int argc, char const *argv[]) {
    char const *id = argv[1];
    std::string ds_filename = argv[2];

    switch (atoi(id)) {
        case 0:
            auto begin = std::chrono::high_resolution_clock::now();
            Baseline b(ds_filename);
            b.serialize_data(ds_filename + "." + id);
            auto end = std::chrono::high_resolution_clock::now();

            std::cout << "Construction time: ";
            std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns";
            std::cout << " ("<<std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " s)" << std::endl;
            std::cout << "Memory size: ";
            std::cout << b.size() << " byte" << std::endl;
        break;
    }
    return 0;
}
