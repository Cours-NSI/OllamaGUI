#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>

#define OL_HAS_CONSOLE !OL_DIST

namespace OllamaCore
{
	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		// Create the folder for the logs if it doesn't exits
		std::string logsDirectory = "logs";
		if (!std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> logSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/OLLAMA_GUI.log", true),
#if OL_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		logSinks[0]->set_pattern("[%T] [%l] %n: %v");

#if OL_HAS_CONSOLE
		logSinks[1]->set_pattern("%^[%T] [%l] %n: %v%$");
#endif

		s_Logger = std::make_shared<spdlog::logger>("OLLAMA GUI", logSinks.begin(), logSinks.end());
		s_Logger->set_level(spdlog::level::trace);
	}

	void Log::ShutDown()
	{
		s_Logger.reset();
		spdlog::drop_all();
	}
}