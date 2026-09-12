#include "GpuProgram.hxx"

namespace lgl {
std::string GpuProgram::shaderTypeToString(std::uint32_t type) {
  switch (type) {
    case GL_VERTEX_SHADER:
      return std::string("vertex shader");
    case GL_FRAGMENT_SHADER:
      return std::string("fragment shader");
    case GL_TESS_EVALUATION_SHADER:
      return std::string("tesselation evaluation shader");
    case GL_TESS_CONTROL_SHADER:
      return std::string("tesselation control shader");
    case GL_GEOMETRY_SHADER:
      return std::string("geometry shader");
    default:
      return std::string("NULL");
  }
}

void GpuProgram::displayShaderCompilationResult(std::uint32_t type,
                                                std::uint32_t id) {
  std::int32_t result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    std::int32_t length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);

    utl::log(utl::LGL_ERROR, "Failed to compile {} at path {}\n{}",
             shaderTypeToString(type), m_shaders[type].m_path, message);

    delete[] message;
  } else if (result == GL_TRUE) {
    utl::log(utl::LGL_OK, "Successful {} compilation!\n",
             shaderTypeToString(type));
  }
}

std::uint32_t GpuProgram::compileShaderSource(std::uint32_t type,
                                              const std::string& source) {
  std::uint32_t id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  displayShaderCompilationResult(type, id);

  return id;
}

void GpuProgram::readShaderSource(std::string& shader,
                                  const std::string& filePath) {
  try {
    shader = utl::readFileToString(filePath);
  } catch (const std::exception& error) {
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

GpuProgram::GpuProgram(const std::string& programName) : m_programId(0) {
  m_programName = programName;
  m_shaders[GL_VERTEX_SHADER] = Shader();
  m_shaders[GL_FRAGMENT_SHADER] = Shader();
  m_shaders[GL_TESS_CONTROL_SHADER] = Shader();
  m_shaders[GL_TESS_EVALUATION_SHADER] = Shader();
  m_shaders[GL_GEOMETRY_SHADER] = Shader();
}

void GpuProgram::createProgram() {
  glDeleteProgram(m_programId);
  std::uint32_t id = glCreateProgram();

  utl::setLogMode(utl::GENERAL_LOGS);
  utl::log(utl::LGL_INFO, "Compilation result of {}:\n", m_programName);
  std::vector<std::uint32_t> shaderIds;
  for (const auto& shader : m_shaders) {
    if (!shader.second.empty()) {
      std::uint32_t shaderId =
          compileShaderSource(shader.first, shader.second.m_source);
      shaderIds.push_back(shaderId);
    }
  }
  utl::log(utl::LGL_EMPTY, "\n");

  for (std::uint32_t shaderId : shaderIds) {
    glAttachShader(id, shaderId);
  }

  glLinkProgram(id);
  glValidateProgram(id);

  for (std::uint32_t shaderId : shaderIds) {
    glDeleteShader(shaderId);
  }

  m_programId = id;
}

void GpuProgram::addShader(std::uint32_t type, const std::string& filePath) {
  try {
    if (m_shaders.find(type) == m_shaders.end()) {
      throw std::runtime_error(
          std::format("Not available shader type: {}", type));
    }

    std::string path = std::format("../../../res/{}", filePath);
    std::string& shaderSource = m_shaders[type].m_source;
    shaderSource = utl::readFileToString(path);
    m_shaders[type].m_hash =
        m_hasher.hash(shaderSource.c_str(), shaderSource.size(), 0);
    m_shaders[type].m_path = filePath;
  } catch (const std::exception& error) {
    throw error;
  }
}

void GpuProgram::handleShaderSourceChange() {
  bool recompile = false;
  for (auto& shader : m_shaders) {
    if (!shader.second.empty()) {
      std::string newSource;
      try {
        std::string path = std::format("../../../res/{}", shader.second.m_path);
        readShaderSource(newSource, path);
      } catch (const std::exception& error) {
        return;
      }
      std::uint64_t newHash =
          m_hasher.hash(newSource.c_str(), newSource.size(), 0);
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

void GpuProgram::bind() const { glUseProgram(m_programId); }

void GpuProgram::unBind() const { glUseProgram(0); }

void GpuProgram::setVec3(const std::string& name, glm::vec3 value) const {
  glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void GpuProgram::setVec3(const std::string& name, float x, float y,
                         float z) const {
  glUniform3f(glGetUniformLocation(m_programId, name.c_str()), x, y, z);
}

void GpuProgram::setMat4(const std::string& name, glm::mat4 mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}

void GpuProgram::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void GpuProgram::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}
}  // namespace lgl