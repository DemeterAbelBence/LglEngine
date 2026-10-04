#pragma once

import utl;
import glm;
import std;

#include <GLEW/glew.h>
#include <XXHASH/xxhash64.h>

namespace lgl {
struct Shader {
 public:
  std::string m_source;
  std::string m_path;
  std::uint64_t m_hash;

 public:
  Shader() : m_source(""), m_path(""), m_hash(0) {}
  bool empty() const { return m_source.empty(); }
};

class GpuProgram {
 private:
 private:
  std::uint32_t m_programId;
  std::string m_programName;
  const XXHash64 m_hasher = XXHash64(0);
  bool m_logShaderCompilation = true;

 private:
  std::unordered_map<std::uint32_t, Shader> m_shaders;

  std::string shaderTypeToString(std::uint32_t type);
  void displayShaderCompilationResult(std::uint32_t type, std::uint32_t id);
  std::uint32_t compileShaderSource(std::uint32_t type, const std::string& source);
  void readShaderSource(std::string& shaderSource, const std::string& filePath);

 public:
  GpuProgram();
  GpuProgram(const std::string& programName);
  std::uint32_t getProgramId() { return m_programId; }
  std::string getProgramName() { return m_programName; }

  void addShader(std::uint32_t type, const std::string& filePath);
  void handleShaderSourceChange();
  void bind() const;
  void unBind() const;

  void setVec3(const std::string& name, glm::vec3 value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setMat4(const std::string& name, glm::mat4 value) const;
  void setFloat(const std::string& name, float value) const;
  void setInt(const std::string& name, int value) const;
  virtual void createProgram(bool logShaderCompilation = true);
};
}  // namespace lgl
