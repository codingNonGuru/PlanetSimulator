/*
 * Mesh.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: andrei
 */
#include <stdlib.h>
#include <GL/glew.h>

#include "Mesh.hpp"

Mesh::Mesh()
{
}

GLuint indexStencil[] = {0, 1, 2, 1, 2, 3};

void traceLine(int lineIndex, glm::vec2 start, glm::vec2 end, VertexSet* vertexSet, float thickness, GLuint boneKey) {
	glm::vec2 line = start - end;
	glm::vec2 right(line.y, -line.x);
	right = glm::normalize(right) * (thickness * 0.5f);
	int vertexOffset = lineIndex * 4;
	int indexOffset = lineIndex * 6;
	vertexSet->vertices_[vertexOffset + 0] = start + right;
	vertexSet->vertices_[vertexOffset + 1] = start - right;
	vertexSet->vertices_[vertexOffset + 2] = end + right;
	vertexSet->vertices_[vertexOffset + 3] = end - right;
	for(int i = 0; i < 6; ++i) {
		vertexSet->indices_[indexOffset + i] = vertexOffset + indexStencil[i];
		vertexSet->boneKeys_[indexOffset + i] = boneKey;
	}
}

void Mesh::initialize(Meshes mesh) {
	int vertexCount, indexCount;
	if(mesh == Meshes::SPACESHIP_SCOUT)
	{
		vertexCount = 3;
		indexCount = 3;
	}
	else if(mesh == Meshes::SPACESHIP_CORVETTE)
	{
		vertexCount = 4;
		indexCount = 6;
	}
	else if(mesh == Meshes::GENERIC_QUAD)
	{
		vertexCount = 4;
		indexCount = 6;
	}

	elementCount_ = indexCount;
	vertexSet_.vertices_ = (glm::vec2*)malloc(sizeof(glm::vec2) * vertexCount);
	vertexSet_.indices_ = (GLuint*)malloc(sizeof(GLuint) * indexCount);
	vertexSet_.boneKeys_ = (GLuint*)malloc(sizeof(GLuint) * indexCount);

	if(mesh == Meshes::SPACESHIP_SCOUT) {
		*(vertexSet_.vertices_ + 0) = glm::vec2(0.5f, 0.0f);
		*(vertexSet_.vertices_ + 1) = glm::vec2(-0.25f, 0.3f);
		*(vertexSet_.vertices_ + 2) = glm::vec2(-0.25f, -0.3f);
		vertexSet_.indices_[0] = 0;
		vertexSet_.indices_[1] = 1;
		vertexSet_.indices_[2] = 2;
	}
	else if(mesh == Meshes::SPACESHIP_CORVETTE) {
		*(vertexSet_.vertices_ + 0) = glm::vec2(0.9f, 0.0f);
		*(vertexSet_.vertices_ + 1) = glm::vec2(-0.3f, 0.45f);
		*(vertexSet_.vertices_ + 2) = glm::vec2(-0.3f, -0.45f);
		*(vertexSet_.vertices_ + 3) = glm::vec2(-0.6f, 0.0f);
		vertexSet_.indices_[0] = 0;
		vertexSet_.indices_[1] = 1;
		vertexSet_.indices_[2] = 2;
		vertexSet_.indices_[3] = 1;
		vertexSet_.indices_[4] = 2;
		vertexSet_.indices_[5] = 3;
	}
	else if(mesh == Meshes::GENERIC_QUAD) {
		*(vertexSet_.vertices_ + 0) = glm::vec2(-0.5f, -0.5f);
		*(vertexSet_.vertices_ + 1) = glm::vec2(0.5f, -0.5f);
		*(vertexSet_.vertices_ + 2) = glm::vec2(0.5f, 0.5f);
		*(vertexSet_.vertices_ + 3) = glm::vec2(-0.5f, 0.5f);
		vertexSet_.indices_[0] = 0;
		vertexSet_.indices_[1] = 1;
		vertexSet_.indices_[2] = 2;
		vertexSet_.indices_[3] = 0;
		vertexSet_.indices_[4] = 2;
		vertexSet_.indices_[5] = 3;
	}

	GLuint key;
	glGenVertexArrays(1, &vertexArrayObjectIndex_);
	glBindVertexArray(vertexArrayObjectIndex_);

	glGenBuffers(1, &key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexCount, vertexSet_.vertices_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * indexCount, vertexSet_.boneKeys_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &key);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, key);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, vertexSet_.indices_, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(glm::mat4 &viewMatrix, glm::mat4 &worldMatrix) {
	glBindVertexArray(vertexArrayObjectIndex_);
	glUniformMatrix4fv(0, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, elementCount_, GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
}

glm::vec2* Mesh::GetVertex(int index)
{
	return vertexSet_.vertices_ + *(vertexSet_.indices_ + index);
}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

void MeshAtlas::initialize(Meshes mesh) {
	meshes_[(int)mesh].initialize(mesh);
}

