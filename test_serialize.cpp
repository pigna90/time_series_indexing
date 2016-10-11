#include "header.hpp"

int main(){
	d_structure data= populateDataStructure(SMALL_DATA);
	save_data(data,"/tmp/data.bin");
}
