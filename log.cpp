#include "Log.h"

void Log::log(LogType t, const std::string& msg) {
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm;
	localtime_s(&local_tm, &now_time);

	std::ofstream log_file("game.log", std::ios_base::app);

	const std::string type = t == LogType::INFO ? "INFO" : t == LogType::WARNING ? "WARNING" : "ERROR";
	std::ostringstream line;
	line << "[" << type << "] "
		<< std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S")
		<< " " << msg;
	const std::string text = line.str();

	log_file << text << std::endl;
	std::cout << text << std::endl;

	log_file.close();
}