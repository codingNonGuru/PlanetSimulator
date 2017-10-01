#pragma once

#include <glm.hpp>
#include "Memory.hpp"
#include "Conventions.hpp"

class Scene;
class Interface;
class MasterBuffer;
class ShaderMap;
class Texture;
class FramebufferAtlas;

class Renderer {
	static container::Array<Position> positionBuffer_;
	static container::Array<Scale> scaleBuffer_;
	static container::Array<Rotation> rotationBuffer_;
	static container::Array<float> highlightBuffer_;
	static container::Array<int> resolutionBuffer_;
	static container::Array<float> contrastBuffer_;
	static container::Array<glm::vec2> offsetBuffer_;
	static MasterBuffer* bodyBuffer_;
	static ShaderMap* shaderMap_;
	static glm::mat4 matrix_;
	static float zoomFactor_;
	static FramebufferAtlas* frameBuffers_;

public:
	static void Initialize(Scene*);
	static void DrawScene(Scene*);
	static void DrawInterface(Interface*);
	static ShaderMap*& GetMap() {return shaderMap_;}
	static float GetZoomFactor() {return zoomFactor_;}
	static void SetZoomFactor(float zoomFactor) {zoomFactor_ = zoomFactor;}
	static Texture* perlinTexture_;
};
