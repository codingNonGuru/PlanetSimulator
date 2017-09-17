/*
 * Mesh.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: andrei
 */
#include <stdlib.h>
#include <stddef.h>
#include <GL/glew.h>

#include "Mesh.hpp"

Mesh::Mesh()
{
}

GLuint indexStencil[] = {0, 1, 2, 1, 2, 3};

void traceLine(int lineIndex, glm::vec2 start, glm::vec2 end, float thickness, GLuint boneKey) {
	/*glm::vec2 line = start - end;
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
	}*/
}

void Mesh::initialize(Meshes mesh) {
	int vertexCount, indexCount;
	if(mesh == Meshes::SHIP_SCOUT)
	{
		vertexCount = 3;
		indexCount = 3;
	}
	else if(mesh == Meshes::SHIP_CORVETTE)
	{
		vertexCount = 4;
		indexCount = 6;
	}
	else if(mesh == Meshes::SHIP_BARGE)
	{
		vertexCount = 4;
		indexCount = 6;
	}
	else if(mesh == Meshes::QUAD)
	{
		vertexCount = 4;
		indexCount = 6;
	}
	else if(mesh == Meshes::SHELL)
	{
		vertexCount = 12;
		indexCount = 36;
	}
	else if(mesh == Meshes::DOCK)
	{
		vertexCount = 7;
		indexCount = 18;
	}

	elementCount_ = indexCount;
	vertices_ = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
	indices_ = (GLuint*)malloc(sizeof(GLuint) * indexCount);

	if(mesh == Meshes::SHIP_SCOUT) {
		vertices_[0].position_ = glm::vec2(0.5f, 0.0f);
		vertices_[1].position_ = glm::vec2(-0.25f, 0.3f);
		vertices_[2].position_ = glm::vec2(-0.25f, -0.3f);
		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 2;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}
	else if(mesh == Meshes::SHIP_CORVETTE) {
		vertices_[0].position_ = glm::vec2(0.9f, 0.0f);
		vertices_[1].position_ = glm::vec2(-0.3f, 0.45f);
		vertices_[2].position_ = glm::vec2(-0.3f, -0.45f);
		vertices_[3].position_ = glm::vec2(-0.6f, 0.0f);
		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 2;
		indices_[3] = 1;
		indices_[4] = 2;
		indices_[5] = 3;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}
	else if(mesh == Meshes::SHIP_BARGE) {
		vertices_[0].position_ = glm::vec2(2.0f, 0.65f);
		vertices_[3].position_ = glm::vec2(2.0f, -0.65f);
		vertices_[2].position_ = glm::vec2(-2.0f, -0.8f);
		vertices_[1].position_ = glm::vec2(-2.0f, 0.8f);
		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 2;
		indices_[3] = 0;
		indices_[4] = 2;
		indices_[5] = 3;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}
	else if(mesh == Meshes::QUAD) {
		vertices_[0].position_ = glm::vec2(-0.5f, -0.5f);
		vertices_[1].position_ = glm::vec2(0.5f, -0.5f);
		vertices_[2].position_ = glm::vec2(0.5f, 0.5f);
		vertices_[3].position_ = glm::vec2(-0.5f, 0.5f);
		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 2;
		indices_[3] = 0;
		indices_[4] = 2;
		indices_[5] = 3;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}
	else if(mesh == Meshes::SHELL)
	{
		vertices_[0].position_ = glm::vec2(-0.45f, 0.3f);
		vertices_[1].position_ = glm::vec2(-0.15f, 0.3f);
		vertices_[2].position_ = glm::vec2(-0.45f, 0.0f);
		vertices_[3].position_ = glm::vec2(-0.15f, 0.0f);
		vertices_[4].position_ = glm::vec2(-0.45f, -0.3f);
		vertices_[5].position_ = glm::vec2(-0.15f, -0.3f);

		vertices_[6].position_ = glm::vec2(0.15f, 0.3f);
		vertices_[7].position_ = glm::vec2(0.45f, 0.3f);
		vertices_[8].position_ = glm::vec2(0.15f, 0.0f);
		vertices_[9].position_ = glm::vec2(0.45f, 0.0f);
		vertices_[10].position_ = glm::vec2(0.15f, -0.3f);
		vertices_[11].position_ = glm::vec2(0.45f, -0.3f);

		vertices_[0].coordinates_ = glm::vec2(-1.0f, 1.0f);
		vertices_[1].coordinates_ = glm::vec2(0.0f, 1.0f);
		vertices_[2].coordinates_ = glm::vec2(-1.0f, 0.0f);
		vertices_[3].coordinates_ = glm::vec2(0.0f, 0.0f);
		vertices_[4].coordinates_ = glm::vec2(-1.0f, -1.0f);
		vertices_[5].coordinates_ = glm::vec2(0.0f, -1.0f);

		vertices_[6].coordinates_ = glm::vec2(0.0f, 1.0f);
		vertices_[7].coordinates_ = glm::vec2(1.0f, 1.0f);
		vertices_[8].coordinates_ = glm::vec2(0.0f, 0.0f);
		vertices_[9].coordinates_ = glm::vec2(1.0f, 0.0f);
		vertices_[10].coordinates_ = glm::vec2(0.0f, -1.0f);
		vertices_[11].coordinates_ = glm::vec2(1.0f, -1.0f);

		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 2;
		indices_[3] = 1;
		indices_[4] = 2;
		indices_[5] = 3;

		indices_[6] = 2;
		indices_[7] = 3;
		indices_[8] = 5;
		indices_[9] = 2;
		indices_[10] = 4;
		indices_[11] = 5;

		indices_[12] = 6;
		indices_[13] = 7;
		indices_[14] = 9;
		indices_[15] = 6;
		indices_[16] = 8;
		indices_[17] = 9;

		indices_[18] = 8;
		indices_[19] = 9;
		indices_[20] = 10;
		indices_[21] = 9;
		indices_[22] = 10;
		indices_[23] = 11;

		indices_[24] = 1;
		indices_[25] = 6;
		indices_[26] = 3;
		indices_[27] = 6;
		indices_[28] = 3;
		indices_[29] = 8;

		indices_[30] = 3;
		indices_[31] = 8;
		indices_[32] = 5;
		indices_[33] = 8;
		indices_[34] = 5;
		indices_[35] = 10;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}
	else if(mesh == Meshes::DOCK)
	{
		float angle = 0.0f;
		for(int i = 0; i < 6; ++i)
		{
			vertices_[i].position_ = glm::vec2(cos(angle), sin(angle)) * 0.5f;
			angle += 6.2831f / 6.0f;
		}

		vertices_[6].position_ = glm::vec2(0.0f, 0.0f);

		indices_[0] = 0;
		indices_[1] = 1;
		indices_[2] = 6;

		indices_[3] = 1;
		indices_[4] = 2;
		indices_[5] = 6;

		indices_[6] = 2;
		indices_[7] = 3;
		indices_[8] = 6;

		indices_[9] = 3;
		indices_[10] = 4;
		indices_[11] = 6;

		indices_[12] = 4;
		indices_[13] = 5;
		indices_[14] = 6;

		indices_[15] = 5;
		indices_[16] = 0;
		indices_[17] = 6;

		for(auto vertex = vertices_; vertex != vertices_ + vertexCount; ++vertex)
		{
			vertex->weight_ = 1.0f;
		}
	}

	GLuint key;
	glGenVertexArrays(1, &vertexArrayObjectIndex_);
	glBindVertexArray(vertexArrayObjectIndex_);

	glGenBuffers(1, &key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Vertex::position_));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Vertex::coordinates_));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Vertex::weight_));

	glGenBuffers(1, &key);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, key);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, indices_, GL_STATIC_DRAW);

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

glm::vec2* Mesh::GetPosition(int index)
{
	return &(vertices_ + *(indices_ + index))->position_;
}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

void MeshAtlas::initialize(Meshes mesh) {
	meshes_[(int)mesh].initialize(mesh);
}

