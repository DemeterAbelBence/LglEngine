#include "SponzaScene.hpp"

using namespace lgl;

void SponzaScene::initResources() {
	try {
		//SPONZA PBR
		auto sponzaShader = std::make_shared<GpuProgram>("SPONZA_PBR");
		sponzaShader->addShader(GL_VERTEX_SHADER, "shaders/sponza/sponza-pbr-vert.glsl");
		sponzaShader->addShader(GL_FRAGMENT_SHADER, "shaders/sponza/sponza-pbr-frag.glsl");
		sponzaShader->createProgram();
		m_programs[sponzaShader->getProgramName()] = sponzaShader;

		// TEX
		auto texture_shader = std::make_shared<GpuProgram>("TEX");
		texture_shader->addShader(GL_FRAGMENT_SHADER, "shaders/basic/tex-frag.glsl");
		texture_shader->addShader(GL_VERTEX_SHADER, "shaders/basic/tex-vert.glsl");
		texture_shader->createProgram();
		m_programs[texture_shader->getProgramName()] = texture_shader;

		// SUN TEXTURE
		m_textures["SUN"] = std::make_shared<Texture>(Texture{
			tex::TextureFromFile("assets/images/sun.jpg"),
			"sun_texture", ""
		});

		// SPONZA MODEL
		auto sponzaModel = std::make_shared<ModelMesh>("assets/sponza_gltf/glTF/Sponza.gltf", false, true);
		sponzaModel->scaleModelBy(glm::vec3(0.1f, 0.1f, 0.1f));
		m_modelMeshes["SPONZA"] = sponzaModel;
	}
	catch (const std::exception& error) {
		std::cerr << "Exception: " << error.what() << std::endl;
	}

}

void SponzaScene::create() {
	try {
		initResources();
		setObjectMoveSpeed(0.05f);

		m_sun = std::make_shared<Sun>(2.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_sun->m_name = std::string("sun");
		m_sun->setProgram(m_programs.at("TEX"));
		m_sun->setTextures({ m_textures.at("SUN") });
		m_sun->translate(glm::vec3(5.0f, 60.0f, 5.0f));
		m_sceneObjects.push_back(m_sun);

		auto sponza = std::make_shared<Sponza>(1.0f, m_modelMeshes["SPONZA"]);
		sponza->m_name = std::string("sponza");
		sponza->getMesh()->setProgram(m_programs["SPONZA_PBR"]);
		m_sceneObjects.push_back(sponza);

	}
	catch (const std::exception& error) {
		std::cerr << "Exception: " << error.what() << std::endl;
	}
}

void SponzaScene::draw() const {
	for (const auto& s : m_sceneObjects) {
		s->draw(*m_camera);
	}
}

void SponzaScene::update(GLFWwindow* window) {
	if (m_moveCamera) {
		m_cameraEventHandler.handleEvents(window, m_camera.get());
		m_camera->resetView();
		m_camera->resetProjection();
		if (Mouse::s_isCaptured) {
			m_camera->mouseUpdate();
		}
	}
	else {
		m_objectEventHandler.handleEvents(window, m_sceneObjects[m_objectIndex].get());
	}

	for (const auto& program : m_programs) {
		program.second->handleShaderSourceChange();
	}

	m_sun->lightUpScene(m_sceneObjects, FrameBuffer());
}