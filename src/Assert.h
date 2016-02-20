#pragma once
#define Assert(expr) if (!(expr)) { throw exception(); }
#define Assert2(expr, msg) if (!(expr)) { throw exception(msg); }