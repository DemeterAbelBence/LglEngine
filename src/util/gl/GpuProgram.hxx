#pragma once

import lglmodule;
import glmmodule;

#include <XXHASH/xxhash64.h>
#include <GLEW/glew.h>

namespace lgl {
	struct Shader {
	public:
		utl::str m_source;
		utl::str m_path;
		utl::ull m_hash;

	public:
		Shader() : m_source(""), m_path(""), m_hash(0) {}
		bool empty() const { return m_source.empty();}
	};

	class GpuProgram {
	private:
		

	private:
		utl::uint m_programId;
		utl::str m_programName;
		const XXHash64 m_hasher = XXHash64(0);

	private:
		utl::imap<Shader> m_shaders;

		utl::str shaderTypeToString(utl::uint type);
		void displayShaderCompilationResult(utl::uint type, utl::uint id);
		utl::uint compileShaderSource(utl::uint type, const utl::str& source);
		void readShaderSource(utl::str& shaderSource, const utl::str& filePath);

	public:
		GpuProgram();
		GpuProgram(const utl::str& programName);
		utl::uint getProgramId() { return m_programId; }
		utl::str getProgramName() { return m_programName; }

		void addShader(utl::uint type, const utl::str& filePath);
		void handleShaderSourceChange();
		void bind() const;
		void unBind() const;

		void setVec3(const utl::str& name, glm::vec3 value) const;
		void setVec3(const utl::str& name, float x, float y, float z) const;
		void setMat4(const utl::str& name, glm::mat4 value) const;
		void setFloat(const utl::str& name, float value) const;
		void setInt(const utl::str& name, int value) const;
		virtual void createProgram();
	};
}


