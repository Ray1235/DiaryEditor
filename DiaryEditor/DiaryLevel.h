#pragma once
#include "stdafx.h"

class DiaryLevel
{
public:
	DiaryLevel();
	~DiaryLevel();

	bool Load(const std::string &path);
	bool Save(const std::string &path);
	bool Export(const std::string &path);
};

