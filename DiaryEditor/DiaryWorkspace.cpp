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
	return floors[index]->GetUIFriendlyName();
}

DiaryFloor * DiaryWorkspace::GetCurrentFloor()
{
	if (isValidFloor(currentFloor)) return floors[currentFloor];
	return NULL;
}

int DiaryWorkspace::GetLevelCount()
{
	if (!isValidFloor(currentFloor)) return 0;
	return floors[currentFloor]->Levels.size();
}

const char * DiaryWorkspace::GetLevelName(int index)
{
	if (isValidFloor(currentFloor))
	{
		if (isValidLevel(index))
		{
			return floors[currentFloor]->Levels[index]->GetUIFriendlyName();
		}
	}
	else {
		return 0;
	}
	return 0;
}

DiaryLevel * DiaryWorkspace::GetCurrentLevel()
{
	if (isValidFloor(currentFloor) && isValidLevel(currentLevel)) return floors[currentFloor]->Levels[currentLevel];
	return NULL;
}

void DiaryWorkspace::AddFloor(const char * sname, const char * name)
{
	DiaryFloor * f = new DiaryFloor();
	f->name = name;
	f->scriptName = sname;
	floors.push_back(f);
}

bool DiaryWorkspace::isValidFloor(int index)
{
	if(index >= floors.size() || index < 0)
		return false;
	if (floors.size() <= 0)
		return false;
	return true;
}

bool DiaryWorkspace::isValidLevel(int index)
{
	if (!isValidFloor(currentFloor)) return false;
	if (index >= floors[currentFloor]->Levels.size() || index < 0)
		return false;
	if (floors[currentFloor]->Levels.size() <= 0)
		return false;
	return true;
}

void DiaryWorkspace::RemoveFloorAtIndex(int index)
{
	if (!isValidFloor(index)) return;
	DiaryFloor *floor = floors[index];
	
	free(floor);
	floors.erase(floors.begin() + index);
}

void DiaryWorkspace::RemoveLevelAtIndex(int index)
{
	if (!isValidFloor(currentFloor) || !isValidLevel(index)) return;
	DiaryLevel *level = floors[currentFloor]->Levels[index];

	free(level);
	floors[currentFloor]->Levels.erase(floors[currentLevel]->Levels.begin() + index);
}

void DiaryWorkspace::AddLevel(const char * sname, const char * name)
{
	DiaryLevel *level = new DiaryLevel();
	level->scriptName = sname;
	level->name = name;
	floors[currentFloor]->Levels.push_back(level);
}
