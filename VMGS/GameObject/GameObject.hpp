#ifndef _INCLUDE_GAME_OBJECT_H
#define _INCLUDE_GAME_OBJECT_H

#include "Cube/cube.hpp"

namespace VM76 {
	class Object {
	public:
		glm::vec3 pos = glm::vec3(0.0);
		glm::vec3 rotation = glm::vec3(0.0);
		glm::vec3 scale = glm::vec3(1.0);

	public:
		glm::mat4 transform();

		Object();
		Object(glm::vec3 position);
		Object(glm::vec3 position, glm::vec3 rotation);
		Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	};
}

#endif
