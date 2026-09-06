#include "SimulationResourceRepository.hxx"

lgl::SimulationResourceRepository::SimulationResourceRepository() : ResourceRepository() {
	initializeResources();
}

void lgl::SimulationResourceRepository::initializeResources() {
	try {
		// PATCH PHONG PROGRAM
		auto patchPhong = utl::makeSptr<GpuProgram>("PATCH_PHONG");
		patchPhong->addShader(GL_FRAGMENT_SHADER, "shaders/patch/patch-frag-phong.glsl");
		patchPhong->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchPhong->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchPhong->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchPhong->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchPhong->createProgram();
		m_programs[patchPhong->getProgramName()] = patchPhong;

		// PATCH DEPTH PROGRAM
		auto patchDepth = utl::makeSptr<GpuProgram>("PATCH_DEPTH");
		patchDepth->addShader(GL_FRAGMENT_SHADER, "shaders/basic/depth-frag.glsl");
		patchDepth->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchDepth->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchDepth->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchDepth->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchDepth->createProgram();
		m_programs[patchDepth->getProgramName()] = patchDepth;

		// PATCH PBR PROGRAM
		auto patchPBR = utl::makeSptr<GpuProgram>("PATCH_PBR");
		patchPBR->addShader(GL_FRAGMENT_SHADER, "shaders/patch/patch-frag-pbr.glsl");
		patchPBR->addShader(GL_VERTEX_SHADER, "shaders/patch/patch-vert.glsl");
		patchPBR->addShader(GL_TESS_CONTROL_SHADER, "shaders/patch/patch-tess-cont.glsl");
		patchPBR->addShader(GL_TESS_EVALUATION_SHADER, "shaders/patch/patch-tess-eval.glsl");
		patchPBR->addShader(GL_GEOMETRY_SHADER, "shaders/patch/patch-geom.glsl");
		patchPBR->createProgram();
		m_programs[patchPBR->getProgramName()] = patchPBR;

		// PHONG PROGRAM
		auto phong = utl::makeSptr<GpuProgram>("PHONG");
		phong->addShader(GL_VERTEX_SHADER, "shaders/phong/basic-phong-vert.glsl");
		phong->addShader(GL_FRAGMENT_SHADER, "shaders/phong/basic-phong-frag.glsl");
		phong->createProgram();
		m_programs[phong->getProgramName()] = phong;

		auto mPhong = utl::makeSptr<GpuProgram>("MPHONG");
		mPhong->addShader(GL_VERTEX_SHADER, "shaders/phong/model-phong-vert.glsl");
		mPhong->addShader(GL_FRAGMENT_SHADER, "shaders/phong/model-phong-frag.glsl");
		mPhong->createProgram();
		m_programs[mPhong->getProgramName()] = mPhong;

		// TEX PROGRAM
		auto texProgram = utl::makeSptr<GpuProgram>("TEX");
		texProgram->addShader(GL_FRAGMENT_SHADER, "shaders/basic/tex-frag.glsl");
		texProgram->addShader(GL_VERTEX_SHADER, "shaders/basic/tex-vert.glsl");
		texProgram->createProgram();
		m_programs[texProgram->getProgramName()] = texProgram;

		// SUN TEXTURE
		m_textures["SUN"] = utl::makeSptr<Texture>(Texture{
			tex::TextureFromFile("assets/images/sun.jpg"),
			"sun_texture", ""
			});

		m_textures["WOOD"] = utl::makeSptr<Texture>(Texture{
			tex::TextureFromFile("assets/images/box1.jpg"),
			"texture_base_color", ""
			});

		// WHITE MATERIAL
		m_materials["WHITE"] = utl::makeSptr<Material>(Material{
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			10.0f
			});

		// CUBE MODEL MESH
		auto cubeModel = utl::makeSptr<ModelMesh>("assets/cube/Cube_obj.obj", true, true);
		cubeModel->scaleModelBy(glm::vec3(0.007f, 0.007f, 0.007f));
		m_modelMeshes["IRONBOX"] = cubeModel;

		// DEPTH BUFFER
		m_buffers["DEPTH"] = utl::makeSptr<FrameBuffer>("DEPTH_MAP", 800, 800,
			GL_DEPTH_COMPONENT24, GL_FLOAT, GL_DEPTH_COMPONENT);
	}
	catch (const utl::except& error) {
		Logger::log(Logger::LGL_ERROR, "Simulation resource exception: {}\n", error.what());
	}
	catch (...) {
		Logger::log(Logger::LGL_ERROR, "Simulation resource exception\n");
	}
}
