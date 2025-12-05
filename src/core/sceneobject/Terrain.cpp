#include "Terrain.hpp"

using namespace lgl;

void Terrain::readTerrainTexturesPBR() {
	m_mesh->addTexture(std::make_shared<Texture>(Texture{
		tex::TextureFromFile("assets/terrain/terr_base_color.jpg"),
		"texture_base_color"
	}));

	m_mesh->addTexture(std::make_shared<Texture>(Texture{
		tex::TextureFromFile("assets/terrain/terr_normal.jpg"),
		"texture_normal"
	}));

	m_mesh->addTexture(std::make_shared<Texture>(Texture{
		tex::TextureFromFile("assets/terrain/terr_roughness.jpg"),
		"texture_roughness"
	}));

	m_mesh->addTexture(std::make_shared<Texture>(Texture{
		tex::TextureFromFile("assets/terrain/terr_metalness.jpg"),
		"texture_metalness"
	}));

	m_mesh->addTexture(std::make_shared<Texture>(Texture{
		tex::TextureFromFile("assets/terrain/terr_ao.jpg"),
		"texture_ambient_occlusion"
	}));
}

Terrain::Terrain(float size, UINT rezolution) {
	m_terrainData = std::make_unique<TerrainData>(TerrainData{
		0.05f,  // amplitude
		0.1f,   // frequency
		0.18f,  // phase
		6,      // iterations
		64,     // minimal_tesselation
		64,     // maximal_tesselation
		0.0f,   // minimal_distance
		50.0f   // maximal_distance
	});

	m_mesh = std::make_shared<PatchSurface>(size, rezolution);

	m_collider = std::make_shared<TerrainCollider>(
		m_terrainData->amplitude, 
		m_terrainData->frequency, 
		m_terrainData->phase, 
		m_terrainData->iterations
	);

	auto bodyData = ribo::BodyData{
		0.0f,					// InvMass
		glm::mat3(0.0f),        // Ibody
		glm::mat3(0.0f),        // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f, 0.0f, 0.0f),        // force
		glm::vec3(0.0f)                      // torque
	};
	m_physicsSolver = std::make_shared<ribo::PhysicsSolver>(bodyData);

	m_transformation = std::make_shared<Transformation>();
	m_mesh->setTransformation(m_transformation);
	m_mesh->create();
	m_collider->setTransformation(m_transformation.get());

	readTerrainTexturesPBR();
}

void Terrain::setParameters(float a, float f, float p, float i) {
	m_terrainData->amplitude = a;
	m_terrainData->frequency = f;
	m_terrainData->phase = p;
	m_terrainData->iterations = i;
}

void Terrain::setTesselationParameters(UINT min_t, UINT max_t, float min_d, float max_d) {
	m_terrainData->minimal_tesselation = min_t;
	m_terrainData->maximal_tesselation = max_t;
	m_terrainData->minimal_distance = min_d;
	m_terrainData->maximal_distance = max_d;
}

void Terrain::updateTerrainParameters() {
	auto program = m_mesh->getProgram();

	program->bind();
	program->setFloat("amplitude_factor", m_terrainData->amplitude);
	program->setFloat("frequency_factor", m_terrainData->frequency);
	program->setFloat("phase_factor", m_terrainData->phase);
	program->setInt("iterations", m_terrainData->iterations);

	program->setFloat("MIN_DIST", m_terrainData->minimal_distance);
	program->setFloat("MAX_DIST", m_terrainData->maximal_distance);
	program->setInt("MIN_TESS", m_terrainData->minimal_tesselation);
	program->setInt("MAX_TESS", m_terrainData->maximal_tesselation);

	auto collider = std::dynamic_pointer_cast<TerrainCollider>(m_collider);
	collider->setTerrrainAmplitude(m_terrainData->amplitude);
	collider->setTerrainFrequency(m_terrainData->frequency);
	collider->setTerrainPhase(m_terrainData->phase);
	collider->setTerrainIterations(m_terrainData->iterations);
}