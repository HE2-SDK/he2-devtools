#include "ResModelEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/Textures.h>

using namespace hh::gfx;
using namespace hh::needle;

ResModelEditor::~ResModelEditor()
{
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	auto* renderEngine = renderMgr->GetNeedleResourceDevice();

	renderEngine->DestroyRenderTextureHandle(renderTexture);
}

void ResModelEditor::UpdateViewportData()
{
	renderTexture->SetCameraParameter(viewportData.viewMatrix, viewportData.projMatrix);
}

void ResModelEditor::SetViewMatrix(const csl::math::Matrix34& mat)
{
	viewportData.viewMatrix = mat;
	csl::math::Matrix34 invViewMat = viewportData.viewMatrix.inverse();
	viewportData.inverseViewMatrix.translate(invViewMat.translation());
}

void ResModelEditor::UpdateViewMatrix()
{
	auto* camera = renderTexture->camera;
	SetCameraPositionAndTarget(camera->position, viewportData.lookAtPos);
	UpdateViewportData();
}

void ResModelEditor::SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target)
{
	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void ResModelEditor::SetCameraPosition(const csl::math::Vector3& position)
{
	if (!renderTexture) return;

	SetViewMatrix(csl::math::Matrix34LookAt(position, csl::math::Vector3::UnitY(), viewportData.lookAtPos));
}

void ResModelEditor::SetCameraTarget(const csl::math::Vector3& target)
{
	if (!renderTexture) return;

	SetViewMatrix(csl::math::Matrix34LookAt(renderTexture->camera->position, csl::math::Vector3::UnitY(), target));
	viewportData.lookAtPos = target;
}

void ResModelEditor::SetCameraFOV(const float fov)
{
	viewportData.SetPerspectiveProjectionMatrix(
		fov,
		resolution[0] / resolution[1],
		.1f,
		10000
	);
}

ResModelEditor::ResModelEditor(csl::fnd::IAllocator* allocator, ResModel* resource) : StandaloneWindow{ allocator }, name{ allocator }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource);
	SetTitle(namebuf);

	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	auto* renderEngine = renderMgr->GetNeedleResourceDevice();
	auto* supportFx = renderEngine->GetSupportFX();

	hh::gfx::RenderManager::SModelCreationInfo createInfo{ allocator };
	createInfo.meshResource = resource->GetMeshResource();

	Model* mdl;
	renderMgr->CreateModelFromResource2(&mdl, createInfo);
	modelInstance = static_cast<PBRModelInstance*>(renderMgr->CreateModelInstance(mdl, createInfo));

	RenderTextureCreateArgs::TextureSettings textureSettings{
		.format = SurfaceFormat::R8G8B8A8,
		.mipLevels = 1
	};

	static const char* nameTemplate = "ModelPreview";
	static const size_t nameTemplateLen = strlen(nameTemplate);

	const char* resName = resource->name.c_str();
	const size_t resNameLen = strlen(resName);

	const size_t nameLen = resNameLen + nameTemplateLen + 1;
	char* nameRaw = static_cast<char*>(allocator->Alloc(nameLen, 4));
	snprintf(nameRaw, nameLen, "%s%s", resName, nameTemplate);
	name.Set(nameRaw);

	RenderTextureCreateArgs rtCreateInfo{
		.width = resolution[0],
		.height = resolution[1],
		.flags = RenderTextureCreateArgs::UNK2,
		.clearedTextureCount = 1,
		.textureSettings = &textureSettings,
		.allocator = allocator,
		.name = name.c_str(),
		.sceneName = name.c_str(),
		.autoCreatePipeline = true
	};
	renderTexture = reinterpret_cast<RenderTextureHandle*>(renderEngine->CreateRenderTextureHandle(rtCreateInfo, allocator));
	
	csl::geom::Aabb bbox;
	modelInstance->GetModelSpaceAabb(&bbox);

	csl::math::Vector3 bboxCenter = bbox.Center();
	csl::math::Vector3 bboxExtent = bbox.Extent();
	float maxDimension = bboxExtent.norm();

	float distance = maxDimension * 2.5f;

	csl::math::Vector3 direction = { 1, 1, 1 };
	direction.normalize();

	csl::math::Vector3 camPos = bboxCenter + direction * distance;
	csl::math::Vector3 camTargetPos = bboxCenter;

	viewportData.Reset();
	SetCameraPositionAndTarget(camPos, camTargetPos);
	viewportData.SetDimensions({
		.resX = static_cast<float>(resolution[0]),
		.resY = static_cast<float>(resolution[1]),
		.renderResX = static_cast<float>(resolution[0]),
		.renderResY = static_cast<float>(resolution[1])
		});
	SetCameraFOV(1.0476f);

	UpdateViewportData();

	auto* world = renderTexture->GetWorldByIdx(0);
	auto* unk0 = world->UnkFunc1();
	world->AddModelInstance(modelInstance, unk0, true, -1, NEEDLE_RESOURCE_MODEL_INSTANCE);
}

ResModelEditor* ResModelEditor::Create(csl::fnd::IAllocator* allocator, ResModel* resource) {
	return new (allocator) ResModelEditor(allocator, resource);
}

void ResModelEditor::RenderContents()
{
	if (renderTexture) {
		if (Editor("Camera Position", renderTexture->camera->position))
			UpdateViewMatrix();

		if (Editor("Camera Target", viewportData.lookAtPos))
			UpdateViewMatrix();

		if (auto* texture = renderTexture->GetTexture(0)) {
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = { static_cast<float>(resolution[0]), static_cast<float>(resolution[1]) };
			ImGui::GetWindowDrawList()->AddRectFilled(
				pos,
				ImVec2(pos.x + size.x, pos.y + size.y),
				IM_COL32(0, 0, 0, 255)
			);
			ImGui::Image(GetTextureIDFromMIRAGETexture(texture), size);
		}
	}
}
