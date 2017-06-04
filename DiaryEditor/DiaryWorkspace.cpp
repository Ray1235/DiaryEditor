#include "stdafx.h"
#include "DiaryWorkspace.h"



DiaryWorkspace::DiaryWorkspace()
{
}


DiaryWorkspace::~DiaryWorkspace()
{
}

bool DiaryWorkspace::Load(const std::string & path)
{
	return false;
}

bool DiaryWorkspace::Save(const std::string & path)
{
	return false;
}

int DiaryWorkspace::GetFloorCount()
{
	return floors.size();
}

const char * DiaryWorkspace::GetFloorName(int index)
{
	if(index >= floors.size() || index < 0)
		return 0;
	return floors[index]->name.c_str();
}

void DiaryWorkspace::AddFloor(const char * sname, const char * name)
{
	DiaryFloor * f = new DiaryFloor();
	f->name = name;
	f->scriptName = sname;
}

bool DiaryWorkspace::isValidFloor(int index)
{
	if(index >= floors.size() || index < 0)
		return false;
	if (floors.size() <= 0)
		return false;
	return true;
}
