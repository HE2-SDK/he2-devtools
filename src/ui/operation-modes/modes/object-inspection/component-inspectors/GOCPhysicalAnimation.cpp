#include "GOCPhysicalAnimation.h"

//bool Editor(const char* label, hh::pba::GOCPhysicalAnimationBullet::SoftBodyInfo& info) {
//	Viewer("Maybe active", info.maybeActive);
//	Viewer("Group", info.group);
//	Viewer("Mask", info.mask);
//}

#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

void RenderPhysicalSkeletonBinding(hh::pba::PhysicalSkeletonBinding& binding) {
	if (auto& skeleton = binding.targetSkeletonRes)
		Viewer("Target skeleton resource", skeleton->GetName());

	if (auto& skeleton = binding.physicalSkeletonRes)
		Viewer("Physical skeleton resource", skeleton->GetName());

	Editor("Rigid body bone indices", binding.rigidBodyBoneIndices);
	Editor("Soft body bone indices", binding.softBodyBoneIndices);
}

void RenderComponentInspector(hh::pba::GOCPhysicalAnimationBullet& component) {
	Viewer("unk202", component.unk202);
	Editor("Frames per physics step", component.framesPerPhysicsStep);
	Editor("Frame time", component.frameTime);
	Viewer("Is ragdoll", component.isRagdoll);
	Viewer("Is enabled", component.enabled);
	Viewer("Is active", component.isActive);
	Editor("Speed", component.speed);
	Editor("Minimum bone weight", component.minimumBoneWeight);
	Editor("dword1C0", component.dword1C0);
	Editor("Position offset", component.positionOffset);
	Editor("Wind velocity", component.windVelocity);
	Editor("Air density", component.airDensity);
	Viewer("Has soft body", component.hasSoftBody);
	Viewer("Manifold count", component.numManifolds);

	Viewer("Transform", component.csl__math__transform160);

	if (auto& skeleton = component.physSkelRes)
		Viewer("Skeleton resource", skeleton->GetName());

	ImGui::SeparatorText("Physical skeleton binding");
	if (component.physSkelBind)
		RenderPhysicalSkeletonBinding(*component.physSkelBind);

	ImGui::SeparatorText("Infos");

	for (int i = 0; i < component.softbodyInfos.size(); i++) {
		csl::ut::MoveArray<csl::math::Vector3> softBodyPositions{ hh::fnd::MemoryRouter::GetTempAllocator() };
		component.GetSoftBodyNodePositions(i, softBodyPositions);
		Editor("SB node positions", softBodyPositions);
	}
}
