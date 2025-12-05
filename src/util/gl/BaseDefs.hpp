#pragma once

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

#define LGL_API __declspec(dllexport)

namespace lgl {
	template<typename T>
	class TMAP : public std::unordered_map<std::string, std::shared_ptr<T>> {
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

		bool has(std::string key) {
			return std_map_base::find(key) != std_map_base::end();
		}
	};

	template<typename T>
	using TVEC = std::vector<std::shared_ptr<T>>;

	template<typename T>
	using UPTR = std::unique_ptr<T>;

	template<typename T>
	using SPTR = std::shared_ptr<T>;

	using UINT = unsigned int;

	using CSTRING = const char*;

	using BYTE = unsigned char;
}