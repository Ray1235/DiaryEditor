// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define MaxUserPropertiesForEntity 5

#define DiaryWorkspaceFormat	".dws"
#define DiaryLevelFormat		".dlv"
#define DiaryFloorDataFormat	".dfd"

// Exported formats
#define DiaryLevelExportFormat	".csv"

// TODO: reference additional headers your program requires here

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "DiaryLevel.h"
#include "DiaryWorkspace.h"
