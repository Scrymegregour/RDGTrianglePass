#pragma once

#include "SceneViewExtension.h"
#include "RenderResource.h"

DECLARE_GPU_STAT_NAMED_EXTERN(TrianglePass, TEXT("TrianglePass"))

class HELLOTRIANGLE_API HelloTriangleSceneView : public FSceneViewExtensionBase
{
public:
	HelloTriangleSceneView(const FAutoRegister& AutoRegister);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {};
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs);
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) {};

protected:
	void RenderTriangle(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs);
};