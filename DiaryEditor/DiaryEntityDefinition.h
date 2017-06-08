#pragma once
#include "Common.h"
#include <string>
#include "stdafx.h"

struct UserProperty {
	std::string name;
	std::string value;
};

struct DiaryEntityDefinition
{
	DiaryEntityDefinition(std::string NameV, std::string pathToImgRef, int propertyCount, ...);
	std::string name;
	std::string pathToReferenceImg;
	std::string scriptFunc = "null";

	int userPropertyCount = 0;
	UserProperty userProperties[MaxUserPropertiesForEntity];

	sf::Image referenceImage;
};

static DiaryEntityDefinition defaultEntities[]{
	DiaryEntityDefinition( "wall", "img/wall.png", 0 ),
	DiaryEntityDefinition( "door", "img/door.png", 1, "enabled", "true" )
};


