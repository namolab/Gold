#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "Gold/Physics2d/Primitives/Primitive.h"
#include "Gold/Math/Math.h"

namespace Gold::RigidBody2D 
{
    bool pointInAABB(glm::vec2 point, AABB box)
    {
        glm::vec2 min = box.GetMin();
        glm::vec2 max = box.GetMax();

        return point.x <= max.x && min.x <= point.x &&
            point.y <= max.y && min.y <= point.y;
    }

    bool lineAndAABB(Line2D line, AABB box)
    {
        glm::vec3 betdegree = -1.0f * box.rotation;
        glm::vec2 localStart = line.from;
        glm::vec2 localEnd = line.to;

        Math::GDRotateVector2D(localStart, betdegree, box.origin);
        Math::GDRotateVector2D(localEnd, betdegree, box.origin);

        line.from = localStart;
        line.to = localEnd;

        if (pointInAABB(line.from, box) || pointInAABB(line.to, box)) 
        {
            return true;
        }

        glm::vec2 unitVector = line.to - line.from;
        unitVector = glm::normalize(unitVector);

        unitVector.x = (unitVector.x != 0) ? 1.0f / unitVector.x : 0.0f;
        unitVector.y = (unitVector.y != 0) ? 1.0f / unitVector.y : 0.0f;

        glm::vec2 minvector = box.GetMin();
        minvector = (minvector - line.from) * unitVector;
        glm::vec2 maxvector = box.GetMax();
        maxvector = (maxvector - line.from) * unitVector;

        float tmin = fmax(fmin(minvector.x, maxvector.x),fmin(minvector.y, maxvector.y));
        float tmax = fmin(fmax(minvector.x, maxvector.x),fmax(minvector.y, maxvector.y));

        if (tmax < 0 || tmin > tmax) 
        {
            return false;
        }

        float t = (tmin < 0.0f) ? tmax : tmin;

        return t > 0.0f && t * t < line.lengthSquared();
    }
}