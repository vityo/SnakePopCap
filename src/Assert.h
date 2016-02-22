#pragma once
// обычный ассерт
#define Assert(expr) if (!(expr)) { throw exception(); }
// ассерт с описанием ошибки
#define Assert2(expr, msg) if (!(expr)) { throw exception(msg); }