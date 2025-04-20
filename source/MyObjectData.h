#pragma once
#include <CObjectData.h>
#include "plugin.h"

using namespace plugin;


struct MyObjectMassPoint
{
	int Id; 
	int Flag;
	float x;
	float y;
	float z;
};

class MyObjectData :CObjectData
   
{
public:
	static MyObjectMassPoint ObjectMassPoint[5000];
	static void Implement();
	static void ReadMassPointFile(const char* fileName);
	static int Initialise(const char* fileName);  
	static void SetObjectData(int dwModelIndex, CObject& obj);
};

