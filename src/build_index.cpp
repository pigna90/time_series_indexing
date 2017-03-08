#pragma once

#include "Baseline.hpp"
#include "Index1.hpp"

#include <iostream>
#include <string.h>
#include <chrono>
#include <ctime>

template <typename T>
void build_index(const std::string &id, const std::string &ds_filename){
	auto begin = std::chrono::high_resolution_clock::now();
	T b(ds_filename);
	b.serialize_data(ds_filename + "." + id);
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Construction time: ";
	std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns";
	std::cout << " ("<<std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " s)" << std::endl;
	std::cout << "Memory size: ";
	std::cout << b.size() << " byte" << std::endl;
}

int main(int argc, char const *argv[]) {
    char const *id = argv[1];
    std::string ds_filename = argv[2];

    switch (atoi(id)) {
        case 0:{
            build_index<Baseline> (id, ds_filename);
        }break;
        case 1:{
			build_index<Index1> (id, ds_filename);
		}break;
    }
    return 0;
}
