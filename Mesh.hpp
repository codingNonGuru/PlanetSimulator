/*
 * Mesh.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: andrei
 */

#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm.hpp>

typedef unsigned int GLuint;

enum class Meshes {
	GENERIC_QUAD, SPACESHIP, PROJECTILE, COUNT
};

class VertexSet {
public:
	glm::vec2* vertices_;
	GLuint* indices_;
	GLuint* boneKeys_;
};

class Mesh {
public:
	GLuint vertexArrayObjectIndex_;
	VertexSet vertexSet_;
	GLuint elementCount_;

	Mesh();
	void initialize(Meshes);
	void draw(glm::mat4&, glm::mat4&);
	glm::vec2* GetVertex(int);
	virtual ~Mesh();
};

class MeshAtlas {
public:
	Mesh meshes_[(int)Meshes::COUNT];

	void initialize(Meshes);
	Mesh& operator[](Meshes mesh) {return meshes_[(int)mesh];}
};

#endif /* MESH_HPP_ */
