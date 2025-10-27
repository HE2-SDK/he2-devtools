#include "Needle.h"
#include <imgui_internal.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/Textures.h>

template<typename T>
bool BitFieldEditor(const char* label, ImGuiDataType dtype, T* field, T startBit, T size) {
	bool edited{};
	T minVal = 0;
	T maxVal = ((1 << size) - 1);
	T orig = (*field >> startBit) & maxVal;
	T v = orig;

	if (ImGui::DragScalar(label, dtype, &v, 1.0f, &minVal, &maxVal)) {
		*field = (*field & ~(maxVal << startBit)) | (v << startBit);
		edited = true;
	}

	return edited;
}

void Viewer(const char* label, hh::needle::Texture& texture)
{
	ImGui::Image(GetTextureIDFromMIRAGETexture(&texture), ImVec2(256, 256));
	ImGui::SameLine();
	ImGui::Text(label);
}

bool Editor(const char* label, hh::needle::intrusive_ptr<hh::needle::Texture>& texture)
{
	Viewer(label, *texture);

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<hh::fnd::ManagedResource**>(payload->Data);
			const hh::fnd::ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo()) {
				auto* resTexture = static_cast<hh::gfnd::ResTexture*>(res);
				texture = resTexture->GetTexture();
			}
		}
		ImGui::EndDragDropTarget();
	}

	return false;
}

