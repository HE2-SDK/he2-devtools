#include "Viewport.h"
#include <ui/common/Textures.h>

using namespace hh::gfx;
using namespace hh::needle;
using namespace hh::gfnd;

Viewport::~Viewport() {
	if (renderTexture) {
		auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
		auto* renderEngine = renderMgr->GetNeedleResourceDevice();

		renderEngine->DestroyRenderTextureHandle(renderTexture);
	}
}

Viewport::Viewport(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, models { GetAllocator() }, name{ GetAllocator() } {}

void Viewport::Render()
{
	if (auto* texture = GetTextureID()) {
		ImVec2 pos = ImGui::GetCursorScreenPos();

		auto& dimensions = viewportData.viewportDimensions;
		ImVec2 size{ static_cast<float>(dimensions.resX), static_cast<float>(dimensions.resY) };

		ImGui::GetWindowDrawList()->AddRectFilled(
			pos,
			ImVec2(pos.x + size.x, pos.y + size.y),
			IM_COL32(0, 0, 0, 255)
		);

		ImGui::Image(texture, size);
	}
}

void Viewport::Setup(const Description& desc) {
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
		.allocator = GetAllocator(),
		.name = name.c_str(),
		.sceneName = name.c_str(),
		.autoCreatePipeline = true
	};
	renderTexture = reinterpret_cast<RenderTextureHandle*>(renderEngine->CreateRenderTextureHandle(rtCreateInfo, pAllocator));

	auto* resMgr = hh::fnd::ResourceManager::GetInstance();
	auto* shLightField = resMgr->GetResource<ResSHLightField>("test_lf");
	SupportFX::LightFieldDescription supportFxLfDesc{};
	supportFxLfDesc.binaryData = shLightField->binaryData;
	supportFxLfDesc.textures = &shLightField->textures;
	supportFx->SetupLightField(supportFxLfDesc, name.c_str());

	auto* renderingEngine = renderMgr->implementation->renderingEngine;
	auto* iblComponent = (IBLComponent*)renderingEngine->GetComponentByHash(csl::ut::HashString("IBLComponent"));

	auto* iblTexture = resMgr->GetResource<ResTexture>("defaultibl_test");
	auto* texture = iblTexture->GetTexture();
	iblComponent->SetIBLTexture(&texture, name.c_str());

	viewportData.Reset();

	viewportData.viewMatrix = csl::math::Matrix34::Identity();
	viewportData.inverseViewMatrix = viewportData.viewMatrix.inverse();

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

void Viewport::UpdateViewportData() {
	renderTexture->SetCameraParameter(viewportData.viewMatrix, viewportData.projMatrix);
}

void Viewport::SetViewMatrix(const csl::math::Matrix34& mat) {
	viewportData.viewMatrix = mat;
	viewportData.inverseViewMatrix = viewportData.viewMatrix.inverse();
}

void Viewport::UpdateViewMatrix() {
	auto mat = viewportData.viewMatrix;
	csl::math::Vector3 camPos{ mat.translation() };
	SetCameraPositionAndTarget(camPos, viewportData.lookAtPos);
	UpdateViewportData();
}

float Viewport::GetZoomToAABB(const csl::geom::Aabb& aabb) const {
	csl::math::Vector3 bboxExtent{ aabb.Extent() };
	float maxDimension{ bboxExtent.norm() };

	return maxDimension * 2.5f;
}

float Viewport::GetZoomToAABB(hh::needle::PBRModelInstance* modelAabb) const {
	csl::geom::Aabb bbox;
	modelAabb->GetModelSpaceAabb(&bbox);
	return GetZoomToAABB(bbox);
}

float Viewport::GetZoomToAABB() const {
	if (models.empty()) return 2.5f;

	float lastDistance{ GetZoomToAABB(models[0]) };

	if (models.size()) return lastDistance;

	for (auto* model : models) {
		if (model == models[0]) continue;

		float distance{ GetZoomToAABB(model) };
		if (distance > lastDistance)
			lastDistance = distance;
	}

	return lastDistance;
}

void Viewport::SetCameraToAABB(const csl::geom::Aabb& aabb) {
	float distance{ GetZoomToAABB(aabb) };

	csl::math::Vector3 direction{ 1, 1, 1 };
	direction.normalize();

	csl::math::Vector3 bboxCenter{ aabb.Center() };
	csl::math::Vector3 camPos{ bboxCenter + direction * distance };
	csl::math::Vector3 camTargetPos{ bboxCenter };

	SetCameraPositionAndTarget(camPos, camTargetPos);
	UpdateViewportData();
}

void Viewport::SetCameraToAABB(hh::needle::PBRModelInstance* modelAabb) {
	csl::geom::Aabb bbox;
	modelAabb->GetModelSpaceAabb(&bbox);
	SetCameraToAABB(bbox);
}

void Viewport::SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target) {
	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void Viewport::SetCameraPosition(const csl::math::Vector3& position) {
	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), viewportData.lookAtPos));
}

void Viewport::SetCameraTarget(const csl::math::Vector3& target) {
	auto& mat{ viewportData.viewMatrix };
	csl::math::Vector3 camPos{ mat.translation() };
	SetViewMatrix(csl::math::Matrix34LookAt(camPos, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void Viewport::SetCameraFOV(const float fov) {
	auto& dimensions{ viewportData.viewportDimensions };

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

hh::needle::Texture* Viewport::GetTexture() const {
	if (!renderTexture) return nullptr;

	return renderTexture->GetTexture(0);
}

ImTextureID Viewport::GetTextureID() const {
	auto* texture{ GetTexture() };
	return texture == nullptr ? nullptr : GetTextureIDFromMIRAGETexture(texture);
}

void Viewport::AddModel(hh::needle::PBRModelInstance* modelInstance, bool setToAabb) {
	models.push_back(modelInstance);
	auto* world{ renderTexture->GetWorldByIdx(0) };
	auto* entryLink{ world->GetReferWorld() };
	world->AddModelInstanceNode(modelInstance, entryLink, true, -1, NEEDLE_RESOURCE_MODEL_INSTANCE);

	if (setToAabb)
		SetCameraToAABB(modelInstance);
}

void Viewport::AddModel(hh::gfx::ResModel* resModel, bool setToAabb) {
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	hh::gfx::RenderManager::SModelCreationInfo createInfo{ pAllocator };
	createInfo.meshResource = resModel->GetMeshResource();

	Model* mdl;
	renderMgr->CreateModelFromResource2(&mdl, createInfo);
	auto* modelInstance = static_cast<PBRModelInstance*>(renderMgr->CreateModelInstance(mdl, createInfo));

	AddModel(modelInstance, setToAabb);
}

void Viewport::RemoveModel(hh::needle::PBRModelInstance* modelInstance)
{
	size_t modelIdx = models.find(modelInstance);
	if (modelIdx == -1) return;

	models.remove(modelIdx);
	auto* world{ renderTexture->GetWorldByIdx(0) };
	world->Remove(modelInstance);
}

void Viewport::ClearModels() {
	auto* world{ renderTexture->GetWorldByIdx(0) };
	for (auto* model : models)
		world->Remove(model);

	models.clear();
}

