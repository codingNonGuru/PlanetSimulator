#pragma once

#include "Memory.hpp"

typedef glm::vec2 Range;

class Shader;

class Voronoi {
public:
	static Shader* shader_;
	static GLuint seedBuffer_, neighbourBuffer_, neighbourSwapBuffer_, resultBuffer_, resultSwapBuffer_, filterBuffer_;

	static void initialize();
	static void destroy();
	static void generate(bool, container::Grid<float> &, container::Grid<unsigned int> *, unsigned int);
	static void loadSeeds(container::Grid<unsigned int> &);
	static void loadRandomTriangulation(int32_t, int32_t, uint32_t &);
};

class Perlin {
public:
	static Shader* shader_;
	static GLuint octavesBuffer_, resultBuffer_, streamBuffer_;

	static void initialize();
	static void destroy();
	static void generate(bool, container::Grid<float> &, Range, float, float, float, float);
	static GLuint GetResultBuffer() {return resultBuffer_;}
};
