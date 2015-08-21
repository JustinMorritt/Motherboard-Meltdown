//***************************************************************************************
// LightHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "LightHelper.h"
 

void Lights::ClearLights()
{
	if (DirLights.size() != 0)
	{
		for (int i = 0; i < DirLights.size(); i++)
		{
			delete DirLights[i];
			DirLights[i] = nullptr;
		}
		DirLights.clear();
	}

	if (PointLights.size() != 0)
	{
		for (int i = 0; i < PointLights.size(); i++)
		{
			delete PointLights[i];
			PointLights[i] = nullptr;
		}
		PointLights.clear();
	}

	if (SpotLights.size() != 0)
	{
		for (int i = 0; i < SpotLights.size(); i++)
		{
			delete SpotLights[i];
			SpotLights[i] = nullptr;
		}
		SpotLights.clear();
	}
}
