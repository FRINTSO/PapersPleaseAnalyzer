#pragma once
#include <memory>

#include <spdlog/spdlog.h>

namespace Utils {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}


#define LOG(...) ::Utils::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_ERR(...) ::Utils::Log::GetLogger()->error(__VA_ARGS__)
#define BeginLOG(message)
#define EndLOG(message)