bool Editor(const char* label, hh::needle::Texture*& texture)
{
	Viewer(label, *texture);

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<hh::fnd::ManagedResource**>(payload->Data);
			const hh::fnd::ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo()) {
				auto* resTexture = static_cast<hh::gfnd::ResTexture*>(res);
				texture = resTexture->GetTexture();
			}
		}
		ImGui::EndDragDropTarget();
	}

	return false;
}
#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::needle::RsFlagMask& mask)
{
	bool edited{};

	if (ImGui::TreeNode(label)) {
		ImGui::Text("Mask: %zx", mask.flags);
		edited |= BitFieldEditor("Blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 28ull, 4ull);
		edited |= BitFieldEditor("Blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 23ull, 4ull);
		edited |= BitFieldEditor("Blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 19ull, 3ull);
		edited |= BitFieldEditor("Alpha blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 14ull, 4ull);
		edited |= BitFieldEditor("Alpha blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 9ull, 4ull);
		edited |= BitFieldEditor("Alpha blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 5ull, 3ull);
		edited |= ImGui::CheckboxFlags("Enable blending", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 46ull);
		edited |= ImGui::CheckboxFlags("Cull front", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 41ull);
		edited |= ImGui::CheckboxFlags("Cull back", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 40ull);
		edited |= ImGui::CheckboxFlags("Cull unknown", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 39ull);
		ImGui::Text("Mask: %zx", mask.flags);
		ImGui::TreePop();
	}

	return edited;
}
#endif

void Viewer(const char* label, const hh::needle::FxCamera& camera) {
	if (ImGui::TreeNode(label)) {
		if (BeginVectorViewerTable("Eye and LookAt", false)) {
			Viewer("Eye position", camera.GetEyePos());
			Viewer("LookAt position", camera.GetLookAtPos());
			EndVectorViewerTable();
		}
		Viewer("View matrix", camera.GetViewMatrix());
		Viewer("Projection matrix", camera.GetProjectionMatrix());
		Viewer("Field of view", camera.GetFieldOfView());

		float nearClip{}, farClip{};
		camera.GetClipPlanes(&nearClip, &farClip);
		Viewer("Near clip plane", nearClip);
		Viewer("Far clip plane", farClip);
		ImGui::TreePop();
	}
}

void Viewer(const char* label, const hh::gfx::RenderManager::Impl::LocalFxCamera& camera)
{
	if (ImGui::TreeNode(label)) {
		Viewer("Viewport ID", camera.id);
		Viewer("FxCamera", static_cast<const hh::needle::FxCamera&>(camera));
		ImGui::TreePop();
	}
}

bool Editor(const char* label, hh::needle::SupportFX::FxViewport& viewport) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		if (viewport.camera)
			Viewer("Camera", *viewport.camera);
		edited |= Editor("X", viewport.x);
		edited |= Editor("Y", viewport.y);
		edited |= Editor("Resolution X", viewport.resX);
		edited |= Editor("Resolution Y", viewport.resY);
		edited |= Editor("Previous resolution X", viewport.prevRenderResX);
		edited |= Editor("Previous resolution Y", viewport.prevRenderResY);
		edited |= Editor("Render resolution X", viewport.renderResX);
		edited |= Editor("Render resolution Y", viewport.renderResY);
		edited |= Editor("unk7", viewport.unk7);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::SupportFX::FxRenderParam& renderParam) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("Viewports", renderParam.viewports);
		edited |= Editor("Viewport count", renderParam.numViewports);
		edited |= Editor("unk32", renderParam.unk32);
		edited |= Editor("unk33", renderParam.unk33);
		edited |= Editor("unk34", renderParam.unk34);
		edited |= Editor("unk35", renderParam.unk35);
		ImGui::TreePop();
	}
	return edited;
}

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::needle::SceneParamContainer& sceneParamContainer) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		for (auto it = sceneParamContainer.params.begin(); it != sceneParamContainer.params.end(); it++) {
			if (ImGui::TreeNode(it.key()->name)) {
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxBloomParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxBloomParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxDOFParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxDOFParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxColorContrastParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxColorContrastParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxToneMapParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxToneMapParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxCameraControlParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxCameraControlParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxShadowMapParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxShadowMapParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxShadowHeightMapParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxShadowHeightMapParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxVolumetricShadowParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxVolumetricShadowParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxScreenBlurParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxScreenBlurParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSSAOParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSSAOParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSHLightFieldParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSHLightFieldParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxLightScatteringParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxLightScatteringParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxRLRParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxRLRParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSSGIParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSSGIParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxPlanarReflectionParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxPlanarReflectionParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxOcclusionCapsuleParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxOcclusionCapsuleParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxGodrayParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxGodrayParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxScreenSpaceGodrayParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxScreenSpaceGodrayParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxHeatHazeParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxHeatHazeParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSceneEnvironmentParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSceneEnvironmentParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxRenderOption>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxRenderOption>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSGGIParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSGGIParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxTAAParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxTAAParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxEffectParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxEffectParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxAtmosphereParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxAtmosphereParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxDensityParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxDensityParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxWindComputeParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxWindComputeParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxGpuEnvironmentParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxGpuEnvironmentParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxInteractiveWaveParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxInteractiveWaveParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxChromaticAberrationParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxChromaticAberrationParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxVignetteParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxVignetteParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxTerrainMaterialBlendingParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxTerrainMaterialBlendingParameter>*>(*it)->data);
				//if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxWeatherParameter>::ID).uniqueObject) Editor(itkey()->name, *static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxWeatherParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxColorAccessibilityFilterParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxColorAccessibilityFilterParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxCyberNoiseEffectParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxCyberNoiseEffectParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxCyberSpaceStartNoiseParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxCyberSpaceStartNoiseParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxCyberNPCSSEffectRenderParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxCyberNPCSSEffectRenderParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::gfx::FxDentParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::gfx::FxDentParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxFieldScanEffectRenderParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxFieldScanEffectRenderParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSeparableSSSParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSeparableSSSParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxAntiAliasing>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxAntiAliasing>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxLODParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxLODParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxDetailParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxDetailParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxDynamicResolutionParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxDynamicResolutionParameter>*>(*it)->data);
				//if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxTerrainParameter>::ID).uniqueObject) Editor(itkey()->name, *static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxTerrainParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxModelParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxModelParameter>*>(*it)->data);
				//if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::SmoothnessParameter>::ID).uniqueObject) Editor(itkey()->name, *static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::SmoothnessParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxPlanarProjectionShadowParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxPlanarProjectionShadowParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxDirectionalRadialBlurParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxDirectionalRadialBlurParameter>*>(*it)->data);
				//if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::PickColorParameter>::ID).uniqueObject) Editor(itkey()->name, *static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::PickColorParameter>*>(*it)->data);
				//if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::DebugVisualizeParameter>::ID).uniqueObject) Editor(itkey()->name, *static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::DebugVisualizeParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxSMAAParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxSMAAParameter>*>(*it)->data);
				if (it.key() == RESOLVE_STATIC_VARIABLE(hh::needle::NeedleType<hh::needle::FxFXAAParameter>::ID).uniqueObject) Editor(it.key()->name, static_cast<hh::needle::SceneParamContainer::ParamHolder<hh::needle::FxFXAAParameter>*>(*it)->data);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::PipelineInfo& pipelineInfo) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("dword8", pipelineInfo.dword8);
		edited |= Editor("Render parameters", pipelineInfo.renderParam);
		edited |= Editor("Scene parameter container", *pipelineInfo.sceneParamContainer);
		if (pipelineInfo.renderUnitNameId)
			Viewer("renderUnitNameId", pipelineInfo.renderUnitNameId->name);
		if (pipelineInfo.sceneNameId)
			Viewer("sceneNameId", pipelineInfo.sceneNameId->name);
		edited |= Editor("Camera ID", pipelineInfo.cameraId);
		//edited |= Editor("sceneParamContainer", pipelineInfo.sceneParamContainer);
		edited |= Editor("Draw pass count", pipelineInfo.drawPassCount);
		edited |= Editor("Current draw pass", pipelineInfo.currentPass);
		edited |= Editor("qword180", pipelineInfo.qword180);
		edited |= Editor("dword188", pipelineInfo.dword188);
		edited |= Editor("qword198", pipelineInfo.qword198);
		ImGui::TreePop();
	}
	return edited;
}
#endif

