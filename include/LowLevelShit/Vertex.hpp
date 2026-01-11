#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad/gl.h>

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // default white
	glm::vec2 texture = {0.f, 0.f};

	static constexpr GLsizei stride() noexcept {
		return static_cast<GLsizei>(
			sizeof(Vertex)
		);
	}
};
