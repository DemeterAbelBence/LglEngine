module;

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>
#include <ctime>

import std;

export module utl;

export namespace utl {	
	std::string readFileToString(const std::string& filePath) {
		std::ifstream stream(filePath);
		std::string line;
		std::string result;

		try {
			if (!stream.is_open()) {
				auto errorMessage = std::string("Failed to open file at: ") + filePath;
				throw std::runtime_error(errorMessage);
			}

			while (std::getline(stream, line)) {
				result += line;
				result += '\n';
			}
			stream.close();
		}
		catch (const std::exception& error) {
			throw error;
		}

		return result;
	}

	void createDirectories(const std::string& path) {
		std::filesystem::path p(path);
		if (auto parent = p.parent_path(); !parent.empty()) {
			std::filesystem::create_directories(parent);
		}
	}

	enum LogLevel {
		LGL_INFO,
		LGL_WARN,
		LGL_ERROR,
		LGL_OK,
		LGL_EMPTY
	};

	enum LogColor {
		LC_DEFAULT = 7,
		LC_RED = 12,
		LC_YELLOW = 14,
		LC_GREEN = 10,
		LC_CYAN = 11
	};

	enum LogMode {
		BISECTION_LOGS,
		CONTACT_LOGS,
		PHYSICS_LOGS,
		GENERAL_LOGS
	};

	inline bool consoleInitialized = false;
#ifdef _WIN32
	inline HANDLE hConsole = INVALID_HANDLE_VALUE;
#endif

	inline bool logToFile = false;
	inline std::string logPath;
	inline LogMode currentLogMode = GENERAL_LOGS;
	inline unsigned int logFrequency = 7;
	inline const std::string logTime = []() {
		std::time_t t = std::time(nullptr);
		std::tm tm{};
		localtime_s(&tm, &t);
		char buf[32];
		std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tm);
		return std::string(buf);
	}();

	inline std::unordered_map<unsigned int, unsigned long long> logCounters = {
		{BISECTION_LOGS, 0},
		{CONTACT_LOGS, 0},
		{PHYSICS_LOGS, 0},
		{GENERAL_LOGS, 0}
	};

	inline void enableAnsiColors() {
		if (consoleInitialized) {
			return;
		}
#ifdef _WIN32
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole != INVALID_HANDLE_VALUE) {
			DWORD dwMode = 0;
			if (GetConsoleMode(hConsole, &dwMode)) {
				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(hConsole, dwMode);
			}
			consoleInitialized = true;
		}
		else {
			throw std::runtime_error("Failed to get console handle for ANSI color support.");
		}
#endif
	}

	inline void incrementLogCounterFor(LogMode logMode) {
		try {
			logCounters.at(static_cast<unsigned int>(logMode))++;
		}
		catch (...) {
			throw std::runtime_error("Invalid log mode");
		}
	}

	inline std::string ansiColorCode(LogColor color) {
		switch (color) {
		case LC_RED:     return "\033[31m";
		case LC_YELLOW:  return "\033[33m";
		case LC_GREEN:   return "\033[32m";
		case LC_CYAN:    return "\033[36m";
		default:         return "\033[0m";
		}
	}

	inline void setLogMode(LogMode logMode) {
		std::string folderName, fileName;

		switch (logMode) {
		case BISECTION_LOGS:
			folderName = "bisection";
			fileName = "lgl_bis_";
			break;
		case CONTACT_LOGS:
			folderName = "contact";
			fileName = "lgl_con_";
			break;
		case PHYSICS_LOGS:
			folderName = "physics";
			fileName = "lgl_phy_";
			break;
		case GENERAL_LOGS:
			folderName = "general";
			fileName = "lgl_gen_";
			break;
		}

		CreateDirectoryA("../../logs", nullptr);
		std::string folder = std::format("../../logs/{}", folderName);
		CreateDirectoryA(folder.c_str(), nullptr);

		logPath = std::format("../../logs/{}/{}{}.log", folderName, fileName, logTime);

		currentLogMode = logMode;
	}

	template<typename... Args>
	inline void log(LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
		std::string formattedMsg = std::format(formatString, std::forward<Args>(args)...);

		std::string levelStr;
		LogColor color = LC_DEFAULT;
		switch (level) {
		case LGL_INFO:
			levelStr = "[LGL_INFO] ";
			color = LC_CYAN;
			break;
		case LGL_WARN:
			levelStr = "[LGL_WARN] ";
			color = LC_YELLOW;
			break;
		case LGL_ERROR:
			levelStr = "[LGL_ERROR] ";
			color = LC_RED;
			break;
		case LGL_OK:
			levelStr = "[LGL_OK] ";
			color = LC_GREEN;
			break;
		case LGL_EMPTY:
			levelStr = "";
			break;
		}

		if (logToFile) {
			auto& counter = logCounters[static_cast<unsigned int>(currentLogMode)];
			if (counter % logFrequency == 0) {
				FILE* file = nullptr;
				fopen_s(&file, logPath.c_str(), "a");
				if (file) {
					fputs(levelStr.c_str(), file);
					fputs(formattedMsg.c_str(), file);
					fclose(file);
				}
			}
			else if (currentLogMode != GENERAL_LOGS){
				return;
			}
		}
		else {
			enableAnsiColors();
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, color);
#endif
			std::cout << levelStr << formattedMsg;
#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, LC_DEFAULT);
#endif
		}
	}

	template<typename... Args>
	inline void logIf(bool enable, LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
		if (enable) {
			log(level, formatString, std::forward<Args>(args)...);
		}
	}
}