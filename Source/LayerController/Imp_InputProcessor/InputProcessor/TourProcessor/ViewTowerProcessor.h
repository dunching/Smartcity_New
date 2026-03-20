#pragma once

#include <functional>

#include "CoreMinimal.h"

#include "InputProcessor.h"
#include "GenerateTypes.h"
#include "TourProcessor.h"

class ATrackVehicleBase;
class ATourPawn;
class AHorseCharacter;
class AViewerPawnBase;
class ABuildingBase;

namespace TourProcessor
{
	/**
	 * 查看整楼
	 * 漫游的状态
	 */
	class LAYERCONTROLLER_API FViewTowerProcessor : public FTourProcessor
	{
	private:
		GENERATIONCLASSINFO(FViewTowerProcessor, FTourProcessor);

	public:
		using FOwnerPawnType = ATourPawn;

		virtual void EnterAction() override;

		virtual bool InputKey(
			const FInputKeyEventArgs& EventArgs
		) override;

		virtual bool InputAxis(
			const FInputKeyEventArgs& EventArgs 
			) override;

		void AdjustCameraSeat(const FRotator&CameraSeat);

		void AdjustCamera();
		
		TSoftObjectPtr<AViewerPawnBase> DefaultBuildingCameraSeat;

	private:

		bool bHasRoted = false;
		
		bool bStartRot = false;
		
		bool bHasMoved = false;
		
		bool bStartMove = false;
	};
}
