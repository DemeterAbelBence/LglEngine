#pragma once

#include "GpuProgram.hpp"

using namespace lgl;

std::string GpuProgram::shaderTypeToString(UINT type) {
	switch (type) {
		case GL_VERTEX_SHADER:
			return std::string("vertex shader");
		case GL_FRAGMENT_SHADER:
			return std::string("fragment shader");
		case GL_TESS_EVALUATION_SHADER:
			return std::string("tesselation evaluatioln shader");
		case GL_TESS_CONTROL_SHADER:
			return std::string("tesselation control shader");
		case GL_GEOMETRY_SHADER:
			return std::string("geometry shader");
		default:
			return std::string("NULL");
	}
}

void GpuProgram::displayShaderCompilationResult(UINT type, UINT id) {
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << shaderTypeToString(type) << " at path " << shaders[type].path << std::endl;
		std::cout << message << std::endl;

		delete[] message;
	}
	else if (result == GL_TRUE) {
		std::cout << "Successful " << shaderTypeToString(type) << " compilation!" << std::endl;
	}
}

UINT GpuProgram::compileShaderSource(UINT type, const std::string& source) {
	UINT id = glCreateShader(type);
	CSTRING src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	displayShaderCompilationResult(type, id);

	return id;
}

void GpuProgram::readShaderSource(std::string& shader, const std::string& filePath) {
	std::ifstream stream(filePath);
	std::string line;

	try {
		if (!stream.is_open()) {
			auto errorMessage = std::string("Failed to open shader file at: ") + filePath;
			throw std::runtime_error(errorMessage);
		}

		while (std::getline(stream, line)) {
			shader += line;
			shader += '\n';
		}
		stream.close();
	}
	catch (const std::exception& error) {
		throw error;
	}
}

GpuProgram::GpuProgram() : programId(0), programName("NULL") {
	shaders[GL_VERTEX_SHADER] = Shader();
	shaders[GL_FRAGMENT_SHADER] = Shader();
	shaders[GL_TESS_CONTROL_SHADER] = Shader();
	shaders[GL_TESS_EVALUATION_SHADER] = Shader();
	shaders[GL_GEOMETRY_SHADER] = Shader();
}

GpuProgram::GpuProgram(const std::string& _programName) : programId(0) {
	programName = _programName;
	shaders[GL_VERTEX_SHADER] = Shader();
	shaders[GL_FRAGMENT_SHADER] = Shader();
	shaders[GL_TESS_CONTROL_SHADER] = Shader();
	shaders[GL_TESS_EVALUATION_SHADER] = Shader();
	shaders[GL_GEOMETRY_SHADER] = Shader();
}

void GpuProgram::createProgram() {
	glDeleteProgram(programId);
	UINT id = glCreateProgram();

	std::cout << programName << " compilation results:" << std::endl;
	std::vector<UINT> shaderIds;
	for (const auto& shader : shaders) {
		if (!shader.second.empty()) {
			UINT shaderId = compileShaderSource(shader.first, shader.second.source);
			shaderIds.push_back(shaderId);
		}
	}
	std::cout << std::endl;

	for (UINT shaderId : shaderIds) {
		glAttachShader(id, shaderId);
	}
	glLinkProgram(id);
	glValidateProgram(id);

	for (UINT shaderId : shaderIds) {
		glDeleteShader(shaderId);
	}

	programId = id;
}

void GpuProgram::addShader(UINT type, const std::string& filePath) {
	try {
		if (shaders.find(type) == shaders.end()) {
			auto errorMessage = std::string("Not available shader type: ") + std::to_string(type);
			throw std::runtime_error(errorMessage);
		}
		std::string& shaderSource = shaders[type].source;
		readShaderSource(shaderSource, filePath);
		shaders[type].hash = hasher.hash(shaderSource.c_str(), shaderSource.size(), 0);
		shaders[type].path = filePath;
	}
	catch (const std::exception& error) {
		throw error;
	}
}

void GpuProgram::handleShaderSourceChange() {
	bool recompile = false;
	for (auto& shader : shaders) {
		if (!shader.second.empty()) {
			std::string newSource;
			try {
				readShaderSource(newSource, shader.second.path);
			}
			catch (const std::exception& error) {
				return;
			}
			std::uint64_t newHash = hasher.hash(newSource.c_str(), newSource.size(), 0);
			if (newHash != shader.second.hash) {
				shader.second.source = newSource;
				shader.second.hash = newHash;
				recompile = true;
			}
		}
	}
	if (recompile) {
		createProgram();
	}
}

void GpuProgram::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void GpuProgram::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void GpuProgram::setMat4(const std::string& name, glm::mat4 mat) const {
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void GpuProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void GpuProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}
