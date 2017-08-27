#pragma once

#include <glm.hpp>
#include "Memory.hpp"

class Scene;
class Buffer;
class ShaderMap;
class Texture;

class Renderer {
	static container::Array<glm::vec3> positionBuffer_;
	static container::Array<float> scaleBuffer_;
	static container::Array<float> rotationBuffer_;
	static container::Array<float> highlightBuffer_;
	static container::Array<int> resolutionBuffer_;
	static container::Array<float> contrastBuffer_;
	static container::Array<glm::vec2> offsetBuffer_;
	static Buffer* bodyBuffer_;
	static ShaderMap* shaderMap_;
	static glm::mat4 matrix_;
	static float zoomFactor_;

public:
	static void Initialize(Scene*);
	static void Draw(Scene*);
	static ShaderMap*& GetMap() {return shaderMap_;}
	static float GetZoomFactor() {return zoomFactor_;}
	static Texture* perlinTexture_;
};
