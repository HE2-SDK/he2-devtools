#include "InteractiveViewport.h"
#include <utilities/math/GLMUtils.h>
#define IMVIEWGUIZMO_IMPLEMENTATION
#include <ImViewGuizmo.h>

InteractiveViewport::InteractiveViewport(csl::fnd::IAllocator* allocator) : Viewport{ allocator } {}

bool InteractiveViewport::RotateControl(csl::math::Vector3& position, csl::math::Quaternion& rotation) const {
	glm::vec3 camPos = EigenToGLM(position);
	glm::quat camRot = EigenToGLM(rotation);

	const ImVec2 guizmoHRes = ImVec2{ 80, 80 } * ImViewGuizmo::GetStyle().scale;
	const ImVec2 guizmoRes = guizmoHRes * 2;

	ImVec2 guizmoPos = 
		ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos() + // base position
		ImVec2{ viewportData.viewportDimensions.resX - guizmoHRes.x, guizmoHRes.y }; // offset position

	ImGui::Dummy(guizmoRes);

	auto id = ImGui::GetID("hover");

	if (ImGui::ItemHoverable(ImRect(guizmoPos - guizmoHRes, guizmoPos + guizmoHRes), id, ImGuiItemFlags_None)) {
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(guizmoPos - guizmoHRes, guizmoPos + guizmoHRes))
			ImGui::SetActiveID(id, ImGui::GetCurrentWindow());
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(guizmoPos - guizmoHRes, guizmoPos + guizmoHRes))
			ImGui::ClearActiveID();
	}
	
	bool changed = false;

	if (changed |= ImViewGuizmo::Rotate(camPos, camRot, guizmoPos, GetZoomToAABB(), controlSpeed)) {
		position = GLMToEigen(camPos);
		rotation = GLMToEigen(camRot);
	}

	return changed;
}

void InteractiveViewport::Render() {
	Viewport::Render();

	auto& viewMatrix = viewportData.viewMatrix;

	csl::math::Vector3 camPos{ viewMatrix.translation() };
	csl::math::Quaternion camRot{ viewMatrix.rotation() };

	if (RotateControl(camPos, camRot))
		UpdateMatrix(camPos, camRot);
}

void InteractiveViewport::UpdateMatrix(const csl::math::Vector3& position, const csl::math::Quaternion& rotation) {
	auto& viewMatrix = viewportData.viewMatrix;
	viewMatrix.linear() = rotation.toRotationMatrix();
	viewMatrix.translation() = position;
	SetViewMatrix(viewMatrix);
	UpdateViewMatrix();
}
