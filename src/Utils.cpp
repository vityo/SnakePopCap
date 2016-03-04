#include "stdafx.h"
#include "Utils.h"
#include <math.h> 
#include <random>

using namespace std;
using namespace utils;

bool Math::ease(float t) {
	t += 0.5f;
	return cos(t * t * t * 30.0f) > 0.f;
}

int Math::random(int min, int max) {
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}