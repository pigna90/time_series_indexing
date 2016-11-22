#pragma once

#include "Baseline.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[]) {
    if(argc == 4){
        char const *id = argv[1];
        std::string ds_filename = argv[2];
        std::string qs_filename = argv[3];
        switch (atoi(id)) {
            case 0:
                auto begin = std::chrono::high_resolution_clock::now();
                Baseline b;
                b.load_data(ds_filename + "." + id);
                auto end = std::chrono::high_resolution_clock::now();

                std::cout << "Loading time: ";
                std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns";
                std::cout << " ("<<std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " s)" << std::endl;
                std::cout << "Memory size: ";
                std::cout << b.size() << " byte" << std::endl;
            break;
        }
	}
	else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
