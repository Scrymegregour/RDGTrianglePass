#pragma once

#include "GlobalShader.h"
#include "Runtime/Renderer/Private/ScreenPass.h"

BEGIN_SHADER_PARAMETER_STRUCT(FTriangleVSParams, )
END_SHADER_PARAMETER_STRUCT()

class FTriangleVS : public FGlobalShader
{
public:
	using FParameters = FTriangleVSParams;
	DECLARE_GLOBAL_SHADER(FTriangleVS);
	SHADER_USE_PARAMETER_STRUCT(FTriangleVS, FGlobalShader);

	static inline bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return FShader::ShouldCompilePermutation(Parameters);
	}
};

BEGIN_SHADER_PARAMETER_STRUCT(FTrianglePSParams, )
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FTrianglePS : public FGlobalShader
{
public:
	using FParameters = FTrianglePSParams;
	DECLARE_GLOBAL_SHADER(FTrianglePS);
	SHADER_USE_PARAMETER_STRUCT(FTrianglePS, FGlobalShader);

	static inline bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return FShader::ShouldCompilePermutation(Parameters);
	}
};


//Resource Classes
struct FVertexData 
{
	FVector2f Position;
	FVector4f Color;
};

class FTriangleIndexBuffer : public FIndexBuffer
{
public:
	virtual void InitRHI() override;
};

class FTriangleVertexBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override;
};

class FTriangleVertexBufferElementDesc : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclerationRHI;
	virtual ~FTriangleVertexBufferElementDesc() override {};

	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;
};

BEGIN_SHADER_PARAMETER_STRUCT(FTintCSParams, )
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutputTexture)
	SHADER_PARAMETER(FVector4f, TintColor)
	SHADER_PARAMETER(float, Intensity)
END_SHADER_PARAMETER_STRUCT()

class FTintCS : public FGlobalShader
{
public:
	using FParameters = FTintCSParams;

	DECLARE_GLOBAL_SHADER(FTintCS);
	SHADER_USE_PARAMETER_STRUCT(FTintCS, FGlobalShader);

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_X"), 8);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Y"), 8);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE_Z"), 1);
	}
};

extern HELLOTRIANGLE_API TGlobalResource<FTriangleIndexBuffer> GTriangleIndexBuffer;
extern HELLOTRIANGLE_API TGlobalResource<FTriangleVertexBuffer> GTriangleVertexBuffer;
extern HELLOTRIANGLE_API TGlobalResource<FTriangleVertexBufferElementDesc> GTriangleVertexBufferElementDesc;