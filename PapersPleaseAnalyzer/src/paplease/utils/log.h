#pragma once
#include <memory>

#include <spdlog/spdlog.h>

namespace paplease {
	namespace utils {

		class Log
		{
		public:
			static void Init();

			static inline std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
		private:
			static inline void LogIndent(spdlog::level::level_enum level)
			{
				for (int i = 0; i < s_IndentLevel; i++)
				{
					s_Logger->log(level, "    ");
				}
			}

			template <typename... Args>
			static inline void LogState(spdlog::level::level_enum level, spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				LogIndent(level);

				if (level == spdlog::level::trace)
				{
					s_Logger->log(level, "- ");
				}
				else if (level == spdlog::level::err)
				{
					s_Logger->log(level, "*Error: ");
				}
				else if (level == spdlog::level::warn)
				{
					s_Logger->log(level, "** Discrepancy: ");
				}

				s_Logger->log(level, fmt, std::forward<Args>(args)...);
				s_Logger->log(level, "\n");
			}
		public:

			template <typename... Args>
			static inline void LogRaw(spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				auto level = spdlog::level::trace;
				s_Logger->log(level, fmt, std::forward<Args>(args)...);
				s_Logger->log(level, "\n");
			}

			template <typename... Args>
			static inline void LogIt(spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				LogState(spdlog::level::trace, fmt, std::forward<Args>(args)...);
			}

			template <typename... Args>
			static inline void LogError(spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				LogState(spdlog::level::err, fmt, std::forward<Args>(args)...);
			}

			template<typename... Args>
			static inline void LogDiscrepancy(spdlog::format_string_t<Args...> fmt, Args&&...args)
			{
				LogState(spdlog::level::warn, fmt, std::forward<Args>(args)...);
			}

			template <typename... Args>
			static inline void LogBegin(spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				auto level = spdlog::level::trace;

				LogIndent(level);
				s_Logger->log(level, fmt, std::forward<Args>(args)...);
				s_Logger->log(level, " [Begin]\n");

				s_IndentLevel++;
			}

			template <typename... Args>
			static inline void LogEnd(spdlog::format_string_t<Args...> fmt, Args &&...args)
			{
				if (s_IndentLevel > 0)
				{
					s_IndentLevel--;
				}

				auto level = spdlog::level::trace;
				LogIndent(level);
				s_Logger->log(spdlog::level::trace, fmt, std::forward<Args>...);
				s_Logger->log(level, " [End]\n\n");
			}

		private:
			static std::shared_ptr<spdlog::logger> s_Logger;
			static int s_IndentLevel;
		};

	}  // namespace utils

#define DO_LOGGING 1
#if DO_LOGGING
#define LOG(...) utils::Log::LogIt(__VA_ARGS__)
#define LOG_RAW(...) utils::Log::LogRaw(__VA_ARGS__)
#define LOG_ERR(...) utils::Log::LogError(__VA_ARGS__)
#define LOG_DISCREPANCY(...) utils::Log::LogDiscrepancy(__VA_ARGS__)
#define BeginLOG(...) utils::Log::LogBegin(__VA_ARGS__)
#define EndLOG(...) utils::Log::LogEnd(__VA_ARGS__)
#else
#define LOG(...)
#define LOG_RAW(...)
#define LOG_ERR(...)
#define BeginLOG(...)
#define EndLOG(...)
#endif

}  // namespace paplease
