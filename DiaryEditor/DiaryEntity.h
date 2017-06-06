#pragma once
struct EntityProperty {
	std::string name;
	std::string value;
};

class DiaryEntity
{
public:
	DiaryEntity();
	~DiaryEntity();

	DiaryEntityDefinition * parentDefinition;
	std::string name;

	int entPropertyCount = 0;
	EntityProperty entProperties[MaxUserPropertiesForEntity];


	void ImportDataFromDefinition();
};

