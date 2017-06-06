#pragma once
#include "stdafx.h"

#define NORTH_DOOR 0
#define EAST_DOOR 1
#define SOUTH_DOOR 2
#define WEST_DOOR 3

class DiaryLevel
{
public:
	DiaryLevel();
	~DiaryLevel();

	bool Load(const std::string &path);
	bool Save(const std::string &path);
	bool Export(const std::string &path);

	std::string name;
	std::string scriptName;

	char uiFriendlyName[128];

	bool doorEnabled[4];
	std::string doorTarget[4];

	bool overrrideFloorStyle = false;
	int style = 0;

	char * GetUIFriendlyName();
};

