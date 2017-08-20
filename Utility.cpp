#include "Utility.hpp"

std::random_device rd;
std::mt19937 mt(rd());

namespace utility
{
	float getRandom(float minimum, float maximum) {
		std::uniform_real_distribution<float> dist(minimum, maximum);
		return dist(mt);
	}

	int getRandom(int min, int max) {
		std::uniform_int_distribution<int> Value(min, max);
		return Value(mt);
	}

	void randomize(float* vector, float minimum, float maximum) {
		std::uniform_real_distribution<float> dist(0.0f, 6.2831f);
		float angle = dist(mt);
		float radius = dist(mt) * maximum + minimum;
		*(vector + 0) = cos(angle) * radius;
		*(vector + 1) = sin(angle) * radius;
		*(vector + 2) = 0.0f;
	}
}
