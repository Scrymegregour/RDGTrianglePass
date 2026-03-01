#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "HelloTriangleSubsystem.generated.h"

class HelloTriangleSceneView;

UCLASS()
class HELLOTRIANGLE_API UHelloTriangleSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override {};

protected:
	TSharedPtr<HelloTriangleSceneView, ESPMode::ThreadSafe> TriangleViewExtension;
};
