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
  } catch (const std::exception& error) {
    throw error;
  }

  return result;
}

struct LglConf {
  std::unordered_map<std::string, std::string> stringConfigs;
  std::unordered_map<std::string, bool> booleanConfigs;
  std::unordered_map<std::string, float> floatConfigs;
  std::unordered_map<std::string, int> intConfigs;
};

LglConf currentLglConf;

bool getCurrentBooleanConfig(const std::string& key) {
  auto it = currentLglConf.booleanConfigs.find(key);
  if (it != currentLglConf.booleanConfigs.end()) {
    return it->second;
  }
  throw std::out_of_range(std::string("Boolean config key not found: ") + key);
}

std::string getCurrentStringConfig(const std::string& key) {
  auto it = currentLglConf.stringConfigs.find(key);
  if (it != currentLglConf.stringConfigs.end()) {
    return it->second;
  }
  throw std::out_of_range(std::string("String config key not found: ") + key);
}

float getCurrentFloatConfig(const std::string& key) {
  auto it = currentLglConf.floatConfigs.find(key);
  if (it != currentLglConf.floatConfigs.end()) {
    return it->second;
  }
  throw std::out_of_range(std::string("Float config key not found: ") + key);
}

int getCurrentIntConfig(const std::string& key) {
  auto it = currentLglConf.intConfigs.find(key);
  if (it != currentLglConf.intConfigs.end()) {
    return it->second;
  }
  throw std::out_of_range(std::string("Int config key not found: ") + key);
}

LglConf readLglConfFromFile(const std::string& filePath) {
  LglConf conf;

  std::ifstream stream(filePath);
  if (!stream.is_open()) {
    throw std::runtime_error(std::string("Failed to open config file: ") +
                             filePath);
  }

  auto trim = [](std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
              return !std::isspace(ch);
            }));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
  };

  std::string line;
  while (std::getline(stream, line)) {
    trim(line);
    if (line.empty()) continue;

    auto colon1Pos = line.find(':');
    if (colon1Pos == std::string::npos) continue;

    std::string key = line.substr(0, colon1Pos);
    std::string rest = line.substr(colon1Pos + 1);
    trim(key);
    trim(rest);

    auto colon2Pos = rest.find(':');
    if (colon2Pos == std::string::npos) continue;

    std::string type = rest.substr(0, colon2Pos);
    std::string value = rest.substr(colon2Pos + 1);
    trim(type);
    trim(value);

    try {
      if (type == "string") {
        conf.stringConfigs.emplace(key, value);
      } else if (type == "bool") {
        bool v = false;
        if (value == "1" || value == "true" || value == "True" ||
            value == "TRUE")
          v = true;
        conf.booleanConfigs.emplace(key, v);
      } else if (type == "float") {
        if (!value.empty() && (value.back() == 'f' || value.back() == 'F'))
          value.pop_back();
        float f = std::stof(value);
        conf.floatConfigs.emplace(key, f);
      } else if (type == "int") {
        int i = std::stoi(value);
        conf.intConfigs.emplace(key, i);
      }
    } catch (const std::exception&) {
      continue;
    }
  }

  stream.close();
  return conf;
}

void createDirectories(const std::string& path) {
  std::filesystem::path p(path);
  if (auto parent = p.parent_path(); !parent.empty()) {
    std::filesystem::create_directories(parent);
  }
}

std::string findFolder(const std::string& folderName) {
  std::filesystem::path exeDir;

#ifdef _WIN32
  char buffer[MAX_PATH];
  DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
  if (len > 0 && len < MAX_PATH) {
    exeDir = std::filesystem::path(buffer).parent_path();
  }
#endif

  if (exeDir.empty()) {
    exeDir = std::filesystem::current_path();
  }

  for (std::filesystem::path dir = exeDir;;) {
    std::filesystem::path candidate = dir / folderName;
    if (std::filesystem::exists(candidate) &&
        std::filesystem::is_directory(candidate)) {
      return std::filesystem::absolute(candidate).string();
    }

    std::filesystem::path parent = dir.parent_path();
    if (parent.empty() || parent == dir) {
      break;
    }
    dir = parent;
  }

  return "";
}

inline const std::string lglResourceFolder = findFolder("res");
inline const std::string lglOutFolder = findFolder("out");

template <typename T>
std::uint64_t makePointerPairHash(T* ptr1, T* ptr2) {
  size_t h1 = std::hash<T*>{}(ptr1);
  size_t h2 = std::hash<T*>{}(ptr2);

  return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

enum LogLevel { LGL_INFO, LGL_WARN, LGL_ERROR, LGL_OK, LGL_EMPTY };

enum LogColor {
  LC_DEFAULT = 7,
  LC_RED = 12,
  LC_YELLOW = 14,
  LC_GREEN = 10,
  LC_CYAN = 11
};

enum LogMode { BISECTION_LOGS, CONTACT_LOGS, PHYSICS_LOGS, GENERAL_LOGS };

inline bool consoleInitialized = false;
#ifdef _WIN32
inline HANDLE hConsole = INVALID_HANDLE_VALUE;
#endif

inline bool enableSimulationLogging = false;
inline std::string simLogFileName;
inline LogMode currentLogMode = GENERAL_LOGS;
inline const std::string lglStartTime = []() {
  std::time_t t = std::time(nullptr);
  std::tm tm{};
  localtime_s(&tm, &t);
  char buf[32];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tm);
  return std::string(buf);
}();

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
  } else {
    throw std::runtime_error(
        "Failed to get console handle for ANSI color support.");
  }
#endif
}

inline std::string ansiColorCode(LogColor color) {
  switch (color) {
    case LC_RED:
      return "\033[31m";
    case LC_YELLOW:
      return "\033[33m";
    case LC_GREEN:
      return "\033[32m";
    case LC_CYAN:
      return "\033[36m";
    default:
      return "\033[0m";
  }
}

inline void setLogMode(LogMode logMode) {
  currentLogMode = logMode;

  if (logMode == GENERAL_LOGS) {
    return;
  }

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
  }

  CreateDirectoryA(std::format("{}/logs", lglOutFolder).c_str(), nullptr);
  CreateDirectoryA(std::format("{}/logs/{}", lglOutFolder, folderName).c_str(),
                   nullptr);

  simLogFileName = std::format("{}/logs/{}/{}{}", lglOutFolder, folderName,
                               fileName, lglStartTime);
}

template <typename... Args>
inline void log(LogMode logMode, LogLevel level,
                std::format_string<Args...> formatString, Args&&... args) {
  setLogMode(logMode);

  std::string formattedMsg =
      std::format(formatString, std::forward<Args>(args)...);

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

  if (currentLogMode != GENERAL_LOGS && enableSimulationLogging) {
    FILE* file = nullptr;
    fopen_s(&file, simLogFileName.c_str(), "a");
    if (file) {
      fputs(levelStr.c_str(), file);
      fputs(formattedMsg.c_str(), file);
      fclose(file);
    }
    return;
  } else {
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

template <typename... Args>
inline void logIf(bool enable, LogMode mode, LogLevel level,
                  std::format_string<Args...> formatString, Args&&... args) {
  if (enable) {
    log(mode, level, formatString, std::forward<Args>(args)...);
  }
}
}  // namespace utl