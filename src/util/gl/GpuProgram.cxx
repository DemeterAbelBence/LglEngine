#include "GpuProgram.hxx"

namespace lgl {
	utl::str GpuProgram::shaderTypeToString(utl::uint type) {
		switch (type) {
		case GL_VERTEX_SHADER:
			return utl::str("vertex shader");
		case GL_FRAGMENT_SHADER:
			return utl::str("fragment shader");
		case GL_TESS_EVALUATION_SHADER:
			return utl::str("tesselation evaluatioln shader");
		case GL_TESS_CONTROL_SHADER:
			return utl::str("tesselation control shader");
		case GL_GEOMETRY_SHADER:
			return utl::str("geometry shader");
		default:
			return utl::str("NULL");
		}
	}

	void GpuProgram::displayShaderCompilationResult(utl::uint type, utl::uint id) {
		utl::int32 result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) {
			utl::int32 length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);

			Logger::log(Logger::LGL_ERROR, "Failed to compile {} at path {}\n", shaderTypeToString(type), m_shaders[type].m_path);

			delete[] message;
		}
		else if (result == GL_TRUE) {
			Logger::log(Logger::LGL_EMPTY, "Successful {} compilation!\n", shaderTypeToString(type));
		}
	}

	utl::uint GpuProgram::compileShaderSource(utl::uint type, const utl::str& source) {
		utl::uint id = glCreateShader(type);
		utl::cstr src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		displayShaderCompilationResult(type, id);

		return id;
	}

	void GpuProgram::readShaderSource(utl::str& shader, const utl::str& filePath) {
		try {
			shader = readFileToString(filePath);
		}
		catch (const utl::except& error) {
			throw error;
		}
	}

	GpuProgram::GpuProgram() : m_programId(0), m_programName("NULL") {
		m_shaders[GL_VERTEX_SHADER] = Shader();
		m_shaders[GL_FRAGMENT_SHADER] = Shader();
		m_shaders[GL_TESS_CONTROL_SHADER] = Shader();
		m_shaders[GL_TESS_EVALUATION_SHADER] = Shader();
		m_shaders[GL_GEOMETRY_SHADER] = Shader();
	}

	GpuProgram::GpuProgram(const utl::str& programName) : m_programId(0) {
		m_programName = programName;
		m_shaders[GL_VERTEX_SHADER] = Shader();
		m_shaders[GL_FRAGMENT_SHADER] = Shader();
		m_shaders[GL_TESS_CONTROL_SHADER] = Shader();
		m_shaders[GL_TESS_EVALUATION_SHADER] = Shader();
		m_shaders[GL_GEOMETRY_SHADER] = Shader();
	}

	void GpuProgram::createProgram() {
		glDeleteProgram(m_programId);
		utl::uint id = glCreateProgram();

		Logger::log(Logger::LGL_INFO, "Compilation result of {}\n", m_programName);
		utl::vec<utl::uint> shaderIds;
		for (const auto& shader : m_shaders) {
			if (!shader.second.empty()) {
				utl::uint shaderId = compileShaderSource(shader.first, shader.second.m_source);
				shaderIds.push_back(shaderId);
			}
		}
		Logger::log(Logger::LGL_EMPTY, "\n");

		for (utl::uint shaderId : shaderIds) {
			glAttachShader(id, shaderId);
		}

		glLinkProgram(id);
		glValidateProgram(id);

		for (utl::uint shaderId : shaderIds) {
			glDeleteShader(shaderId);
		}

		m_programId = id;
	}

	void GpuProgram::addShader(utl::uint type, const utl::str& filePath) {
		try {
			if (m_shaders.find(type) == m_shaders.end()) {
				throw utl::runtime(utl::strFormat("Not available shader type: {}", type));
			}

			utl::str& shaderSource = m_shaders[type].m_source;
			shaderSource = readFileToString(filePath);
			m_shaders[type].m_hash = m_hasher.hash(shaderSource.c_str(), shaderSource.size(), 0);
			m_shaders[type].m_path = filePath;
		}
		catch (const utl::except& error) {
			throw error;
		}
	}

	void GpuProgram::handleShaderSourceChange() {
		bool recompile = false;
		for (auto& shader : m_shaders) {
			if (!shader.second.empty()) {
				utl::str newSource;
				try {
					readShaderSource(newSource, shader.second.m_path);
				}
				catch (const utl::except& error) {
					return;
				}
				utl::ull newHash = m_hasher.hash(newSource.c_str(), newSource.size(), 0);
				if (newHash != shader.second.m_hash) {
					shader.second.m_source = newSource;
					shader.second.m_hash = newHash;
					recompile = true;
				}
			}
		}
		if (recompile) {
			createProgram();
		}
	}

	void GpuProgram::bind() const {
		glUseProgram(m_programId);
	}

	void GpuProgram::unBind() const {
		glUseProgram(0);
	}

	void GpuProgram::setVec3(const utl::str& name, glm::vec3 value) const {
		glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
	}

	void GpuProgram::setVec3(const utl::str& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(m_programId, name.c_str()), x, y, z);
	}

	void GpuProgram::setMat4(const utl::str& name, glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void GpuProgram::setFloat(const utl::str& name, float value) const {
		glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
	}

	void GpuProgram::setInt(const utl::str& name, int value) const {
		glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
	}
}