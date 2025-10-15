#pragma once
#include<iostream>

#include<string>
#include<fstream>
#include<sstream>

#include<chrono>
#include<ctime>
#include<iomanip>

enum class LogType {
	INFO,
	WARNING,
	ERROR
};

class Log {
public:
	static void log(LogType t, const std::string& msg);
};