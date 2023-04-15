#include "infscrpch.h"
#include "logger.h"

namespace infinity {


	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;
	std::shared_ptr<spdlog::logger> Logger::s_FileLogger;

	void Logger::init() { 
		spdlog::set_pattern("%^%n | %T |%$ %v");
		s_CoreLogger = spdlog::stdout_color_mt("INF_CORE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("INFINITY");
		s_AppLogger->set_level(spdlog::level::trace);

		s_FileLogger = spdlog::rotating_logger_mt("LOGGER", "logs/infinity.log", 1048576 * 5, 3);
		s_FileLogger->set_level(spdlog::level::trace);
	}
	std::shared_ptr<spdlog::logger> Logger::getCoreLogger() {
		return Logger::s_CoreLogger;
	}
	std::shared_ptr<spdlog::logger> Logger::getAppLogger() {
		return Logger::s_AppLogger;
	}
	std::shared_ptr<spdlog::logger> Logger::getFileLogger() {
		return Logger::s_FileLogger;
	}
}