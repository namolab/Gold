#pragma once
#include <glm/glm.hpp>

namespace Gold {

    struct AABB
    {
        glm::vec2 size;
        glm::vec2 halfSize;
        glm::vec2 origin;
        float rotation;

        AABB() = default;

        AABB(glm::vec2 sizeP, glm::vec2 originP, float rotationP)
            :size(sizeP), halfSize(size * 0.5f), origin(originP), rotation(rotationP)
        {
        }

        glm::vec2 GetMin()
        {
            return origin - halfSize;
        }

        glm::vec2 GetMax() 
        {
            return origin + halfSize;
        }
    };

    struct Line2D
    {
        glm::vec2 from;
        glm::vec2 to;

        Line2D() = default;

        Line2D(glm::vec2 fromP, glm::vec2 toP)
            :from(fromP), to(toP)
        {
        }

        float lengthSquared() 
        {
            return glm::length(to - from)* glm::length(to - from);
        }
    };

}