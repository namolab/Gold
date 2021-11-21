#pragma once

#include <glm/glm.hpp>

#include "Gold/Core/KeyCodes.h"
#include "Gold/Core/MouseCodes.h"

namespace Gold {

	class Input
	{

	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private:
		static Input* s_Instance;

	};
}