#pragma once


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