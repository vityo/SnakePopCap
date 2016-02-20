#pragma once
#define Assert(expr) if (!(expr)) { throw exception(); }
#define Assert(expr, msg) if (!(expr)) { throw exception(msg); }