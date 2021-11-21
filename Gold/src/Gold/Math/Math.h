#pragma once

#include <glm/glm.hpp>

#include <cmath>

namespace Gold::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	void GDRotateVector2D(glm::vec2& vec, float rotation, glm::vec2& origin);

	bool compare(float x, float y, float epsilon)
	{
		return abs(x - y) <= epsilon * fmax(1.0f, fmax(abs(x), abs(y)));
	}

	bool compare(glm::vec2 vec1, glm::vec2 vec2, float epsilon)
	{
		return compare(vec1.x, vec2.x, epsilon) && compare(vec1.y, vec2.y, epsilon);
	}

	bool compare(float x, float y) 
	{
		return abs(x - y) <=FLT_MIN* fmax(1.0f, fmax(abs(x), abs(y)));
	}

	bool compare(glm::vec2 vec1, glm::vec2 vec2)
	{
		return compare(vec1.x, vec2.x) && compare(vec1.y, vec2.y);
	}

}