#pragma once
#include <math.h> 
#include <random>

namespace utils {
	// вспомогательные функции

	// функция моргания. t = [0, 1], чем ближе к 1, тем чаще true меняется на false
	bool ease(float t) {
		t += 0.5f;
		return cos(t * t * t * 30.0f) > 0.f;
	}

	// настоящий рандом [min, max]. В PopCap для int нет
	int random(int min, int max) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distribution(min, max);

		return distribution(generator);
	}
}