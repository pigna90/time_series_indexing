#include "../include/header.hpp"

int main(int argc, char* argv[]){
	if(argc == 3){
		string in_dataset = argv[1];
		string out_bin = argv[2];
		
		d_structure data= populateDataStructure(in_dataset);
		save_data(data,out_bin);
	}
	else
		cout << "Missing parameters" << endl << "Exit." << endl;
}
