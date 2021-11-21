#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Gold {

	class Light
	{
	public:
		Light() = default;

		Light(glm::vec3 position, glm::vec3 color)
			:Position(position), Color(color)
		{
		
		}

		glm::vec3 GetPosition()
		{
			return Position;
		}

		void SetPosition(glm::vec3 position)
		{
			Position = position;
		}

		glm::vec3 GetColor()
		{
			return Color;
		}

		void SetColor(glm::vec3 color)
		{
			Color = color;
		}

	private:
		glm::vec3 Position;
		glm::vec3 Color;

	};

}
