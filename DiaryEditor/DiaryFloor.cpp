#include "stdafx.h"
#include "DiaryFloor.h"

#define INIT_STR(s,n) s.resize(n);s[0] = '\0';

DiaryFloor::DiaryFloor()
{
	INIT_STR(name, 128);
	INIT_STR(scriptName, 128);
	style = 0;
}


DiaryFloor::~DiaryFloor()
{
}

bool DiaryFloor::Load(const std::string & path)
{
	return false;
}

bool DiaryFloor::Save(const std::string & path)
{
	return false;
}

bool DiaryFloor::Export(const std::string & path)
{
	return false;
}

char * DiaryFloor::GetUIFriendlyName()
{
	strcpy(uiFriendlyName, (name + " [" + scriptName + "] ").c_str());
	return uiFriendlyName;
}

void DiaryFloor::AddLevel(const char * sname, const char * name)
{
}
