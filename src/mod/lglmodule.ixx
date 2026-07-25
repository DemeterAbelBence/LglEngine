module;

#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <sstream>
#include <random>
#include <array>
#include <optional>
#include <filesystem>
#include <format>
#include <unordered_set>

#ifdef _WIN32
#include <windows.h>
#endif

export module lglmodule;

// Helper macro to include the template/type info in static_assert messages
#if defined(_MSC_VER)
#define LGL_TYPE_NAME_FUNC __FUNCSIG__
#else
#define LGL_TYPE_NAME_FUNC __PRETTY_FUNCTION__
#endif

// Helpers to stringify line numbers for inclusion in messages
#define LGL_STR_HELPER(x) #x
#define LGL_STR(x) LGL_STR_HELPER(x)

export namespace lgl {
	namespace utl {
		// Collection utilities
		template<typename T>
		using set = std::unordered_set<T>;

		template<typename U, typename V>
		using uvmap = std::unordered_map<U, V>;

		template<typename T>
		using imap = std::unordered_map<int, T>;

		template<typename T>
		class smap : public std::unordered_map<std::string, std::shared_ptr<T>> {
		public:
			using std_map_base = std::unordered_map<std::string, std::shared_ptr<T>>;
			using std_map_base::std_map_base;
			std::shared_ptr<T> at(std::string key) const {
				if (std_map_base::find(key) == std_map_base::end()) {
					auto msg = std::string("Key ") + key + std::string(" not found in MAP!");
					throw std::exception(msg.c_str());
				}
			return std_map_base::at(key);
			}
			bool has(std::string key) const {
				return std_map_base::find(key) != std_map_base::end();
			}
		};

		template<typename T>
		using vec = std::vector<T>;

		template<typename T>
		using svec = std::vector<std::shared_ptr<T>>;

		template<typename T, int s>
		using arr = std::array<T, s>;

		// Smart pointer utilities
		template<typename T>
		using uptr = std::unique_ptr<T>;

		template<typename T>
		using sptr = std::shared_ptr<T>;

		template<typename T>
		using wptr = std::weak_ptr<T>;

