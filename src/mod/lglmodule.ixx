module;

#include <iostream>
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
		using umap = std::unordered_map<U, V>;

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

		// Primitive type utilities
		using int32 = std::int32_t;
		using uint = std::uint32_t;
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

      template<typename... Args>
		static void log(LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
			// Logging enabled by default for backward compatibility with existing call sites.
			enableAnsiColors();

			utl::str formattedMsg = std::format(formatString, std::forward<Args>(args)...);

			utl::str levelStr;
			switch (level) {
				case LGL_INFO: 
					levelStr = "[LGL_INFO] "; 
					SetConsoleTextAttribute(hConsole, LogColor::LC_CYAN);
					break;
				case LGL_WARN:
					levelStr = "[LGL_WARN] "; 
					SetConsoleTextAttribute(hConsole, LogColor::LC_YELLOW);
					break;
				case LGL_ERROR:
					levelStr = "[LGL_ERROR] ";
					SetConsoleTextAttribute(hConsole, LogColor::LC_RED);
					break;
				case LGL_OK:
					levelStr = "[LGL_OK] ";
					SetConsoleTextAttribute(hConsole, LogColor::LC_GREEN);
					break;
				case LGL_EMPTY:
					levelStr = "";
					break;
			}

			std::cout << levelStr << formattedMsg;
			SetConsoleTextAttribute(hConsole, LogColor::LC_DEFAULT);
		}

		template<typename... Args>
		static void logIf(bool enable, LogLevel level, std::format_string<Args...> formatString, Args&&... args) {
			if (enable) {
				log(level, formatString, std::forward<Args>(args)...);
			}
		}
	};
}