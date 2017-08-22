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

	float biasedRandom(float minValue, float maxValue, float expValue, float standDev) {
		float value = getRandom(minValue, maxValue);
		float gaussCh = 1.0f - exp(-pow(value - expValue, 2.0f) / (2.0f * pow(standDev, 2.0f)));
		if(throwChance<float>(gaussCh) == true)
			return biasedRandom(minValue, maxValue, expValue, standDev);
			//return random_f(minValue, maxValue);
		else
			return value;
	}
}
