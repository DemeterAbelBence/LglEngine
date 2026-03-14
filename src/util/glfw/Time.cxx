#include "Time.hxx"

namespace lgl {
	void Time::calculateCurrentTime() {
		s_currentTime = glfwGetTime();
	}

	void Time::calculateDeltaTime() {
		s_deltaTime = s_currentTime - s_previousTime;
		s_previousTime = s_currentTime;
	}
}
