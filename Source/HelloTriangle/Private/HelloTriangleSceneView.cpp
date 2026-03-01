#include "HelloTriangleSceneView.h"

#include "HelloTriangleShader.h"
#include "ScreenPass.h"
#include "PixelShaderUtils.h"
#include "PostProcess/PostProcessing.h"
#include "DynamicResolutionState.h"
#include "FXRenderingUtils.h"
#include "RenderGraphUtils.h"

#include "RenderGraphBuilder.h"
#include "SceneView.h"

DEFINE_GPU_STAT(TrianglePass)

HelloTriangleSceneView::HelloTriangleSceneView(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{

}

void HelloTriangleSceneView::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	RenderTriangle(GraphBuilder, View, Inputs);
}

void HelloTriangleSceneView::RenderTriangle(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	checkSlow(View.bIsViewInfo);
	const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;

	Inputs.Validate();

	const FSceneViewFamily& ViewFamily = *View.Family;

	float ScreenPercentage = ViewFamily.SecondaryViewFraction;

	if (ViewFamily.GetScreenPercentageInterface())
	{
		DynamicRenderScaling::TMap<float> UpperBounds = ViewFamily.GetScreenPercentageInterface()->GetResolutionFractionsUpperBound();
		ScreenPercentage *= UpperBounds[GDynamicPrimaryResolutionFraction];
	}

	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, Viewport);

	if (!SceneColor.IsValid())
	{
		return;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "TrianglePass");
	RDG_GPU_STAT_SCOPE(GraphBuilder, TrianglePass);

	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

	FTrianglePSParams* PixelShaderParams = GraphBuilder.AllocParameters<FTrianglePSParams>();
	PixelShaderParams->RenderTargets[0] = FRenderTargetBinding(SceneColor.Texture, ERenderTargetLoadAction::ELoad);

	TShaderMapRef<FTrianglePS> PixelShader(GlobalShaderMap);
	check(PixelShader.IsValid());
	TShaderMapRef<FTriangleVS> VertexShader(GlobalShaderMap);
	check(VertexShader.IsValid());

	ClearUnusedGraphResources(PixelShader, PixelShaderParams);

	GraphBuilder.AddPass(
		RDG_EVENT_NAME("TriangleShader"),
		PixelShaderParams,
		ERDGPassFlags::Raster,
		[PixelShaderParams, Viewport, VertexShader, PixelShader](FRHICommandList& RHICmdList)
		{
			FGraphicsPipelineStateInitializer GraphicsPSO;
			GraphicsPSO.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSO.RasterizerState = TStaticRasterizerState<>::GetRHI();
			GraphicsPSO.DepthStencilState = TStaticDepthStencilState<>::GetRHI();
			GraphicsPSO.PrimitiveType = EPrimitiveType::PT_TriangleList;
			GraphicsPSO.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSO.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSO.BoundShaderState.VertexDeclarationRHI = GTriangleVertexBufferElementDesc.VertexDeclerationRHI;

			RHICmdList.ApplyCachedRenderTargets(GraphicsPSO);

			// Set viewport/scissor rect
			RHICmdList.SetViewport(
				Viewport.Min.X, Viewport.Min.Y, 0.0f,
				Viewport.Max.X, Viewport.Max.Y, 1.0f);

			RHICmdList.SetScissorRect(true, Viewport.Min.X, Viewport.Min.Y, Viewport.Max.X, Viewport.Max.Y);

			// this creates and caches the PSO using the init desc
			SetGraphicsPipelineState(RHICmdList, GraphicsPSO, 0);

			// Bind shader params for each stage
			SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PixelShaderParams);

			// Bind VB/IB and draw
			RHICmdList.SetStreamSource(0, GTriangleVertexBuffer.VertexBufferRHI, 0);
			RHICmdList.DrawIndexedPrimitive(GTriangleIndexBuffer.IndexBufferRHI, 0, 0, 3, 0, 1, 1);
		}
	);
}

