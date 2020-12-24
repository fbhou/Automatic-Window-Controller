#include"FCNN.h"
#include<iostream>
#include<string>

int main(int argc, char **argv){
	if (argv[0][0] == 'i') {
		Initiallize();
		return 0;
	}
	if (argv[0][0] == 't') {
		Train(std::stol(argv[1]));
		return 0;
	}
	if (argv[0][0] == 'w') {	
		return Run(std::stol(argv[1]), std::stol(argv[2]), std::stol(argv[3]), std::stol(argv[4]));
	}
	return 0;
}
