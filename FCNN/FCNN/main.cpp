#include"FCNN.h"
#include<iostream>

int main(){
	int TrainTimes;
	int RandID;
	double Cost, CostSum = 0.0;
	LoadData();
	LoadStatus();
	std::cout << "Input operation count: ";
	std::cin >> TrainTimes;
	for (int i = 1; i <= TrainTimes; i++) {
		RandID = (int)(RandFloat() * MaxData);
		Cost = WorkOnce(RandID);
		CostSum += Cost * Cost;
		Evolve(-Cost);
		if (i % 10 == 0) {
			std::cout << "Operation Complement: " << i << " of " << TrainTimes << std::endl;
			std::cout << "Present Cost: " << CostSum << std::endl;
			CostSum = 0.0;
		}
	}
	SaveStatus();
	return 0;
}
