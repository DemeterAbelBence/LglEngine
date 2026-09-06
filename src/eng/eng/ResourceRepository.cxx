#include "ResourceRepository.hxx"

void lgl::ResourceRepository::handleShaderChanges() {
	for (const auto& program : m_programs) {
		program.second->handleShaderSourceChange();
	}
}