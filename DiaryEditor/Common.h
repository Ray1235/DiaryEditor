#pragma once
#include "Version.h"
#ifdef _DEBUG
#define DE_STR "DiaryEditor-"DE_VERSION" (dev)"
#else
#define DE_STR "DiaryEditor-"DE_VERSION" (ship)"
#endif

#define MaxUserPropertiesForEntity 5

#define RoomWidth 15
#define RoomHeight 9

#define LayoutFile "layout.dpr"

#define DiaryWorkspaceFormat	".dws"
#define DiaryLevelFormat		".dlv"
#define DiaryFloorDataFormat	".dfd"

// Exported formats
#define DiaryLevelExportFormat	".csv"

void Print(const char* fmt, ...);