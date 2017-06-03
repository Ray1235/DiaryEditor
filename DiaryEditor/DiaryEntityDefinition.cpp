#include "stdafx.h"
#include "DiaryEntityDefinition.h"

DiaryEntityDefinition::DiaryEntityDefinition(std::string NameV, std::string pathToImgRef, int propertyCount, ...)
{
	_ASSERT(propertyCount < 5);
	this->name = NameV;
	this->pathToReferenceImg = pathToImgRef;
	this->userPropertyCount = propertyCount;
	va_list v;
	int varg_c = propertyCount * 2;
	va_start(v, propertyCount);
	for (int i = 0; i < propertyCount; i++)
	{
		userProperties[i].name = va_arg(v, const char*);
		userProperties[i].value = va_arg(v, const char*);
	}
	va_end(v);
}
