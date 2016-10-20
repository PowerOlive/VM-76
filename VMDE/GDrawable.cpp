//=============================================================================
// ■ GDrawable.cpp
//-----------------------------------------------------------------------------
//   VMDE中GDrawable的控制与类。
//=============================================================================
#include "GDrawable.hpp"

namespace GDrawable {

	void draw(struct GDrawable* s) {
		GLuint model_location = glGetUniformLocation(main_shader->shaderProgram, "modelMatrix");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(s->model));

		glBindVertexArray(s->VAO);
		glDrawElements(GL_TRIANGLES, s->tri_mesh_count * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void fbind(struct GDrawable* s) {
		glBindVertexArray(s->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, s->VBO);
		for (int i = 8; i < s->vtx_c; i += 9) {
			s->vertices[i] = 1.0d - s->vertices[i];
		}
		glBufferData(GL_ARRAY_BUFFER, s->vtx_c * sizeof(GLfloat), s->vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s->ind_c * sizeof(GLuint), s->indices, GL_DYNAMIC_DRAW);

		size_t vertex_size = (3 + 4 + 2) * sizeof(GLfloat); // X,Y,Z,  R,G,B,A,  S,T
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*) (7 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void update(struct GDrawable* s) {
		glBindVertexArray(s->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, s->VBO);
		void *bufv = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(bufv, s->vertices, s->vtx_c * sizeof(GLfloat));
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->EBO);
		void *bufi = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(bufi, s->indices, s->ind_c * sizeof(GLuint));
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	void dispose(struct GDrawable* s) {
		if (s) {
			xefree(s->vertices);
			xefree(s->indices);
			free(s);
		}
	}

	struct GDrawable* create() {
		struct GDrawable* s = (struct GDrawable*) malloc(sizeof(struct GDrawable));

		glGenVertexArrays(1, &s->VAO);
		glGenBuffers(1, &s->VBO);
		glGenBuffers(1, &s->EBO);
		s->vertices = NULL;
		s->indices = NULL;
		s->model = glm::mat4(1.0f);

		return s;
	}

}
