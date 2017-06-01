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

	std::vector<DiaryFloor *> floors;
	std::vector<DiaryEntityDefinition> entities;
};

