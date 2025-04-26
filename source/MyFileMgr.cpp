#include "MyFileMgr.h"



char MyFileMgr::userPath[MAX_PATH] = {};

void MyFileMgr::Implement()
{

	char* p;
	GetModuleFileName(NULL, userPath, MAX_PATH);
	p = strrchr(userPath, '\\');
	strcpy(p + 1, "userfiles");
	patch::RedirectJump(0x744FB0, MyFileMgr::InitUserDir);
}

void MyFileMgr::MakeDir(const char* path)
{
	HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == (HANDLE)-1)
		CreateDirectoryA(path, NULL);
	else
		CloseHandle(h);
}

char* MyFileMgr::InitUserDir()
{
	char* tracksPath = (char*)0xC92168;
	char* galleryPath = (char*)0xC92268;

	MakeDir(userPath);

	HANDLE h = CreateFileA(userPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == (HANDLE)-1)
		CreateDirectoryA(userPath, NULL);
	else
		CloseHandle(h);

	strcpy(galleryPath, userPath);
	strcat(galleryPath, "\\Gallery");
	MakeDir(galleryPath);

	strcpy(tracksPath, userPath);
	strcat(tracksPath, "\\User Tracks");
	MakeDir(tracksPath);

	return userPath;
}
