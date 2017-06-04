#pragma once
#include "DiaryEntityDefinition.h"
#include "DiaryFloor.h"

class DiaryWorkspace
{
public:
	DiaryWorkspace();
	~DiaryWorkspace();

	bool Load(const std::string &path);
	bool Save(const std::string &path);

	int GetFloorCount();
	const char * GetFloorName(int index);

	void AddFloor(const char * sname, const char * name);
	bool isValidFloor(int index);

	int currentFloor = 0;


	std::vector<DiaryFloor *> floors;
	std::vector<DiaryEntityDefinition> entities;
};

