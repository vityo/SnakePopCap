#pragma once

namespace utils {
	// вспомогательные функции
	class Math {
	public:
		// функция моргания. t = [0, 1], чем ближе к 1, тем чаще true меняется на false
		static bool ease(float t);

		// настоящий рандом [min, max]. В PopCap для int нет
		static int random(int min, int max);
	};
}