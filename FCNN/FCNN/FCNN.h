#pragma once
#include<iostream>
#include<cmath>
#include<random>

#ifndef FCNN_H
#define FCNN_H

static const int MaxLayer = 4;
static const int MaxMaxNode = 32;
static const int MaxNode[MaxLayer] = { 6,32,16,1 };
static const int MaxData = 100000;
static const double EvolveRate1 = 1e-3, EvolveRate2 = 1e-1;
static std::string STATUSFILE = "status.txt";
static std::string DATAFILE = "data.txt";

static double weight[MaxLayer][MaxMaxNode][MaxMaxNode];//Weight of edge (i,j)->(i+1,k)
static double bias[MaxLayer][MaxMaxNode];//Bias of point (i,j)
static double val[MaxLayer][MaxMaxNode];//
static double delta[MaxLayer][MaxMaxNode];//Error of point (i,j)

static int DataCount;
struct WeatherData {
	double ClockTime, Temprature, WindDirection, WindSpeed, Rainfall, TimeLen;
	int Answer;
};
static WeatherData Database[MaxData];

static std::default_random_engine RandomEngine;
static std::uniform_real_distribution<double> u(0.0, 1.0);
static double RandFloat() {
	return u(RandomEngine);
}

static double sigmoid(double x){
    return 1.0/(1.0+exp(-x));
}

void Initiallize();
void LoadData();
void LoadStatus();
void SaveStatus();

double WorkOnce(int);
void Evolve(double);
#endif
