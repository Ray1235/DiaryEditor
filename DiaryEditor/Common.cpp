#include "stdafx.h"


void Print(const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	char out[1024];
	vsprintf_s(out, fmt, v);
	va_end(v);
	printf("%s\n", out);
	console.AddLog(out);
}