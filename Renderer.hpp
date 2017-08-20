#pragma once

#include <glm.hpp>
#include "Memory.hpp"

class Scene;
class Buffer;
class ShaderMap;

class Renderer {
	static container::Array<glm::vec3> positionBuffer_;
	static container::Array<float> scaleBuffer_;
	static container::Array<float> rotationBuffer_;
	static Buffer* bodyBuffer_;
	static ShaderMap* shaderMap_;
	static glm::mat4 matrix_;
	static float zoomFactor_;

public:
	static void Initialize();
	static void Draw(Scene*);
	static ShaderMap*& GetMap() {return shaderMap_;}
	static float GetZoomFactor() {return zoomFactor_;}
};
