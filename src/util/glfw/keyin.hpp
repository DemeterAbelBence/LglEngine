#pragma once

#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
	namespace keyin {
		class LGL_API Key {
		private:
			UINT m_id;
			bool m_pressed;

		public:
			Key();
			Key(UINT id);
			bool handlePress(GLFWwindow* window);
			bool handleRelease(GLFWwindow* window);

		private:
			UINT getState(GLFWwindow* window);
		};

		// - - - - - - - - - - - - - - - - - - - - - - - - - - // 

		struct NoArgType {};

		template<typename C, typename A = NoArgType>
		class LGL_API KeyEvent {
		private:
			Key m_key;
			void(C::* m_argOperation)(A);
			void(C::* m_operation)(void);
			A m_arg;
			bool m_onRelease;

		public:
			explicit KeyEvent(Key key, void(C::* operation)(A), A arg, bool onRelease);
			explicit KeyEvent(Key key, void(C::* operation)(void), bool onRelease);
			KeyEvent(const KeyEvent& other);
			KeyEvent& operator=(const KeyEvent& other);
			KeyEvent() = delete;
			A& getArgRef() { return m_arg; }
			void performOperation(C* c) const;
			bool applyKeyLogic(GLFWwindow* window, C* c);
		};

		template<typename C, typename A>
		inline KeyEvent<C, A>::KeyEvent(Key key, void(C::* operation)(A), A arg, bool onRelease) {
			m_key = key;
			m_argOperation = operation;
			m_operation = nullptr;
			m_arg = arg;
			m_onRelease = onRelease;
		}

		template<typename C, typename A>
		inline KeyEvent<C, A>::KeyEvent(Key key, void(C::* operation)(void), bool onRelease) {
			m_key = key;
			m_argOperation = nullptr;
			m_operation = operation;
			m_onRelease = onRelease;
		}

		template<typename C, typename A>
		inline KeyEvent<C, A>::KeyEvent(const KeyEvent<C, A>& other) {
			m_key = other.m_key;
			m_argOperation = other.m_argOperation;
			m_operation = other.m_operation;
			m_arg = other.m_arg;
			m_onRelease = other.m_onRelease;
		}

		template<typename C, typename A>
		inline KeyEvent<C, A>& KeyEvent<C, A>::operator=(const KeyEvent& other) {
			if (this != &other) {
				m_key = other.m_key;
				m_argOperation = other.m_argOperation;
				m_operation = other.m_operation;
				m_arg = other.m_arg;
				m_onRelease = other.m_onRelease;
			}
			return *this;
		}

		template<typename C, typename A>
		inline void KeyEvent<C, A>::performOperation(C* c) const {
			if (m_argOperation != nullptr) {
				(c->*m_argOperation)(m_arg);
			}
			if (m_operation != nullptr) {
				(c->*m_operation)();
			}
		}

		template<typename C, typename A>
		inline bool KeyEvent<C, A>::applyKeyLogic(GLFWwindow* window, C* c) {
			if (m_key.handlePress(window) && !m_onRelease) {
				performOperation(c);
				return true;
			}
			if (m_key.handleRelease(window) && m_onRelease) {
				performOperation(c);
				return true;
			}

			return false;
		}

		template<typename C, typename A>
		class LGL_API KeyEventHandler {
		private:
			std::vector<KeyEvent<C, A>> m_argEvents;
			std::vector<KeyEvent<C>> m_events;

		public:
			KeyEventHandler() {}
			void addEvent(KeyEvent<C, A> argEvent) { m_argEvents.push_back(argEvent); }
			void addEvent(KeyEvent<C> event) { m_events.push_back(event); }
			std::vector<KeyEvent<C, A>>& getArgEvents() { return m_argEvents; }
			void removeAllEvents();
			bool handleEvents(GLFWwindow* window, C* c);
		};

		template<typename C, typename A>
		void KeyEventHandler<C, A>::removeAllEvents() {
			m_argEvents.clear();
			m_events.clear();
		}

		template<typename C, typename A>
		bool KeyEventHandler<C, A>::handleEvents(GLFWwindow* window, C* c) {
			bool eventOccured = false;
			for (auto& event : m_events) {
				eventOccured = event.applyKeyLogic(window, c);
			}
			for (auto& argEvent : m_argEvents) {
				eventOccured = argEvent.applyKeyLogic(window, c);
			}
			return eventOccured;
		}
	}
}