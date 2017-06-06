#include "stdafx.h"
#include "DiaryEntity.h"



DiaryEntity::DiaryEntity()
{
}


DiaryEntity::~DiaryEntity()
{
}

void DiaryEntity::ImportDataFromDefinition()
{
	if (parentDefinition)
	{
		for (int i = 0; i < parentDefinition->userPropertyCount; i++)
		{
			entProperties[i].name = parentDefinition->userProperties[i].name;
			entProperties[i].value = parentDefinition->userProperties[i].value;
		}
	}
}