		template<typename T, typename... Args>
		sptr<T> makeSptr(Args&&... args) {
            static_assert(
				std::is_constructible_v<T, Args...>,
				"makeSptr failed: type is not constructible with the provided arguments.\n"
				"Instantiation: " LGL_TYPE_NAME_FUNC "\n"
				"Location: " __FILE__ ":" LGL_STR(__LINE__) "\n"
				"Suggestion: Check that the target type's constructors match the provided argument types and that any implicit conversions are available."
			);
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		uptr<T> makeUptr(Args&&... args) {
            static_assert(
				std::is_constructible_v<T, Args...>,
				"makeUptr failed: type is not constructible with the provided arguments.\n"
				"Instantiation: " LGL_TYPE_NAME_FUNC "\n"
				"Location: " __FILE__ ":" LGL_STR(__LINE__) "\n"
				"Suggestion: Check that the target type's constructors match the provided argument types and that any implicit conversions are available."
			);
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

        template<typename U, typename V>
		sptr<U> sptrCast(const sptr<V>& v) {
			auto res = std::dynamic_pointer_cast<U>(v);
			if (!res) {
				std::string msg = std::string("sptrCast failed: dynamic_pointer_cast returned nullptr.\n")
					+ "Instantiation: " + LGL_TYPE_NAME_FUNC + "\n"
					+ std::string("Location: ") + __FILE__ + ":" LGL_STR(__LINE__) + "\n"
					+ "Suggestion: Ensure the source shared_ptr actually owns an object of the target type (or derived) and that RTTI is enabled.";
				throw std::runtime_error(msg);
			}
			return res;
		}

		template<typename T>
		unsigned long long makePointerPairHash(T* ptr1, T* ptr2) {
			size_t h1 = std::hash<T*>{}(ptr1);
			size_t h2 = std::hash<T*>{}(ptr2);

			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}

		// Primitive type utilities
		using int32 = std::int32_t;
		using uint = std::uint32_t;
		using ul = unsigned long;
		using ull = std::uint64_t;
		using byte = unsigned char;
		using cmemptr = const void*;
		using memptr = void*;

		// String utilities

		using cstr = const char*;
		using str = std::string;

		template<typename... Args>
		str strFormat(std::format_string<Args...> formatString, Args&&... args) {
			return std::format(formatString, std::forward<Args>(args)...);
		}

		bool strEquals(const str& lhs, const str& rhs) {
			return lhs == rhs;
		}

		// workflow utilities
		using runtime = std::runtime_error;
		using except = std::exception;

		// Random utilities
		using rdev = std::random_device;
		using rng = std::mt19937;

		template<typename T>
		using udist = std::uniform_real_distribution<T>;
		
		template<typename T>
		using opt = std::optional<T>;

		template<typename... Args>
		class tup {
		private:
			std::tuple<Args...> data;
			static constexpr std::size_t tuple_size = sizeof...(Args);

		public:
			tup() : data() {}

			tup(Args... args) : data(std::forward<Args>(args)...) {}

			template<std::size_t I>
			auto& get() {
				static_assert(I < tuple_size, "Tuple index out of bounds!");
				return std::get<I>(data);
			}

			template<std::size_t I>
			const auto& get() const {
				static_assert(I < tuple_size, "Tuple index out of bounds!");
				return std::get<I>(data);
			}

			constexpr std::size_t size() const {
				return tuple_size;
			}

			std::tuple<Args...>& tuple() {
				return data;
			}

			const std::tuple<Args...>& tuple() const {
				return data;
			}
		};
	}

	utl::str readFileToString(const utl::str& filePath) {
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





	class Logger {
	private:
		inline static bool consoleInitialized = false;
		static inline HANDLE hConsole;

		static void enableAnsiColors() {
			if (consoleInitialized) {
				return;
			}

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
		}

	public:
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

		static utl::str ansiColorCode(LogColor color) {
			switch (color) {
				case LC_RED:     return "\033[31m";
				case LC_YELLOW:  return "\033[33m";
				case LC_GREEN:   return "\033[32m";
				case LC_CYAN:    return "\033[36m";
				default:         return "\033[0m";
			}
		}

		static void setLogMode(LogMode logMode) {
			utl::str folderName, fileName;

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
			utl::str folder = std::format("../../logs/{}", folderName);
			CreateDirectoryA(folder.c_str(), nullptr);

			logPath = std::format("../../logs/{}/{}{}.log", folderName, fileName, logTime);

			currentLogMode = logMode;
		}

		inline static bool logToFile = false;
		inline static utl::str logPath;
		inline static LogMode currentLogMode = GENERAL_LOGS;
		inline static utl::uint logFrequency = 3;
		inline static const utl::str logTime = []() {
			std::time_t t = std::time(nullptr);
			std::tm tm{};
			localtime_s(&tm, &t);
			char buf[32];
			std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tm);
			return std::string(buf);
		}();
		inline static utl::uvmap<utl::uint, utl::ull> logCounters = {
			{BISECTION_LOGS, 0},
			{CONTACT_LOGS, 0},
			{PHYSICS_LOGS, 0},
			{GENERAL_LOGS, 0}
		};

		static void incrementLogCounterFor(LogMode logMode) {
			logCounters[logMode]++;
		}

		template<typename... Args>
		static void log(LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
			utl::str formattedMsg = std::format(formatString, std::forward<Args>(args)...);

			utl::str levelStr;
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
				auto& counter = logCounters[static_cast<utl::uint>(currentLogMode)];
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
				SetConsoleTextAttribute(hConsole, color);
				std::cout << levelStr << formattedMsg;
				SetConsoleTextAttribute(hConsole, LC_DEFAULT);
			}
		}

		template<typename... Args>
		static void logIf(bool enable, LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
			if (enable) {
				log(level, formatString, std::forward<Args>(args)...);
			}
		}
	};
}