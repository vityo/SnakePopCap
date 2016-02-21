#pragma once
#include <math.h> 
#include <random>

namespace utils {
	const float PI = 3.141592654f;

	float easeOutElastic(float t, float a, float p) {
		float c = 1.0f;

		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return c;

		float s;
		if (a < c) {
			a = c;
			s = p / 4.0f;
		}
		else {
			s = p / (2.0f * PI) * asin(c / a);
		}

		return (a * pow(2.0f, -10.0f * t) * sin((t - s) * (2.0f * PI) / p) + c);
	}

	int random(int min, int max) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distribution(min, max);

		return distribution(generator);
	}
}