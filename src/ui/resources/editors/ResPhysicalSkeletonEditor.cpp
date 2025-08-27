#include "ResPhysicalSkeletonEditor.h"
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/pba/v1.h>
#include <rip/binary/containers/binary-file/v2.h>

bool ResPhysicalSkeletonEditor::Editor(Node& value, unsigned int idx, SoftBody& parent)
{
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("%s###", value.boneName);

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "Node%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (changed |= ImGui::Selectable("Remove Node")) {
			resources::ManagedCArray<Node, unsigned int> nodes{ resource, parent.nodes, parent.nodeCount };
			nodes.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= ::Editor("Mass", value.mass);
		changed |= ::Editor("Rigid body index", value.rigidBodyIndex);
		changed |= ::Editor("Is Pinned", value.isPinned);
		changed |= ::Editor("+X sibling index", value.positiveXSiblingIndex);
		changed |= ::Editor("-X sibling index", value.negativeXSiblingIndex);
		changed |= ::Editor("+Y sibling index", value.positiveYSiblingIndex);
		changed |= ::Editor("-Y sibling index", value.negativeYSiblingIndex);
		changed |= ::Editor("+Z sibling index", value.positiveZSiblingIndex);
		changed |= ::Editor("-Z sibling index", value.negativeZSiblingIndex);
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResPhysicalSkeletonEditor::Editor(Link& value, unsigned int idx, SoftBody& parent)
{
	bool changed = false;

	ImGui::PushID(&value);

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d###", idx);

	bool isOpen = ImGui::TreeNode(buffer);

	snprintf(buffer, sizeof(buffer), "Link%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (changed |= ImGui::Selectable("Remove Link")) {
			resources::ManagedCArray<Link, unsigned int> links{ resource, parent.links, parent.linkCount };
			links.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= ::Editor("Vertices", value.verts);
		changed |= ::Editor("Rest Length", value.restLength);
		changed |= ::Editor("Stiffness", value.stiffness);
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResPhysicalSkeletonEditor::Editor(Limit& value)
{
	bool changed = false;

	static const char* limitModeNames[] = {
		"None",
		"Enabled",
		"Disabled"
	};

	changed |= ComboEnum("Limit Mode", value.mode, limitModeNames);
	changed |= ::Editor("Low Limit", value.lowLimit);
	changed |= ::Editor("High Limit", value.highLimit);
	if (ImGui::TreeNode("Spring Settings")) {
		changed |= ::Editor("Spring Enabled", value.springEnabled);
		changed |= ::Editor("Spring Stiffness", value.springStiffness);
		changed |= ::Editor("Spring Damping", value.springDamping);
		ImGui::TreePop();
	}

	return changed;
}

bool ResPhysicalSkeletonEditor::Editor(SoftBody& value, unsigned int idx, PbaData& parent)
{
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("%s###", value.name);

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "SoftBody%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (changed |= ImGui::Selectable("Remove SoftBody")) {
			resources::ManagedCArray<SoftBody, unsigned int> softbodies{ resource, parent.softBodies, parent.softBodyCount };
			softbodies.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= InputText("Name", value.name, resource);
		changed |= ::Editor("Margin", value.margin);
		if (ImGui::TreeNode("Physics Settings")) {
			changed |= ::Editor("Damping Coefficient", value.dampingCoeff);
			changed |= ::Editor("Drag Coefficient", value.dragCoeff);
			changed |= ::Editor("Lift Coefficient", value.liftCoeff);
			changed |= ::Editor("Dynamic Friction Coefficient", value.dynamicFrictionCoeff);
			changed |= ::Editor("Pose Matching Coefficient", value.poseMatchingCoeff);
			changed |= ::Editor("Rigid Contacts Hardness", value.rigidContactsHardness);
			changed |= ::Editor("Kinetic Contacts Hardness", value.kineticContactsHardness);
			changed |= ::Editor("Soft Contacts Hardness", value.softContactsHardness);
			changed |= ::Editor("Anchors Hardness", value.anchorsHardness);
			changed |= ::Editor("Position Solver Iteration Count", value.positionSolverIterationCount);
			ImGui::TreePop();
		}
		changed |= ::Editor("Unk0", value.unk0);
		changed |= ::Editor("Group", value.group);

		bool nodesOpen = ImGui::TreeNode("Nodes");

		if (ImGui::BeginPopupContextItem("NodesOptions")) {
			if (changed |= ImGui::Selectable("Add Node")) {
				resources::ManagedCArray<Node, unsigned int> nodes{ resource, value.nodes, value.nodeCount };

				auto& node = nodes.emplace_back();
				node = {};
				node.boneName = "bone-name";
			}
			ImGui::EndPopup();
		}

		if (nodesOpen) {
			for (auto x = 0; x < value.nodeCount; x++) changed |= Editor(value.nodes[x], x, value);
			ImGui::TreePop();
		}

		bool linksOpen = ImGui::TreeNode("Links");

		if (ImGui::BeginPopupContextItem("LinksOptions")) {
			if (changed |= ImGui::Selectable("Add Link")) {
				resources::ManagedCArray<Link, unsigned int> links{ resource, value.links, value.linkCount };

				auto& link = links.emplace_back();
				link = {};
			}
			ImGui::EndPopup();
		}

		if (linksOpen) {
			for (auto x = 0; x < value.linkCount; x++) changed |= Editor(value.links[x], x, value);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResPhysicalSkeletonEditor::Editor(Constraint& value, unsigned int idx, PbaData& parent)
{
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("%s###", value.boneName);

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "Constraint%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (changed |= ImGui::Selectable("Remove Constraint")) {
			resources::ManagedCArray<Constraint, unsigned int> constraints{ resource, parent.constraints, parent.constraintCount };
			constraints.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= InputText("Bone Name", value.boneName, resource);
		changed |= ::Editor("Disable Collisions Between Linked Bodies", value.disableCollisionsBetweenLinkedBodies);
		changed |= ::Editor("Unk1", value.unk1);
		changed |= ::Editor("Override Solver Iteration Count", value.overrideSolverIterationCount);
		changed |= ::Editor("Parent Rigid Body Index", value.parentRigidBodyIndex);
		changed |= ::Editor("Child Rigid Body Index", value.childRigidBodyIndex);
		changed |= ::Editor("Skeleton Parent Bone Index", value.skeletonParentBoneIndex);
		if (ImGui::TreeNode("Linear Limits")) {
			for (auto x = 0; x < 3; x++) {
				changed |= Editor(value.linearLimits[x]);
				ImGui::Separator();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Angular Limits")) {
			for (auto x = 0; x < 3; x++) {
				changed |= Editor(value.angularLimits[x]);
				ImGui::Separator();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point A")) {
			changed |= ::Editor("Offset Position", value.offsetPositionA);
			changed |= ::Editor("Offset Rotation", value.offsetRotationA);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point B")) {
			changed |= ::Editor("Offset Position", value.offsetPositionB);
			changed |= ::Editor("Offset Rotation", value.offsetRotationB);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

static const char* typeNames[]{ "Dynamic", "Static" };
static const char* shapeNames[]{ "Sphere", "Box" };

bool ResPhysicalSkeletonEditor::Editor(RigidBody& value, unsigned int idx, PbaData& parent)
{
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("%s###", value.boneName);

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "RigidBody%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (changed |= ImGui::Selectable("Remove RigidBody")) {
			resources::ManagedCArray<RigidBody, unsigned int> rigidBodies{ resource, parent.rigidBodies, parent.rigidBodyCount };
			rigidBodies.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= InputText("Bone Name", value.boneName, resource);
		if (ImGui::TreeNode("Shape Settings")) {
			changed |= ::ComboEnum("Shape", value.shape, shapeNames);
			changed |= ::Editor("Radius", value.shapeRadius);
			changed |= ::Editor("Height", value.shapeHeight);
			changed |= ::Editor("Depth", value.shapeDepth);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Physics Settings")) {
			changed |= ::ComboEnum("Type", value.type, typeNames);
			changed |= ::Editor("Mass", value.mass);
			changed |= ::Editor("Friction", value.friction);
			changed |= ::Editor("Restitution", value.restitution);
			changed |= ::Editor("Linear Damping", value.linearDamping);
			changed |= ::Editor("Angular Damping", value.angularDamping);
			ImGui::TreePop();
		}
		changed |= ::Editor("Offset Position", value.offsetPosition);
		changed |= ::Editor("Offset Rotation", value.offsetRotation);
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResPhysicalSkeletonEditor::Editor(PbaData& value)
{
	bool changed = false;

	changed |= InputText("Skeleton Name", value.skeletonName, resource);

	bool rigidBodiesOpen = ImGui::TreeNode("Rigid Bodies");

	if (ImGui::BeginPopupContextItem("RigidBodiesOptions")) {
		if (changed |= ImGui::Selectable("Add RigidBody")) {
			resources::ManagedCArray<RigidBody, unsigned int> rigidBodies{ resource, value.rigidBodies, value.rigidBodyCount };
			auto& rigidBody = rigidBodies.emplace_back();
			rigidBody = {};
			rigidBody.boneName = "bone-name";
		}
		ImGui::EndPopup();
	}

	if (rigidBodiesOpen) {
		for (auto i = 0; i < value.rigidBodyCount; i++) changed |= Editor(value.rigidBodies[i], i, value);
		ImGui::TreePop();
	}

	bool constraintsOpen = ImGui::TreeNode("Constraints");

	if (ImGui::BeginPopupContextItem("ConstraintsOptions")) {
		if (changed |= ImGui::Selectable("Add Constraint")) {
			resources::ManagedCArray<Constraint, unsigned int> constraints{ resource, value.constraints, value.constraintCount };
			auto& constraint = constraints.emplace_back();
			constraint = {};
			constraint.boneName = "bone-name";
		}
		ImGui::EndPopup();
	}

	if (constraintsOpen) {
		for (auto i = 0; i < value.constraintCount; i++) changed |= Editor(value.constraints[i], i, value);
		ImGui::TreePop();
	}

	bool softBodiesOpen = ImGui::TreeNode("SoftBodies");

	if (ImGui::BeginPopupContextItem("SoftBodiesOptions")) {
		if (changed |= ImGui::Selectable("Add SoftBody")) {
			resources::ManagedCArray<SoftBody, unsigned int> softbodies{ resource, value.softBodies, value.softBodyCount };
			auto& softbody = softbodies.emplace_back();
			softbody = {};
			softbody.name = "softbody-name";
		}
		ImGui::EndPopup();
	}

	if (softBodiesOpen) {
		for (auto i = 0; i < value.softBodyCount; i++) changed |= Editor(value.softBodies[i], i, value);
		ImGui::TreePop();
	}

	return changed;
}

ResPhysicalSkeletonEditor::ResPhysicalSkeletonEditor(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
	if (goc)
		gocPhys = goc;
}

ResPhysicalSkeletonEditor* ResPhysicalSkeletonEditor::Create(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc) {
	return new (allocator) ResPhysicalSkeletonEditor(allocator, resource, goc);
}

void ResPhysicalSkeletonEditor::RenderContents()
{
	bool clickedExport = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Export", nullptr, &clickedExport);
		ImGui::EndMenuBar();
	}

	if (clickedExport) {
		IGFD::FileDialogConfig cfg{};
		cfg.path = GlobalSettings::defaultFileDialogDirectory;
		cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
		cfg.userDatas = resource->unpackedBinaryData;
		ImGuiFileDialog::Instance()->OpenDialog("ResPhysicalSkeletonExportDialog", "Choose File", ".pba", cfg);
	}
	if (ImGuiFileDialog::Instance()->Display("ResPhysicalSkeletonExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* exportData = static_cast<PbaData*>(ImGuiFileDialog::Instance()->GetUserDatas());

			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
			serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	if (Editor(*static_cast<PbaData*>(resource->unpackedBinaryData))) {
		resource->Reload(resource->unpackedBinaryData, resource->GetSize());
		if (gocPhys) {
			gocPhys->Deinitialize();
			gocPhys->Initialize((hh::anim::GOCAnimator*)gocPhys->gocAnimationSingle);
		}
	}
}
