#include "SimulationResourceRepository.hxx"

lgl::SimulationResourceRepository::SimulationResourceRepository() : ResourceRepository() { initializeResources(); }

void lgl::SimulationResourceRepository::initializeResources() {
  try {
    // PATCH PHONG PROGRAM
    auto patchPhong = std::make_shared<GpuProgram>("PATCH_PHONG");
    patchPhong->addShader(GL_FRAGMENT_SHADER,
                          std::format("{}/shaders/patch/patch-frag-phong.glsl", utl::lglResourceFolder));
    patchPhong->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/patch/patch-vert.glsl", utl::lglResourceFolder));
    patchPhong->addShader(GL_TESS_CONTROL_SHADER,
                          std::format("{}/shaders/patch/patch-tess-cont.glsl", utl::lglResourceFolder));
    patchPhong->addShader(GL_TESS_EVALUATION_SHADER,
                          std::format("{}/shaders/patch/patch-tess-eval.glsl", utl::lglResourceFolder));
    patchPhong->addShader(GL_GEOMETRY_SHADER, std::format("{}/shaders/patch/patch-geom.glsl", utl::lglResourceFolder));
    patchPhong->createProgram();
    m_programs[patchPhong->getProgramName()] = patchPhong;

    // PATCH DEPTH PROGRAM
    auto patchDepth = std::make_shared<GpuProgram>("PATCH_DEPTH");
    patchDepth->addShader(GL_FRAGMENT_SHADER, std::format("{}/shaders/basic/depth-frag.glsl", utl::lglResourceFolder));
    patchDepth->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/patch/patch-vert.glsl", utl::lglResourceFolder));
    patchDepth->addShader(GL_TESS_CONTROL_SHADER,
                          std::format("{}/shaders/patch/patch-tess-cont.glsl", utl::lglResourceFolder));
    patchDepth->addShader(GL_TESS_EVALUATION_SHADER,
                          std::format("{}/shaders/patch/patch-tess-eval.glsl", utl::lglResourceFolder));
    patchDepth->addShader(GL_GEOMETRY_SHADER, std::format("{}/shaders/patch/patch-geom.glsl", utl::lglResourceFolder));
    patchDepth->createProgram();
    m_programs[patchDepth->getProgramName()] = patchDepth;

    // PATCH PBR PROGRAM
    auto patchPBR = std::make_shared<GpuProgram>("PATCH_PBR");
    patchPBR->addShader(GL_FRAGMENT_SHADER,
                        std::format("{}/shaders/patch/patch-frag-pbr.glsl", utl::lglResourceFolder));
    patchPBR->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/patch/patch-vert.glsl", utl::lglResourceFolder));
    patchPBR->addShader(GL_TESS_CONTROL_SHADER,
                        std::format("{}/shaders/patch/patch-tess-cont.glsl", utl::lglResourceFolder));
    patchPBR->addShader(GL_TESS_EVALUATION_SHADER,
                        std::format("{}/shaders/patch/patch-tess-eval.glsl", utl::lglResourceFolder));
    patchPBR->addShader(GL_GEOMETRY_SHADER, std::format("{}/shaders/patch/patch-geom.glsl", utl::lglResourceFolder));
    patchPBR->createProgram();
    m_programs[patchPBR->getProgramName()] = patchPBR;

    // PHONG PROGRAM
    auto phong = std::make_shared<GpuProgram>("PHONG");
    phong->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/phong/basic-phong-vert.glsl", utl::lglResourceFolder));
    phong->addShader(GL_FRAGMENT_SHADER, std::format("{}/shaders/phong/basic-phong-frag.glsl", utl::lglResourceFolder));
    phong->createProgram();
    m_programs[phong->getProgramName()] = phong;

    auto mPhong = std::make_shared<GpuProgram>("MPHONG");
    mPhong->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/phong/model-phong-vert.glsl", utl::lglResourceFolder));
    mPhong->addShader(GL_FRAGMENT_SHADER,
                      std::format("{}/shaders/phong/model-phong-frag.glsl", utl::lglResourceFolder));
    mPhong->createProgram();
    m_programs[mPhong->getProgramName()] = mPhong;

    // TEX PROGRAM
    auto texProgram = std::make_shared<GpuProgram>("TEX");
    texProgram->addShader(GL_FRAGMENT_SHADER, std::format("{}/shaders/basic/tex-frag.glsl", utl::lglResourceFolder));
    texProgram->addShader(GL_VERTEX_SHADER, std::format("{}/shaders/basic/tex-vert.glsl", utl::lglResourceFolder));
    texProgram->createProgram();
    m_programs[texProgram->getProgramName()] = texProgram;

    // SUN TEXTURE
    m_textures["SUN"] = std::make_shared<Texture>(Texture{
        tex::TextureFromFile(std::format("{}/assets/images/sun.jpg", utl::lglResourceFolder)), "sun_texture", ""});

    m_textures["WOOD"] = std::make_shared<Texture>(
        Texture{tex::TextureFromFile(std::format("{}/assets/images/box1.jpg", utl::lglResourceFolder)),
                "texture_base_color", ""});

    // WHITE MATERIAL
    m_materials["WHITE"] = std::make_shared<Material>(
        Material{glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 10.0f});

    // CUBE MODEL MESH
    auto cubeModel =
        std::make_shared<ModelMesh>(std::format("{}/assets/cube/Cube_obj.obj", utl::lglResourceFolder), true, true);
    cubeModel->scaleModelBy(glm::vec3(0.007f, 0.007f, 0.007f));
    m_modelMeshes["IRONBOX"] = cubeModel;

    // DEPTH BUFFER
    m_buffers["DEPTH"] =
        std::make_shared<FrameBuffer>("DEPTH_MAP", 800, 800, GL_DEPTH_COMPONENT24, GL_FLOAT, GL_DEPTH_COMPONENT);
  } catch (const std::exception& error) {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "Simulation resource exception: {}\n", error.what());
  } catch (...) {
    utl::log(utl::CONSOLE_LOGS, utl::LGL_ERROR, "Simulation resource exception\n");
  }
}
