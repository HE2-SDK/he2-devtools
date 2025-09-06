#include "Previewer.h"
#include <ui/common/Textures.h>

using namespace hh::gfx;
using namespace hh::needle;

Previewer::~Previewer()
{
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	auto* renderEngine = renderMgr->GetNeedleResourceDevice();

	renderEngine->DestroyRenderTextureHandle(renderTexture);
}

Previewer::Previewer(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, models { allocator }, name{ allocator }
{
}

void Previewer::Setup(const Description& desc)
{
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	auto* renderEngine = renderMgr->GetNeedleResourceDevice();
	auto* supportFx = renderEngine->GetSupportFX();

	RenderTextureCreateArgs::TextureSettings textureSettings{
		.format = SurfaceFormat::R8G8B8A8,
		.mipLevels = 1
	};

	name.Set(desc.name);

	RenderTextureCreateArgs rtCreateInfo{
		.width = desc.resolution[0],
		.height = desc.resolution[1],
		.flags = RenderTextureCreateArgs::UNK2,
		.clearedTextureCount = 1,
		.textureSettings = &textureSettings,
		.allocator = pAllocator,
		.name = name.c_str(),
		.sceneName = name.c_str(),
		.autoCreatePipeline = true
	};
	renderTexture = reinterpret_cast<RenderTextureHandle*>(renderEngine->CreateRenderTextureHandle(rtCreateInfo, pAllocator));

	viewportData.Reset();
	SetCameraPositionAndTarget({}, {});

	float resF[2] = {desc.resolution[0], desc.resolution[1]};
	viewportData.SetDimensions({
		.resX = resF[0],
		.resY = resF[1],
		.renderResX = resF[0],
		.renderResY = resF[1]
	});
	SetCameraFOV(desc.fov);

	UpdateViewportData();

	if (desc.modelInstance)
		AddModel(desc.modelInstance, desc.setToAabb);
}

void Previewer::UpdateViewportData()
{
	renderTexture->SetCameraParameter(viewportData.viewMatrix, viewportData.projMatrix);
}

void Previewer::SetViewMatrix(const csl::math::Matrix34& mat)
{
	viewportData.viewMatrix = mat;
	csl::math::Matrix34 invViewMat = viewportData.viewMatrix.inverse();
	viewportData.inverseViewMatrix.translate(invViewMat.translation());
}

void Previewer::UpdateViewMatrix()
{
	auto mat = viewportData.viewMatrix.matrix();
	csl::math::Vector3 camPos = { mat(0, 3), mat(1, 3), mat(2, 3) };
	SetCameraPositionAndTarget(camPos, viewportData.lookAtPos);
	UpdateViewportData();
}

void Previewer::SetCameraToAABB(const csl::geom::Aabb& aabb)
{
	csl::math::Vector3 bboxCenter = aabb.Center();
	csl::math::Vector3 bboxExtent = aabb.Extent();
	float maxDimension = bboxExtent.norm();

	float distance = maxDimension * 2.5f;

	csl::math::Vector3 direction = { 1, 1, 1 };
	direction.normalize();

	csl::math::Vector3 camPos = bboxCenter + direction * distance;
	csl::math::Vector3 camTargetPos = bboxCenter;

	SetCameraPositionAndTarget(camPos, camTargetPos);
	UpdateViewportData();
}

void Previewer::SetCameraToAABB(hh::needle::PBRModelInstance* modelAabb)
{
	csl::geom::Aabb bbox;
	modelAabb->GetModelSpaceAabb(&bbox);
	SetCameraToAABB(bbox);
}

void Previewer::SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target)
{
	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void Previewer::SetCameraPosition(const csl::math::Vector3& position)
{
	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), viewportData.lookAtPos));
}

void Previewer::SetCameraTarget(const csl::math::Vector3& target)
{
	auto mat = viewportData.viewMatrix.matrix();
	csl::math::Vector3 camPos = { mat(0, 3), mat(1, 3), mat(2, 3) };
	SetViewMatrix(csl::math::Matrix34LookAt(camPos, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void Previewer::SetCameraFOV(const float fov)
{
	auto& dimensions = viewportData.viewportDimensions;

	float nearZ{ .1f };
	float farZ{ 10000 };
	viewportData.GetClipPlanes(&nearZ, &farZ);

	viewportData.SetPerspectiveProjectionMatrix(
		fov,
		dimensions.resX / dimensions.resY,
		nearZ,
		farZ
	);
}

hh::needle::Texture* Previewer::GetTexture() const
{
	if (!renderTexture) return nullptr;

	return renderTexture->GetTexture(0);
}

ImTextureID Previewer::GetTextureID() const
{
	auto* texture = GetTexture();
	return texture == nullptr ? nullptr : GetTextureIDFromMIRAGETexture(texture);
}

void Previewer::AddModel(hh::needle::PBRModelInstance* modelInstance, bool setToAabb)
{
	models.push_back(modelInstance);
	auto* world = renderTexture->GetWorldByIdx(0);
	auto* unk0 = world->UnkFunc1();
	world->AddModelInstance(modelInstance, unk0, true, -1, NEEDLE_RESOURCE_MODEL_INSTANCE);

	if (setToAabb)
		SetCameraToAABB(modelInstance);
}

void Previewer::AddModel(hh::gfx::ResModel* resModel, bool setToAabb)
{
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	hh::gfx::RenderManager::SModelCreationInfo createInfo{ pAllocator };
	createInfo.meshResource = resModel->GetMeshResource();

	Model* mdl;
	renderMgr->CreateModelFromResource2(&mdl, createInfo);
	auto* modelInstance = static_cast<PBRModelInstance*>(renderMgr->CreateModelInstance(mdl, createInfo));

	AddModel(modelInstance, setToAabb);
}
