#include"FCNN.h"
#include<fstream>
#include<cstring>
#include<iomanip>
#include<iostream>
#include<random>
#include<cmath>

double WorkOnce(int DataID) {
	val[0][0] = Database[DataID].ClockTime;
	val[0][1] = Database[DataID].Temprature;
	val[0][2] = Database[DataID].WindSpeed;
	val[0][3] = Database[DataID].Rainfall;
	val[0][4] = Database[DataID].TimeLen;
	for (int i = 1; i < MaxLayer; i++)
		for (int j = 0; j < MaxNode[i]; j++) {
			val[i][j] = bias[i][j];
			for (int k = 0; k < MaxNode[i - 1]; k++)
				val[i][j] += val[i - 1][k] * weight[i - 1][k][j];
			val[i][j] = sigmoid(val[i][j]);
		}
	return val[MaxLayer - 1][0] - Database[DataID].Answer;
}
void Evolve(double Cost) {
	memset(delta, 0, sizeof(delta));
	delta[MaxLayer - 1][0] = Cost * val[MaxLayer - 1][0] * (1 - val[MaxLayer - 1][0]);
	for (int i = MaxLayer - 1; i > 0; i--) {
		for (int j = 0; j < MaxNode[i]; j++) {
			bias[i][j] += delta[i][j] * EvolveRate2;
			for (int k = 0; k < MaxNode[i - 1]; k++) {
				weight[i - 1][k][j] += EvolveRate1 * delta[i][j] * val[i - 1][k];
				delta[i - 1][k] += delta[i][j] * weight[i - 1][k][j] * val[i - 1][k] * (1 - val[i - 1][k]);
			}
		}
	}
}

void Initiallize() {
	srand(time(0));
	for (int i = 0; i < MaxLayer; i++) {
		for (int j = 0; j < MaxNode[i]; j++) {
			bias[i][j] = 20.0 * (RandFloat() - 0.5);
			if (i < MaxLayer - 1)
				for (int k = 0; k < MaxNode[i + 1]; k++)
					weight[i][j][k] = 2.0 * (RandFloat() - 0.5);
		}
	}
	SaveStatus();
}
void LoadData() {
	std::ifstream ifs(DATAFILE);
	int bufint, buflen = 1;
	double bufdouble;
	ifs >> DataCount;
	Database[0].Answer = 0;
	for (int i = 1; i <= DataCount; i++) {
		ifs >> bufint;
		Database[i].ClockTime = ((bufint / 100) * 1.0 + (bufint % 100) / 60.0) / 12.0;
		ifs >> bufdouble;
		Database[i].Temprature = sigmoid((bufdouble - 15.0) / 15.0);
		ifs >> bufdouble;
		Database[i].WindDirection = std::fabs(bufdouble - 180.0) / 180.0;
		ifs >> bufdouble;
		Database[i].WindSpeed = sigmoid((bufdouble - 3.0) / 1.5);
		ifs >> bufdouble;
		Database[i].Rainfall = bufdouble > 0.5 ? 1 : 0;
		ifs >> bufint;
		Database[i].Answer = bufint;
		Database[i].TimeLen = sigmoid((buflen - 24) / 12.0);
		if (bufint == Database[i - 1].Answer) {
			buflen++;
		}
		else {
			buflen = 1;
		}
	}
	ifs.close();
	std::cout << "Data Load Completed!" << std::endl;
}
void LoadStatus() {
	std::ifstream ifs(STATUSFILE);
	for (int i = 0; i < MaxLayer - 1; i++)
		for (int j = 0; j < MaxNode[i]; j++)
			for (int k = 0; k < MaxNode[i + 1]; k++)
				ifs >> weight[i][j][k];
	for (int i = 0; i < MaxLayer; i++)
		for (int j = 0; j < MaxNode[i]; j++)
			ifs >> bias[i][j];
	ifs.close();
	std::cout << "Status Load Completed!" << std::endl;
}
void SaveStatus() {
	std::cout << "Saving... DO NOT QUIT!" << std::endl;
	std::ofstream ofs(STATUSFILE);
	for (int i = 0; i < MaxLayer - 1; i++)
		for (int j = 0; j < MaxNode[i]; j++)
			for (int k = 0; k < MaxNode[i + 1]; k++)
				ofs << std::setprecision(4) << weight[i][j][k];
	ofs << std::endl << std::endl;
	for (int i = 0; i < MaxLayer; i++)
		for (int j = 0; j < MaxNode[i]; j++)
			ofs << std::setprecision(4) << bias[i][j];
	ofs.close();
	std::cout << "Status Save Completed!" << std::endl;
}
