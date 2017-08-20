#pragma once

#include <math.h>
#include <random>

#include "Memory.hpp"

namespace utility
{
	enum FilterTypes {
		GAUSS,
		CAUCHY,
		LAPLACE
	};

	class DistanceFilter {
	public:
		container::Grid<float> values_;
		int side_;
		float sum_;

		DistanceFilter(int side) {
			side_ = side;
			values_.initialize(2 * side + 1, 2 * side + 1);
		}

		int getSide() {
			return side_;
		}

		float* get(int x, int y) {
			return values_(x + side_, y + side_);
		}

		float* operator() (int x, int y) {
			return values_(x + side_, y + side_);
		}

		void initialize(FilterTypes type, float variable, float contrastStrength, bool normalized) {
			sum_ = 0.0;
			for(int y = -side_; y <= side_; ++y)
				for(int x = -side_; x <= side_; ++x) {
					float distance = (float)x * (float)x + (float)y * (float)y;
					auto currentValue = values_(x + side_, y + side_);
					if(type == GAUSS)
						*currentValue = exp(-distance / variable);
					else if(type == CAUCHY)
						*currentValue = 1.0f / (1.0f + sqrt(distance) * variable);
					else if(type == LAPLACE)
						*currentValue = exp(-sqrt(distance) / variable);

					*currentValue = pow(*currentValue, pow(contrastStrength, 4.0f - 8.0f * *currentValue));

					sum_ += *currentValue;
				}

			if(normalized == true) {
				//sum /= pow(2.0 * (double)side_ + 1.0, 2);
				for(int y = -side_; y <= side_; ++y)
					for(int x = -side_; x <= side_; ++x) {
						*values_(x + side_, y + side_) /= sum_;
					}
			}
		}

		void addLayer(FilterTypes type, float variable, float contrastStrength, bool normalized, float weight) {
			double sum = 0.0;
			for(int y = -side_; y <= side_; ++y)
				for(int x = -side_; x <= side_; ++x) {
					float distance = (float)x * (float)x + (float)y * (float)y;
					auto currentValue = values_(x + side_, y + side_);
					if(type == GAUSS)
						*currentValue += exp(-distance / variable) * weight;
					else if(type == CAUCHY)
						*currentValue += (1.0f / (1.0f + sqrt(distance) * variable)) * weight;
					else if(type == LAPLACE)
						*currentValue += exp(-sqrt(distance) / variable) * weight;

					//*currentValue = pow(*currentValue, pow(contrastStrength, 4.0f - 8.0f * *currentValue));

					if(normalized == true)
						sum += *currentValue;
				}

			if(normalized == true) {
				for(int y = -side_; y <= side_; ++y)
					for(int x = -side_; x <= side_; ++x) {
						*values_(x + side_, y + side_) /= (float)sum;
					}
			}
		}
	};

	float getRandom(float minimum, float maximum);

	int getRandom(int min, int max);

	void randomize(float* vector, float minimum, float maximum);
}
