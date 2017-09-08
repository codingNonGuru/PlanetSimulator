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

enum class Meshes
{
	QUAD,
	SPACESHIP_SCOUT,
	SPACESHIP_CORVETTE,
	SHELL,
	COUNT
};

class Vertex
{
public:
	glm::vec2 position_;
	glm::vec2 coordinates_;
	float weight_;
};

class Mesh {
public:
	GLuint vertexArrayObjectIndex_;
	Vertex* vertices_;
	GLuint* indices_;
	GLuint elementCount_;

	Mesh();
	void initialize(Meshes);
	void draw(glm::mat4&, glm::mat4&);
	glm::vec2* GetPosition(int);
	virtual ~Mesh();
};

class MeshAtlas {
public:
	Mesh meshes_[(int)Meshes::COUNT];

	void initialize(Meshes);
	Mesh& operator[](Meshes mesh) {return meshes_[(int)mesh];}
};

#endif /* MESH_HPP_ */
