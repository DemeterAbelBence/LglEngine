#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <XXHASH/xxhash64.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
	class LGL_API GpuProgram {
	private:
		struct Shader {
		public:
			std::string source;
			std::string path;
			std::uint64_t hash;

		public:
			Shader() : source(""), path(""), hash(0) {}
			bool empty() const { return source.empty(); }
		};

	private:
		UINT programId;
		std::string programName;
		const XXHash64 hasher = XXHash64(0);

	private:
		std::unordered_map<UINT, Shader> shaders;

		std::string shaderTypeToString(UINT type);
		void displayShaderCompilationResult(UINT type, UINT id);
		UINT compileShaderSource(UINT type, const std::string& source);
		void readShaderSource(std::string& shaderSource, const std::string& filePath);

	public:
		GpuProgram();
		GpuProgram(const std::string& _programName);
		UINT getProgramId() { return programId; }
		std::string getProgramName() { return programName; }

		void addShader(UINT type, const std::string& filePath);
		void handleShaderSourceChange();
		void bind() const { glUseProgram(programId); }
		void unBind() const { glUseProgram(0); }

		void setVec3(const std::string& name, glm::vec3 value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setMat4(const std::string& name, glm::mat4 value) const;
		void setFloat(const std::string& name, float value) const;
		void setInt(const std::string& name, int value) const;

		virtual void createProgram();
	};
}


