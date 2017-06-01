#pragma once
#include "stdafx.h"
#include "DiaryLevel.h"
#include "DiaryEntityDefinition.h"

class DiaryWorkspace
{
public:
	DiaryWorkspace();
	~DiaryWorkspace();

	bool Load(const std::string &path);
	bool Save(const std::string &path);

	std::vector<DiaryLevel *> levels;
	std::vector<DiaryEntityDefinition> entities;
};

