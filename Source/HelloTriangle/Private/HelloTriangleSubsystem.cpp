#include "HelloTriangleSubsystem.h"
#include "HelloTriangleSceneView.h"

void UHelloTriangleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TriangleViewExtension = FSceneViewExtensions::NewExtension<HelloTriangleSceneView>();
}
