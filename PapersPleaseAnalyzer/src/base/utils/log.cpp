#include "pch.h"
#include "base/utils/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


namespace Utils {

	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T]: %v%$");
		s_Logger = spdlog::stdout_color_mt("LOGGER");
		s_Logger->set_level(spdlog::level::level_enum::trace);
	}
}