template<typename T>
bool SingleParamEditor(const char* label, uint32_t count, hh::needle::CNameIDObject** names, T* paramDatas) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		for (unsigned int i = 0; i < count; i++) {
			char name[100];
			snprintf(name, sizeof(name), "%d - %s", i, names[i]->name);

			ImGui::PushID(names[i]);
			edited |= Editor(name, paramDatas[i]);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	return edited;
}

template<typename T>
bool ArrayParamEditor(const char* label, uint32_t count, hh::needle::CNameIDObject** names, hh::needle::InstanceParameterContainerData::ArrayParamData<T>* paramDatas) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		for (unsigned int i = 0; i < count; i++) {
			if (ImGui::TreeNode(names[i], "%d - %s", i, names[i]->name)) {
				for (size_t j = 0; j < paramDatas[i].count; j++) {
					char name[100];
					snprintf(name, sizeof(name), "%s[%zd]", names[i]->name, j);

					ImGui::PushID(static_cast<unsigned int>(j));

					edited |= Editor(name, paramDatas[i].data[j]);
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::InstanceParameterContainerData& ipcd) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		if (ipcd.counts->boolCounts > 0) edited |= SingleParamEditor("Booleans", ipcd.counts->boolCounts, ipcd.names[0], ipcd.data->boolData);
		if (ipcd.counts->uintCounts > 0) edited |= ArrayParamEditor("Unsigned integers", ipcd.counts->uintCounts, ipcd.names[1], ipcd.data->uintData);
		if (ipcd.counts->floatCounts > 0) edited |= ArrayParamEditor("Floats", ipcd.counts->floatCounts, ipcd.names[2], ipcd.data->floatData);
		if (ipcd.counts->textureCounts > 0) edited |= SingleParamEditor("Textures", ipcd.counts->textureCounts, ipcd.names[3], ipcd.data->textureData);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, hh::needle::ParameterValueObjectContainer& pvo) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("InstanceParameterContainerData", pvo.ipcd);
		ImGui::TreePop();
	}

	return edited;
}

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::needle::RenderJob& job)
{
	bool edited{};
	if (ImGui::TreeNode(&job, "%zx - %s", &job, job.GetName())) {
		if (job.GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440D1234)) {
			auto& defaultModelRenderJob = static_cast<hh::needle::DefaultModelRenderJob&>(job);

			edited |= Editor("Viewport ID", defaultModelRenderJob.viewportId);
		}
		if (job.GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440C9CB0)) {
			auto& sebastienSky = static_cast<hh::needle::SebastienSky&>(job);

			edited |= Editor("Viewport ID", sebastienSky.viewportId);
			edited |= Editor("parameterValueObject1", *sebastienSky.parameterValueObject1);
			edited |= Editor("daySkyParameters", *sebastienSky.daySkyParameters);
			edited |= Editor("nightSkyParameters", *sebastienSky.nightSkyParameters);
			edited |= Editor("heightFogParameters", *sebastienSky.heightFogParameters);
			edited |= Editor("parameterValueObject5", *sebastienSky.parameterValueObject5);
		}
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::RenderingPipeline& pipeline)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		Viewer("name", pipeline.name);
		Viewer("Initialized", pipeline.initialized);
		Viewer("Jobs running", pipeline.jobsAreRunning);

		for (auto* job : pipeline.renderJobs)
			Editor(job->GetName(), *job);

		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::WorldRenderingPipeline& pipeline)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("RenderingPipeline", static_cast<hh::needle::RenderingPipeline&>(pipeline));
		edited |= Editor("drawPassCount", pipeline.settings.drawPassCount);
		edited |= Editor("parameterCount", pipeline.settings.renderParameters.parameterCount);
		edited |= Editor("unk104", pipeline.settings.renderParameters.unk104);
		edited |= Editor("parameterSize", pipeline.settings.renderParameters.parameterSize);
		edited |= Editor("cullingGroupCount", pipeline.settings.renderParameters.cullingGroupCount);
		//edited |= Editor("occlusionCullingViewIndex", pipeline.settings.renderParameters.occlusionCullingIndices);
		//edited |= Editor("unk108a", pipeline.settings.renderParameters.);
		edited |= Editor("enableOcclusionCullingView", pipeline.settings.renderParameters.enableOcclusionCullingView);
		edited |= Editor("unk109", pipeline.settings.renderParameters.unk109);
		edited |= Editor("unk110", pipeline.settings.renderParameters.unk110);
		edited |= Editor("unk111", pipeline.settings.unk111);
		edited |= Editor("unk114", pipeline.unk114);
		edited |= Editor("unk115", pipeline.unk115);

		if (ImGui::TreeNode("gatherDrawPassInfoJobs")) {
			for (auto* job : pipeline.gatherDrawPassInfoJobs) {
				if (ImGui::TreeNode(job, "%s", job->renderJob->GetName())) {
					edited |= Editor("viewportId", job->viewportId);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	return edited;
}

#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::needle::RenderingPipelineMiller& pipeline)
#elif DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::needle::RenderingPipelineRangers& pipeline)
#endif
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("WorldRenderingPipeline", static_cast<hh::needle::WorldRenderingPipeline&>(pipeline));
		Viewer("name", pipeline.name);
		edited |= Editor("qword138", pipeline.qword138);
		edited |= Editor("qword150", pipeline.qword150);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::RenderUnit::Unk1& renderUnitUnk1) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("unk1", renderUnitUnk1.unk1);
	//edited |= Editor("unk2", renderUnitUnk1.unk2);
	Viewer("unk2", renderUnitUnk1.viewMatrices);
	Viewer("unk3", renderUnitUnk1.projectionMatrices);
	//edited |= Editor("unk4", renderUnitUnk1.unk4);
	//edited |= Editor("unk5", renderUnitUnk1.unk5);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::WorldRenderingPipelineExecContext& execContext) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("PipelineInfo", execContext.pipelineInfo);
		edited |= Editor("Global parameters", *execContext.globalParameters);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::RenderUnit& renderUnit) {
	bool edited{};
	ImGui::PushID(label);
	Viewer("Name", renderUnit.nameId->name);
	Viewer("Scene name", renderUnit.sceneNameId->name);
	edited |= Editor("Pipeline", *renderUnit.pipeline);
	edited |= Editor("PipelineInfo", *renderUnit.pipelineInfo);
	edited |= Editor("WorldRenderingPipelineExecContext", *renderUnit.renderingPipelineExecContext);
	edited |= Editor("unk1", renderUnit.unk1);
	edited |= Editor("unk2", renderUnit.unk2);
	edited |= Editor("Render unit viewport", renderUnit.renderUnitViewport);
	Viewer("Priority", renderUnit.priority);
	Viewer("Parameters are initialized", renderUnit.paramsInitialized);
	Viewer("Creates SceneParam listeners when initialized", renderUnit.createSceneParamListeners);
	edited |= Editor("unk14", renderUnit.unk14);
	edited |= Editor("PostFX Camera ID", renderUnit.cameraId);
	edited |= Editor("Renders", renderUnit.renders);
	edited |= Editor("unk18", renderUnit.unk18);
	Viewer("GatherRenderingPassContext memory size", renderUnit.flags);
	//pipelineInfo
	//renderingPipelineExecContext
	edited |= Editor("sceneParamContainer", *renderUnit.sceneParamContainer);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderTextureHandle& renderTexture)
{
	bool edited{};
	ImGui::PushID(label);
	Editor("RenderUnit", static_cast<hh::needle::RenderUnit&>(renderTexture));
	Viewer("Camera", *renderTexture.camera);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::MeshResource& meshResource)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		auto lodCount = meshResource.GetLODCount();

		for (unsigned int lodIdx = 0; lodIdx < lodCount; lodIdx++) {
			char lodNodeName[40];
			snprintf(lodNodeName, 40, "LOD %d", lodIdx);

			if (ImGui::TreeNode(lodNodeName)) {
				Viewer("ID", meshResource.GetLODID(lodIdx));

				auto scenePassIDStartIdx = meshResource.GetScenePassIDStartIndex(lodIdx);
				auto scenePassIDCount = meshResource.GetScenePassIDCount(lodIdx);

				for (unsigned int scenePassIDOffset = 0; scenePassIDOffset < scenePassIDCount; scenePassIDOffset++) {
					auto scenePassIDIdx = scenePassIDStartIdx + scenePassIDOffset;

					char scenePassIDNodeName[40];
					snprintf(scenePassIDNodeName, 40, "Scene Pass ID %d", lodIdx);

					if (ImGui::TreeNode(scenePassIDNodeName)) {
						Viewer("ID", meshResource.GetScenePassID(scenePassIDIdx));
						Viewer("Mesh count", meshResource.GetMeshCount(scenePassIDIdx));
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}

		auto nodeCount = meshResource.GetNodeCount();

		for (unsigned int nodeIdx = 0; nodeIdx < nodeCount; nodeIdx++) {
			char nodeNodeName[40];
			snprintf(nodeNodeName, 40, "Node %d", nodeIdx);

			if (ImGui::TreeNode(nodeNodeName)) {
				ImGui::Text("Not yet implemented");
				//auto* nodeMaterialResource = meshResource.GetNodeMaterialResource(nodeIdx);
				//if (nodeMaterialResource)
				//	edited |= Editor(nodeMaterialResource->GetMaterialInstanceParameter());
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	return edited;
}
#endif
