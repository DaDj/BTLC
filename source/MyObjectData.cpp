#include "MyObjectData.h"

MyObjectMassPoint MyObjectData::ObjectMassPoint[5000] = {};

void MyObjectData::Implement()
{
	patch::RedirectCall(0x59F8A1, MyObjectData::SetObjectData);
	patch::RedirectCall(0x5B925F, MyObjectData::Initialise);
	
}

void MyObjectData::ReadMassPointFile(const char* fileName)
{
	std::ifstream TheFile;
	std::string TheLine;
	char Line2[500] = "";
	int i = 0;
	char* FilePath = (char*)paths::GetGameDirPathA();
	strcat(FilePath, fileName);

	TheFile.open(FilePath);

	if (!TheFile.is_open())
		return;

	int ModelIndex = -1, Flag = -1;
	float x = 0, y = 0, z = 0;
	char modelName[32];
	CBaseModelInfo *MyObject;
	int ObjId = 0;

	while (std::getline(TheFile, TheLine))
	{
		strcpy(Line2, TheLine.c_str());
		if (Line2[0] != ';' && Line2[0] != ' ')
		{
			//We don't actually need the model id defined in the file anymore. 
			//it can also jsut be a dummy ID.. doesnt matter
			sscanf(Line2, "%d, %[^,], %d, %f, %f, %f",&ModelIndex, &modelName, &Flag, &x, &y, &z);

			ModelIndex = 0;
			MyObject = CModelInfo::GetModelInfo(modelName, &ModelIndex);

			if (MyObject)
			{
					ObjectMassPoint[ObjId].Id = ModelIndex;
					ObjectMassPoint[ObjId].Flag = Flag;
					ObjectMassPoint[ObjId].x = x;
					ObjectMassPoint[ObjId].y = y;
					ObjectMassPoint[ObjId].z = z;
					ObjId++;
			}
		}	
		i++;
	}

	TheFile.close();
	return;
}



int MyObjectData::Initialise(const char* fileName)
{
	int Output = ((int(__cdecl*)(const char*))0x5B5360)(fileName);
	ReadMassPointFile("data\\masspoints.dat");
	return Output;
}



void MyObjectData::SetObjectData(int dwModelIndex, CObject& obj)
{
	((void(__cdecl*)(int, CObject&))0x5A2D00)(dwModelIndex, obj);

	int MyIndex = -1;
	for (int i = 0; i < 5000; i++)
	{
		if (ObjectMassPoint[i].Id == dwModelIndex)
		{
			MyIndex = i;
			break;
		}
		if (ObjectMassPoint[i].Id == 0)
		{
			MyIndex = -1;
			break;
		}
	}

	if (MyIndex > 0)
	{
		if (ObjectMassPoint[MyIndex].Flag == 2)
		{
   			obj.m_vecCentreOfMass.x = ObjectMassPoint[MyIndex].x;
			obj.m_vecCentreOfMass.y = ObjectMassPoint[MyIndex].y;
			obj.m_vecCentreOfMass.z = ObjectMassPoint[MyIndex].z;
		}
		else if (ObjectMassPoint[MyIndex].Flag == 1)
		{
			//Get boundingbox coords
			float bound_zmax = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMax.z;
			float bound_zmin = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMin.z;
			float massz = bound_zmax + bound_zmin / 2; //middle

			float bound_xmax = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMax.x;
			float bound_xmin = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMin.x;
			float massx = bound_xmax + bound_xmin / 2; //middle

			float bound_ymax = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMax.y;
			float bound_ymin = CModelInfo::ms_modelInfoPtrs[dwModelIndex]->m_pColModel->m_boundBox.m_vecMin.y;
			float massy = bound_ymax + bound_ymin / 2; //middle
			obj.m_vecCentreOfMass.x = massx;
			obj.m_vecCentreOfMass.y = massy;
			obj.m_vecCentreOfMass.z = massz;
		}
	}


}
  