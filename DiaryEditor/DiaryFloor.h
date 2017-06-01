#pragma once
#include "DiaryLevel.h"
#include "stdafx.h"

struct LevelEntry {
	DiaryLevel * level;
	sf::Vector2<int> position;
};

class DiaryFloor
{
public:
	DiaryFloor();
	~DiaryFloor();

	bool Load(const std::string &path);
	bool Save(const std::string &path);
	bool Export(const std::string &path);

	std::vector<LevelEntry> Levels;

	std::string scriptName;
	std::string name;
	int style;
};

