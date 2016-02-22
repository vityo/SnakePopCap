#pragma once
// вспомогательные ассерты
#if defined(_DEBUG)
	// обычный ассерт
	#define Assert(expr) if (!(expr)) { throw exception(); }
	// ассерт с описанием ошибки
	#define Assert2(expr, msg) if (!(expr)) { throw exception(msg); }
#else
	#define Assert(expr) ((void)0)
	#define Assert2(expr, msg) ((void)0)
#endif 