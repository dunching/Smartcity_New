#include "SceneElementTools.h"

#include "DatasmithAssetUserData.h"

bool USceneElementTools::CheckIsJiaCeng(
	UDatasmithAssetUserData* AUDPtr
	)
{
	if (!AUDPtr)
	{
		return false;
	}
	{
		{
			auto ID = AUDPtr->MetaData.Find(TEXT("Element*管线类型编号"));
			if (ID)
			{
				if (ID->Contains(TEXT("J")))
				{
					return true;
				}
			}
		}
		{
			auto ID = AUDPtr->MetaData.Find(TEXT("Element*照明回路编号"));
			if (ID)
			{
				if (ID->Contains(TEXT("J")))
				{
					return true;
				}
			}
		}
		{
			auto ID = AUDPtr->MetaData.Find(TEXT("Element*设备回路编号"));
			if (ID)
			{
				if (ID->Contains(TEXT("J")))
				{
					return true;
				}
			}
		}
		{
			auto ID = AUDPtr->MetaData.Find(TEXT("Element*空调和新风回路编号"));
			if (ID)
			{
				if (ID->Contains(TEXT("J")))
				{
					return true;
				}
			}
		}
	}
	return false;
}
