#include "MyDebug.h"

void MyDebug::Implement()
{
	plugin::Events::initGameEvent += []
		{
			MyDebug::GenerateFreeIDList();
		};
}

void MyDebug::GenerateFreeIDList()
{
	int FreeStart = 0;
	bool FreeFlag = 0;
	int count = 0;
	int maxId = 29999;
	std::ofstream myfile;
	myfile.open("FreeID.info");
	for (int i = 0; i < maxId; i++)
	{
		if (!CModelInfo::ms_modelInfoPtrs[i] && !FreeFlag)
		{
			FreeStart = i;
			FreeFlag = 1;

		}
		if (CModelInfo::ms_modelInfoPtrs[i] && FreeFlag)
		{
			count = (i)-FreeStart;
			myfile << FreeStart << ", " << (i - 1) << ",  " << count << std::endl;
			FreeFlag = 0;
		}
		if (i == (maxId - 1) && FreeFlag)
		{
			FreeFlag = 0;
			count = (i)-FreeStart;
			myfile << FreeStart << ", " << (i) << ",  " << count << std::endl;
		}

	}
	myfile.close();
}
