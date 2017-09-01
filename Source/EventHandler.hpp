#pragma once

#include <glm.hpp>
#include "Memory.hpp"

class Engine;

struct Mouse {
	glm::vec2 position_;
	bool currentRight_, currentLeft_;
	bool scrollUp_, scrollDown_;
	bool lastRight_, lastLeft_;
	Mouse() : position_(glm::vec2(0.0f, 0.0f)), currentRight_(false), currentLeft_(false), lastRight_(false), lastLeft_(false), scrollUp_(false), scrollDown_(false) {}
};

class EventHandler {
public:
	static Mouse mouse_;
	static int keyCount_;
	static container::Array<int> currentKeys_;
	static container::Array<int> formerKeys_;

	static void receiveInput();
	static void processInput();
	static void update();
	static bool isKeyPressed(int32_t);
	static bool wasKeyPressed(int32_t);
	static void Initialize();
};
