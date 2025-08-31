#pragma once
//
// This logging system is highly inspired (almost an exact copy) of Walnut's one.
// You can find it in Walnut/Source/Core/Walnut/Log.h
// I just rewrote it by hand while modifying it to fit exactly my needs
// 
// Just like Walnut's one, it's a wrapper around the spdlog library (https://github.com/gabime/spdlog)
//

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <map>
#include <format>  // For C++20 std::format

#define OL_ASSERT_MESSAGE_BOX (!OL_DIST && OL_PLATFORM_WINDOWS)
#if OL_ASSERT_MESSAGE_BOX
#ifdef OL_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logger Class																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace OllamaCore
{

	class Log
	{
	public:
		// Class enums and struct
		enum class Level : uint8_t
		{
			Trace = 0, Info, Warning, Error, Fatal
		};

		enum class Type : uint8_t
		{
			API = 0, App
		}; // This is used to separate the logs from the Ollama API related code and the logs from the ImGui app/interface

		struct TagDetails
		{
			bool enabled = true;
			Log::Level level = Log::Level::Trace;
		};

	public:
		// Class Methods
		static void Init();
		static void ShutDown();

		static bool HasTag(std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::map<std::string, TagDetails>& EnabledTags() { return s_EnabledTags; }

		template <typename... Args>
		static void MessageTag(Log::Level level, Log::Type type, std::string_view tag, fmt::format_string<Args...> format, Args&&... args);

		static void MessageTag(Log::Level level, Log::Type type, std::string_view tag, std::string_view message);

		template <typename... Args>
		static void MessageAssert(Log::Type type, std::string_view prefix, fmt::format_string<Args...> fmtStr, Args&&... args);

		static void MessageAssert(Log::Type type, std::string_view prefix);

	private:
		// Private variables (static)
		static std::shared_ptr<spdlog::logger> s_Logger;

		inline static std::map<std::string, TagDetails> s_EnabledTags;
	};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tagged Logging																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// API Logging
#define OL_API_TRACE_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Trace, ::OllamaCore::Log::Type::API, tag, __VA_ARGS__)
#define OL_API_INFO_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Info, ::OllamaCore::Log::Type::API, tag, __VA_ARGS__)
#define OL_API_WARN_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Warning, ::OllamaCore::Log::Type::API, tag, __VA_ARGS__)
#define OL_API_ERROR_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Error, ::OllamaCore::Log::Type::API, tag, __VA_ARGS__)
#define OL_API_FATAL_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Fatal, ::OllamaCore::Log::Type::API, tag, __VA_ARGS__)

// App/Client Logging
#define OL_TRACE_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Trace, ::OllamaCore::Log::Type::App, tag, __VA_ARGS__)
#define OL_INFO_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Info, ::OllamaCore::Log::Type::App, tag, __VA_ARGS__)
#define OL_WARN_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Warning, ::OllamaCore::Log::Type::App, tag, __VA_ARGS__)
#define OL_ERROR_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Error, ::OllamaCore::Log::Type::App, tag, __VA_ARGS__)
#define OL_FATAL_TAG(tag, ...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Fatal, ::OllamaCore::Log::Type::App, tag, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// API Logging
#define OL_API_TRACE(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Trace, ::OllamaCore::Log::Type::API, "API", __VA_ARGS__)
#define OL_API_INFO(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Info, ::OllamaCore::Log::Type::API, "API", __VA_ARGS__)
#define OL_API_WARN(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Warning, ::OllamaCore::Log::Type::API, "API", __VA_ARGS__)
#define OL_API_ERROR(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Error, ::OllamaCore::Log::Type::API, "API", __VA_ARGS__)
#define OL_API_FATAL(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Fatal, ::OllamaCore::Log::Type::API, "API", __VA_ARGS__)

// App/Client Logging
#define OL_TRACE(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Trace, ::OllamaCore::Log::Type::App, "App", __VA_ARGS__)
#define OL_INFO(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Info, ::OllamaCore::Log::Type::App, "App", __VA_ARGS__)
#define OL_WARN(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Warning, ::OllamaCore::Log::Type::App, "App", __VA_ARGS__)
#define OL_ERROR(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Error, ::OllamaCore::Log::Type::App, "App", __VA_ARGS__)
#define OL_FATAL(...) ::OllamaCore::Log::MessageTag(::OllamaCore::Log::Level::Fatal, ::OllamaCore::Log::Type::App, "App", __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template Implementations																							//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace OllamaCore
{
	// Modifed Walnut's original implementation to handle lvalues as formatting parameters

	template <typename... Args>
	void Log::MessageTag(Log::Level level, Log::Type type, std::string_view tag, fmt::format_string<Args...> format, Args&&... args)
	{
		std::string tagStr(tag);
		auto it = s_EnabledTags.find(tagStr);
		if (it == s_EnabledTags.end())
		{
			// If tag doesn't exist, create it with default settings
			s_EnabledTags[tagStr] = TagDetails{};
			it = s_EnabledTags.find(tagStr);
		}

		const TagDetails& details = it->second;
		if (details.enabled && details.level <= level)
		{
			std::string formatted = fmt::format(format, std::forward<Args>(args)...);
			switch (level)
			{
			case Log::Level::Trace:
				s_Logger->trace("[{0}] : {1}", tag, formatted);
				break;
			case Log::Level::Info:
				s_Logger->info("[{0}] : {1}", tag, formatted);
				break;
			case Log::Level::Warning:
				s_Logger->warn("[{0}] : {1}", tag, formatted);
				break;
			case Log::Level::Error:
				s_Logger->error("[{0}] : {1}", tag, formatted);
				break;
			case Log::Level::Fatal:
				s_Logger->critical("[{0}] : {1}", tag, formatted);
				break;
			}
		}
	}

	inline void Log::MessageTag(Log::Level level, Log::Type type, std::string_view tag, std::string_view message)
	{
		std::string tagStr(tag);
		auto it = s_EnabledTags.find(tagStr);
		if (it == s_EnabledTags.end())
		{
			// If tag doesn't exist, create it with default settings
			s_EnabledTags[tagStr] = TagDetails{};
			it = s_EnabledTags.find(tagStr);
		}

		const TagDetails& details = it->second;
		if (details.enabled && details.level <= level)
		{
			switch (level)
			{
			case Log::Level::Trace:
				s_Logger->trace("[{0}] : {1}", tag, message);
				break;
			case Log::Level::Info:
				s_Logger->info("[{0}] : {1}", tag, message);
				break;
			case Log::Level::Warning:
				s_Logger->warn("[{0}] : {1}", tag, message);
				break;
			case Log::Level::Error:
				s_Logger->error("[{0}] : {1}", tag, message);
				break;
			case Log::Level::Fatal:
				s_Logger->critical("[{0}] : {1}", tag, message);
				break;
			}
		}
	}
	
	template <typename... Args>
	inline void Log::MessageAssert(Log::Type type, std::string_view prefix, fmt::format_string<Args...> fmtStr, Args&&... args)
	{
		std::string message = fmt::format(fmtStr, std::forward<Args>(args)...);
		s_Logger->critical("[{0}] : {1}", prefix, message);
#if OL_ASSERT_MESSAGE_BOX
		MessageBoxA(nullptr, message.c_str(), "OllamaCore Assert", MB_OK | MB_ICONERROR);
#endif
	}

	inline void Log::MessageAssert(Log::Type type, std::string_view prefix)
	{
		s_Logger->critical("{0}", prefix);
#if OL_ASSERT_MESSAGE_BOX
		MessageBoxA(nullptr, "No message was provided :'(", "OllamaCore Assert", MB_OK | MB_ICONERROR);
#endif
	}
}