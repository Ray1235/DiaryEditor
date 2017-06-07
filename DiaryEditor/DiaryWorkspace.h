#pragma once
#include "DiaryEntityDefinition.h"
#include "DiaryFloor.h"

enum class SelectionType {
	None,
	Floor,
	Level,
	Entity,
	EntityDef
};

class DiaryWorkspace
{
public:
	DiaryWorkspace();
	~DiaryWorkspace();

	bool Load(const std::string &path);
	bool Save(const std::string &path);

	int GetFloorCount();
	const char * GetFloorName(int index);
	DiaryFloor * GetCurrentFloor();
	int GetLevelCount();
	const char * GetLevelName(int index);
	DiaryLevel * GetCurrentLevel();

	void AddFloor(const char * sname, const char * name);
	bool isValidFloor(int index);
	bool isValidLevel(int index);

	void RemoveFloorAtIndex(int index);
	void RemoveLevelAtIndex(int index);

	void AddLevel(const char * sname, const char * name);

	int currentFloor = 0;
	int currentLevel = 0;
	int currentEntity = 0;
	int currentEntityDef = 0;
	SelectionType currentSelection = SelectionType::None; // for property editor	

	std::vector<DiaryFloor *> floors;
	std::vector<DiaryEntityDefinition> entities;
};

