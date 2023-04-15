#pragma once
#include <memory>
#include <string>

#include "spdlog\spdlog.h"
#include "spdlog\sinks\stdout_color_sinks.h"
#include "spdlog\sinks\rotating_file_sink.h"

namespace infinity {

	class Logger {
	public:
		static void init();

		static std::shared_ptr<spdlog::logger> getCoreLogger();
		static std::shared_ptr<spdlog::logger> getAppLogger();
		static std::shared_ptr<spdlog::logger> getFileLogger();

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;
		static std::shared_ptr<spdlog::logger> s_FileLogger;

	};
}

#define INFINITY_CORE_TRACE(...) infinity::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define INFINITY_CORE_INFO(...) infinity::Logger::getCoreLogger()->info(__VA_ARGS__)
#define INFINITY_TRACE(...) infinity::Logger::getAppLogger()->trace(__VA_ARGS__)
#define INFINITY_INFO(...) infinity::Logger::getAppLogger()->info(__VA_ARGS__)

#define INFINITY_CORE_WARN(...) infinity::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define INFINITY_CORE_ERROR(...) infinity::Logger::getCoreLogger()->error(__VA_ARGS__)
#define INFINITY_CORE_CRITICAL(...) infinity::Logger::getCoreLogger()->critical(__VA_ARGS__)

#define INFINITY_WARN(...) infinity::Logger::getAppLogger()->warn(__VA_ARGS__)
#define INFINITY_ERROR(...) infinity::Logger::getAppLogger()->error(__VA_ARGS__)
#define INFINITY_CRITICAL(...) infinity::Logger::getAppLogger()->critical(__VA_ARGS__)

#define INFINITY_LOG(...) infinity::Logger::getFileLogger()->trace(__VA_ARGS__)