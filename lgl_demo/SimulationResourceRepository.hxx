#pragma once

#include <eng/ResourceRepository.hxx>

namespace lgl {
	class SimulationResourceRepository : public ResourceRepository {
	public:
		SimulationResourceRepository();
		virtual void initializeResources() override;
	};
}