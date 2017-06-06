#include "stdafx.h"
#include "DiaryLevel.h"

#define INIT_STR(s,n) s.resize(n);s[0] = '\0';

DiaryLevel::DiaryLevel()
{
	INIT_STR(name, 128);
	INIT_STR(scriptName, 128);
}


DiaryLevel::~DiaryLevel()
{
}

bool DiaryLevel::Load(const std::string & path)
{
	return false;
}

bool DiaryLevel::Save(const std::string & path)
{
	return false;
}

bool DiaryLevel::Export(const std::string & path)
{
	return false;
}

char * DiaryLevel::GetUIFriendlyName()
{
	strcpy(uiFriendlyName, (name + " [" + scriptName + "] ").c_str());
	return uiFriendlyName;
}
