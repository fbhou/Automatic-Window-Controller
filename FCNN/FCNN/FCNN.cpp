#include"FCNN.h"
#include<fstream>
#include<cstring>
#include<iomanip>
#include<iostream>
#include<random>
#include<cmath>
#include<ctime>

double WorkOnce(WeatherData Data) {
	val[0][0] = Data.ClockTime;
	val[0][1] = Data.Temprature;
	val[0][2] = Data.WindSpeed;
	val[0][3] = Data.Humidity;
	val[0][4] = Data.TimeLen;
	for (int i = 1; i < MaxLayer; i++)
		for (int j = 0; j < MaxNode[i]; j++) {
			val[i][j] = bias[i][j];
			for (int k = 0; k < MaxNode[i - 1]; k++)
				val[i][j] += val[i - 1][k] * weight[i - 1][k][j];
			val[i][j] = sigmoid(val[i][j]);
		}
	return val[MaxLayer - 1][0] - Data.Answer;
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
		if (Database[i].ClockTime > 1) Database[i].ClockTime = 2.0 - Database[i].ClockTime;
		ifs >> bufdouble;
		Database[i].Temprature = sigmoid((bufdouble - 15.0) / 15.0);
		ifs >> bufdouble;
		Database[i].WindDirection = std::fabs(bufdouble - 180.0) / 180.0;
		ifs >> bufdouble;
		Database[i].WindSpeed = sigmoid((bufdouble - 3.0) / 1.5);
		ifs >> bufdouble;
		Database[i].Humidity = sigmoid((bufdouble - 0.80) / 0.10);
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

int Run(int Temprature, int Windspeed, int Winddirection, int Humidity) {
	long long LastTime, NowTime;
	int WindowType, OrderType;
	WeatherData wtd;
	std::ifstream ifs(TIMELOGFILE);
	ifs >> LastTime >> WindowType;
	ifs.close();

	NowTime = time(0);
	wtd.ClockTime = NowTime % 86400 / 43200.0;
	if (wtd.ClockTime > 1) wtd.ClockTime = 2.0 - wtd.ClockTime;
	wtd.Temprature = sigmoid((Temprature - 15.0) / 15.0);
	wtd.Humidity = sigmoid((Humidity - 50.0) / 30.0);
	wtd.WindDirection = std::fabs(Winddirection - 180.0) / 180.0;
	wtd.WindSpeed = sigmoid((Windspeed - 3.0) / 1.5);
	wtd.TimeLen = sigmoid(((NowTime - LastTime) - 43200) / 21600.0);

	OrderType = WorkOnce(wtd) > 0.5 ? 1 : 0;
	std::ofstream ofs(TIMELOGFILE);
	if (WindowType == OrderType) {
		ofs << LastTime << OrderType;
	}
	else {
		ofs << NowTime << OrderType;
	}
	ofs.close();

	return OrderType;
}
void Train(int TrainTimes) {
	int RandID;
	double Cost, CostSum;
	LoadData();
	LoadStatus();
	std::cout << "\tRound\tCost" << std::endl;
	for (int i = 0; i < TrainTimes; i++) {
		RandID = (int)(RandFloat() * MaxData);
		CostSum = 0.0;
		for (int j = 0; j < MaxData; j++) {
			Cost = WorkOnce(Database[(RandID + j) % MaxData]);
			CostSum += Cost * Cost;
			Evolve(-Cost);
		}
		std::cout <<"\t" << i << "\t" << CostSum << std::endl;
	}
	SaveStatus();
}