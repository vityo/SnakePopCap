#pragma once

namespace utils {
	// ��������������� �������
	class Math {
	public:
		// ������� ��������. t = [0, 1], ��� ����� � 1, ��� ���� true �������� �� false
		static bool ease(float t);

		// ��������� ������ [min, max]. � PopCap ��� int ���
		static int random(int min, int max);
	};
}