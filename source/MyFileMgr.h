#pragma once
#include "plugin.h"
#include "CFileMgr.h"
using namespace plugin;

class MyFileMgr : CFileMgr
{
public:

	static char userPath[MAX_PATH];
	static void Implement();
	static void MakeDir(const char* path);
	static char* InitUserDir();

};