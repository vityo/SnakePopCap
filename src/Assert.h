#pragma once
// ��������������� �������
#if defined(_DEBUG)
	// ������� ������
	#define Assert(expr) if (!(expr)) { throw exception(); }
	// ������ � ��������� ������
	#define Assert2(expr, msg) if (!(expr)) { throw exception(msg); }
#else
	#define Assert(expr) ((void)0)
	#define Assert2(expr, msg) ((void)0)
#endif 