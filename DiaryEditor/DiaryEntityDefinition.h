#pragma once
#include "stdafx.h"
struct UserProperty {
	std::string name;
	std::string value;
};

struct DiaryEntityDefinition
{
	std::string name;
	std::string pathToReferenceImg;

	UserProperty userProperties[MaxUserPropertiesForEntity];

	sf::Image referenceImage;
};

