#include "HelloTriangleShader.h"

TGlobalResource<FTriangleIndexBuffer> GTriangleIndexBuffer;
TGlobalResource<FTriangleVertexBuffer> GTriangleVertexBuffer;
TGlobalResource<FTriangleVertexBufferElementDesc> GTriangleVertexBufferElementDesc;

void FTriangleIndexBuffer::InitRHI()
{
	const uint16 Indicies[] = { 0, 1, 2};
	TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> IndexBuffer;
	uint32 NumIndices = UE_ARRAY_COUNT(Indicies);
	IndexBuffer.AddUninitialized(NumIndices);
	FMemory::Memcpy(IndexBuffer.GetData(), Indicies, NumIndices * sizeof(uint16));
	FRHIResourceCreateInfo CreateInfo(TEXT("FTriangleIndexBuffer"), &IndexBuffer);
	IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), IndexBuffer.GetResourceDataSize(), BUF_Static, CreateInfo);
}

void FTriangleVertexBuffer::InitRHI()
{
	TResourceArray<FVertexData, VERTEXBUFFER_ALIGNMENT> VertexBuffer;
	VertexBuffer.SetNumUninitialized(3);

	VertexBuffer[0].Position = FVector2f(-0.1f, 0.9f);
	VertexBuffer[0].Color = FVector4f(1, 0, 0, 1);
	VertexBuffer[1].Position = FVector2f(-0.1f, 0.0f);
	VertexBuffer[1].Color = FVector4f(0, 1, 0, 1);
	VertexBuffer[2].Position = FVector2f(-0.9f, 0.0f);
	VertexBuffer[2].Color = FVector4f(0, 0, 1, 1);

	FRHIResourceCreateInfo CreateInfo(TEXT("FTriangleVertexBuffer"), &VertexBuffer);
	VertexBufferRHI = RHICreateVertexBuffer(VertexBuffer.GetResourceDataSize(), BUF_Static, CreateInfo);
}

void FTriangleVertexBufferElementDesc::InitRHI()
{
	FVertexDeclarationElementList Elements;
	uint16 stride = sizeof(FVertexData);
	Elements.Add(FVertexElement(0, STRUCT_OFFSET(FVertexData, Position), VET_Float2, 0, stride));
	Elements.Add(FVertexElement(0, STRUCT_OFFSET(FVertexData, Color), VET_Float4, 1, stride));

	VertexDeclerationRHI = PipelineStateCache::GetOrCreateVertexDeclaration(Elements);
}

void FTriangleVertexBufferElementDesc::ReleaseRHI()
{
	VertexDeclerationRHI.SafeRelease();
}

IMPLEMENT_SHADER_TYPE(, FTriangleVS, TEXT("/CustomShaders/Private/TrianglePixelShader.usf"), TEXT("TriangleVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FTrianglePS, TEXT("/CustomShaders/Private/TrianglePixelShader.usf"), TEXT("TrianglePS"), SF_Pixel);