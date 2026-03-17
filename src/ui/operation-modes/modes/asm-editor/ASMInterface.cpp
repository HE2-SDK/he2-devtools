#include "ASMInterface.h"
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <resources/ReloadManager.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

namespace ui::operation_modes::modes::asm_editor {
	using namespace ucsl::resources::animation_state_machine::current_asm_version;

	ASMInterface::IdRegistry::IdRegistry(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
	}

	ASMInterface::Id ASMInterface::IdRegistry::Add() {
		return AddWithId(nextId++);
	}

	ASMInterface::Id ASMInterface::IdRegistry::AddWithId(Id id) {
		short idx = (short)ids.size();

		indicesById.Insert(id, idx);
		ids.push_back(id);

		return id;
	}

	ASMInterface::Id ASMInterface::IdRegistry::Add(short idx) {
		return AddWithId(idx, nextId++);
	}

	ASMInterface::Id ASMInterface::IdRegistry::AddWithId(short idx, Id id) {
		for (auto it = indicesById.begin(); it != indicesById.end(); it++)
			if (*it >= idx)
				(*it)++;

		indicesById.Insert(id, idx);
		ids.emplace_back();

		for (size_t j = ids.size() - 1; j >= idx + 1; j--)
			ids[j] = ids[j - 1];

		ids[idx] = id;

		return id;
	}

	void ASMInterface::IdRegistry::Remove(Id id) {
		short idx = indicesById.GetValueOrFallback(id, -1);

		indicesById.Erase(id);

		for (auto it = indicesById.begin(); it != indicesById.end(); it++)
			if (*it > idx)
				(*it)--;

		ids.remove(idx);
	}

	short ASMInterface::IdRegistry::Get(Id id) const {
		return indicesById.GetValueOrFallback(id, -1);
	}

	ASMInterface::Id ASMInterface::IdRegistry::GetId(short idx) const {
		return ids[idx];
	}

	ASMInterface::Id ASMInterface::IdRegistry::GetCount() const {
		return ids.size();
	}

	ASMInterface::ASMInterface(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator* resource, hh::anim::GOCAnimator* gocAnimator)
		: CompatibleObject{ allocator }
		, resource{ resource }
		, gocAnimator{ gocAnimator } {
		for (unsigned short i = 0; i < asmData.variableCount; i++)
			variableRegistry.Add();
		for (unsigned short i = 0; i < asmData.blendMaskCount; i++)
			blendMaskRegistry.Add();
		for (unsigned short i = 0; i < asmData.clipCount; i++)
			clipRegistry.Add();
		for (unsigned short i = 0; i < asmData.flagCount; i++)
			flagRegistry.Add();
		for (unsigned short i = 0; i < asmData.blendSpaceCount; i++)
			blendSpaceRegistry.Add();
		for (unsigned short i = 0; i < asmData.stateCount; i++)
			stateRegistry.Add();
		for (unsigned short i = 0; i < asmData.blendNodeCount; i++)
			blendNodeRegistry.Add();
		for (unsigned short i = 0; i < asmData.colliderCount; i++)
			colliderRegistry.Add();
	}

	void ASMInterface::SetClipBlendMask(Id clipId, Id blendMaskId) {
		GetClip(clipId).blendMaskIndex = blendMaskRegistry.Get(blendMaskId);
	}

	void ASMInterface::ClearClipBlendMask(Id clipId) {
		GetClip(clipId).blendMaskIndex = -1;
	}

	unsigned short ASMInterface::GetBlendNodeChildCount(Id blendNodeId) const {
		auto& blendNode = GetBlendNode(blendNodeId);

		return blendNode.childNodeArrayOffset == -1 || blendNode.childNodeArraySize == 0 ? 0 : blendNode.childNodeArraySize;
	}

	ASMInterface::Id ASMInterface::GetBlendNodeChild(Id blendNodeId, unsigned short childIndex) const {
		short idx = GetBlendNode(blendNodeId).childNodeArrayOffset + childIndex;

		return blendNodeRegistry.GetId(idx);
	}

	void ASMInterface::SetBlendNodeClip(Id blendNodeId, Id clipId) {
		GetBlendNode(blendNodeId).childNodeArrayOffset = clipRegistry.Get(clipId);
	}

	void ASMInterface::ClearBlendNodeClip(Id blendNodeId) {
		GetBlendNode(blendNodeId).childNodeArrayOffset = -1;
	}

	//void ASMInterface::AddBlendNodeNode(Id blendNodeId) {
	//	auto& blendNode = GetBlendNode
	//	resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::StateData, int> states{ resource, asmData.states, asmData.stateCount };
	//}

	//void ASMInterface::RemoveBlendNodeNode(Id blendNodeId) {
	//}

	void ASMInterface::SetBlendNodeBlendSpace(Id blendNodeId, Id blendSpaceId) {
		short blendSpaceIdx = blendSpaceRegistry.Get(blendSpaceId);
		auto& blendSpace = GetBlendSpace(blendSpaceId);

		GetBlendNode(blendNodeId).blendSpaceIndex = blendSpaceIdx;

		RemoveBlendNodeChildren(blendNodeId);
		CreateMinimalBlendNodeChildren(blendNodeId);
	}

	void ASMInterface::ClearBlendNodeBlendSpace(Id blendNodeId) {
		GetBlendNode(blendNodeId).blendSpaceIndex = -1;

		RemoveBlendNodeChildren(blendNodeId);
	}

	std::optional<ASMInterface::Id> ASMInterface::GetBlendNodeBlendSpace(Id blendNodeId) const {
		short idx = GetBlendNode(blendNodeId).blendSpaceIndex;

		return idx == -1 ? std::nullopt : std::make_optional(blendSpaceRegistry.GetId(idx));
	}

	void ASMInterface::SetBlendNodeBlendFactorVariable(Id blendNodeId, Id variableId) {
		GetBlendNode(blendNodeId).blendFactorVariableIndex = variableRegistry.Get(variableId);
	}

	void ASMInterface::ClearBlendNodeBlendFactorVariable(Id blendNodeId) {
		GetBlendNode(blendNodeId).blendFactorVariableIndex = -1;
	}

	void ASMInterface::SetBlendSpaceClip(Id blendSpaceId, Id clipIndex, Id clipId) {
		GetBlendSpace(blendSpaceId).clipIndices[clipIndex] = clipRegistry.Get(clipId);
	}

	void ASMInterface::ClearBlendSpaceClip(Id blendSpaceId, Id clipIndex) {
		GetBlendSpace(blendSpaceId).clipIndices[clipIndex] = -1;
	}

	void ASMInterface::SetBlendSpaceXVariable(Id blendSpaceId, Id variableId) {
		GetBlendSpace(blendSpaceId).xVariableIndex = variableRegistry.Get(variableId);
	}

	void ASMInterface::ClearBlendSpaceXVariable(Id blendSpaceId) {
		GetBlendSpace(blendSpaceId).xVariableIndex = -1;
	}

	void ASMInterface::SetBlendSpaceYVariable(Id blendSpaceId, Id variableId) {
		GetBlendSpace(blendSpaceId).yVariableIndex = variableRegistry.Get(variableId);
	}

	void ASMInterface::ClearBlendSpaceYVariable(Id blendSpaceId) {
		GetBlendSpace(blendSpaceId).yVariableIndex = -1;
	}

	void ASMInterface::SetStateClip(Id stateId, Id clipId) {
		GetState(stateId).rootBlendNodeOrClipIndex = clipRegistry.Get(clipId);
	}

	void ASMInterface::ClearStateClip(Id stateId) {
		GetState(stateId).rootBlendNodeOrClipIndex = -1;
	}

	void ASMInterface::SetStateBlendNode(Id stateId, Id blendNodeId) {
		GetState(stateId).rootBlendNodeOrClipIndex = blendNodeRegistry.Get(blendNodeId);
	}

	void ASMInterface::ClearStateBlendNode(Id stateId) {
		GetState(stateId).rootBlendNodeOrClipIndex = -1;
	}

	ASMInterface::Id ASMInterface::AddState() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::StateData, int> states{ resource, asmData.states, asmData.stateCount };

		auto& state = states.emplace_back();
		state.name = "new state";
		state.type = StateType::CLIP;
		state.transitImmediately = false;
		state.flags = {};
		state.defaultLayerIndex = 0;
		state.rootBlendNodeOrClipIndex = -1;
		state.maxCycles = 1;
		state.speed = 1.0f;
		state.speedVariableIndex = -1;
		state.eventCount = 0;
		state.eventOffset = -1;
		state.transitionArrayIndex = -1;
		state.stateEndTransition.transitionInfo.type = TransitionType::IMMEDIATE;
		state.stateEndTransition.transitionInfo.easingType = TransitionEasingType::LINEAR;
		state.stateEndTransition.transitionInfo.targetStateIndex = -1;
		state.stateEndTransition.transitionInfo.transitionTime = 1.0f;
		state.flagIndexCount = 0;
		state.flagIndexOffset = -1;
#ifdef DEVTOOLS_TARGET_SDK_miller
		state.pbaBlendFactor = 0.0f;
#endif

		return stateRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddBlendMask() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::BlendMaskData, int> blendMasks{ resource, asmData.blendMasks, asmData.blendMaskCount };

		auto& blendMask = blendMasks.emplace_back();
		blendMask.name = "new mask";
		blendMask.maskBoneCount = 0;
		blendMask.maskBoneOffset = -1;

		return blendMaskRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddBlendNode() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData, int> blendNodes{ resource, asmData.blendNodes, asmData.blendNodeCount };

		auto& blendNode = blendNodes.emplace_back();
		blendNode.type = ucsl::resources::animation_state_machine::current_asm_version::BlendNodeType::CLIP;
		blendNode.blendSpaceIndex = -1;
		blendNode.blendFactorVariableIndex = -1;
		blendNode.blendFactorTarget = 0.0f;
		blendNode.childNodeArraySize = 0;
		blendNode.childNodeArrayOffset = -1;

		return blendNodeRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddChildBlendNode(Id parentId) {
		auto& parent = GetBlendNode(parentId);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData, int> blendNodes{ resource, asmData.blendNodes, asmData.blendNodeCount };

		if (parent.childNodeArraySize == 0) {
			auto& blendNode = blendNodes.emplace_back();
			blendNode.type = ucsl::resources::animation_state_machine::current_asm_version::BlendNodeType::CLIP;
			blendNode.blendSpaceIndex = -1;
			blendNode.blendFactorVariableIndex = -1;
			blendNode.blendFactorTarget = 0.0f;
			blendNode.childNodeArraySize = 0;
			blendNode.childNodeArrayOffset = -1;

			auto resId = blendNodeRegistry.Add();

			auto& parent2 = GetBlendNode(parentId);
			parent2.childNodeArrayOffset = blendNodes.size() - 1;
			parent2.childNodeArraySize = 1;

			return resId;
		}
		else {
			auto newItemIdx = parent.childNodeArrayOffset + parent.childNodeArraySize;
			auto& blendNode = blendNodes.emplace(newItemIdx);
			blendNode.type = ucsl::resources::animation_state_machine::current_asm_version::BlendNodeType::CLIP;
			blendNode.blendSpaceIndex = -1;
			blendNode.blendFactorVariableIndex = -1;
			blendNode.blendFactorTarget = 0.0f;
			blendNode.childNodeArraySize = 0;
			blendNode.childNodeArrayOffset = -1;

			for (auto& b : std::span(asmData.blendNodes, asmData.blendNodeCount))
				if (b.type != BlendNodeType::CLIP)
					AdjustOffsetAndCountForAddition(b.childNodeArrayOffset, b.childNodeArraySize, newItemIdx);

			for (auto& state : std::span(asmData.states, asmData.stateCount))
				if (state.type == StateType::BLEND_TREE)
					AdjustIndexForAddition(state.rootBlendNodeOrClipIndex, newItemIdx);

			AdjustIndexForAddition(asmData.blendTreeRootNodeId, newItemIdx);

			auto resId = blendNodeRegistry.Add(newItemIdx);

			auto& parent2 = GetBlendNode(parentId);
			parent2.childNodeArraySize++;

			return resId;
		}
	}

	ASMInterface::Id ASMInterface::AddBlendSpace() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::BlendSpaceData, int> blendSpaces{ resource, asmData.blendSpaces, asmData.blendSpaceCount };

		auto& blendSpace = blendSpaces.emplace_back();
		blendSpace.xVariableIndex = -1;
		blendSpace.yVariableIndex = -1;
		blendSpace.xMin = -1.0f;
		blendSpace.xMax = 1.0f;
		blendSpace.yMin = -1.0f;
		blendSpace.yMax = 1.0f;
		blendSpace.nodeCount = 0;
		blendSpace.triangleCount = 0;
		blendSpace.nodes = nullptr;
		blendSpace.clipIndices = nullptr;
		blendSpace.triangles = nullptr;

		return blendSpaceRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddClip() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<ucsl::resources::animation_state_machine::current_asm_version::ClipData, int> clips{ resource, asmData.clips, asmData.clipCount };

		auto& newClip = clips.emplace_back();
		newClip.name = "new clip";
		newClip.animationSettings.resourceName = "";
		newClip.animationSettings.start = 0.0f;
		newClip.animationSettings.end = -1.0f;
		newClip.animationSettings.speed = 1.0f;
		newClip.animationSettings.flags = {};
		newClip.animationSettings.loops = false;
		newClip.triggerCount = 0;
		newClip.triggerOffset = -1;
		newClip.blendMaskIndex = -1;
		newClip.childClipIndexCount = 0;
		newClip.childClipIndexOffset = -1;

		return clipRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddVariable() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<const char*, int> variables{ resource, asmData.variables, asmData.variableCount };

		auto& variable = variables.emplace_back();
		variable = "new variable";

		return variableRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddFlag() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<const char*, int> flags{ resource, asmData.flags, asmData.flagCount };

		auto& flag = flags.emplace_back();
		flag = "new flag";

		return flagRegistry.Add();
	}

	ASMInterface::Id ASMInterface::AddCollider() {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		resources::ManagedCArray<const char*, int> colliders{ resource, asmData.colliders, asmData.colliderCount };

		auto& collider = colliders.emplace_back();
		collider = "new collider";

		return colliderRegistry.Add();
	}

	void ASMInterface::RemoveState(Id id) {
		short idx = stateRegistry.Get(id);

		resources::ManagedCArray<StateData, int> states{ resource, asmData.states, asmData.stateCount };

		states.remove(idx);

		for (auto& state : std::span(asmData.states, asmData.stateCount))
			AdjustIndexForRemoval(state.stateEndTransition.transitionInfo.targetStateIndex, idx);

		for (auto& event : std::span(asmData.events, asmData.eventCount))
			AdjustIndexForRemoval(event.transition.transitionInfo.targetStateIndex, idx);

		for (auto& transition : std::span(asmData.transitions, asmData.transitionCount))
			AdjustIndexForRemoval(transition.transitionInfo.targetStateIndex, idx);

		AdjustIndexForRemoval(asmData.nullTransition.transitionInfo.targetStateIndex, idx);

		stateRegistry.Remove(id);
	}

	void ASMInterface::RemoveBlendMask(Id id) {
		short idx = blendMaskRegistry.Get(id);

		resources::ManagedCArray<BlendMaskData, int> blendMasks{ resource, asmData.blendMasks, asmData.blendMaskCount };

		blendMasks.remove(idx);

		for (auto& clip : std::span(asmData.clips, asmData.clipCount))
			AdjustIndexForRemoval(clip.blendMaskIndex, idx);

		for (auto& layer : std::span(asmData.layers, asmData.layerCount))
			AdjustIndexForRemoval(layer.blendMaskIndex, idx);

		blendMaskRegistry.Remove(id);
	}

	void ASMInterface::RemoveBlendNode(Id id) {
		RemoveBlendNodeChildren(id);

		short idx = blendNodeRegistry.Get(id);

		resources::ManagedCArray<BlendNodeData, int> blendNodes{ resource, asmData.blendNodes, asmData.blendNodeCount };

		blendNodes.remove(idx);

		for (auto& blendNode : std::span(asmData.blendNodes, asmData.blendNodeCount))
			if (blendNode.type != BlendNodeType::CLIP)
				AdjustOffsetAndCountForRemoval(blendNode.childNodeArrayOffset, blendNode.childNodeArraySize, idx);

		for (auto& state : std::span(asmData.states, asmData.stateCount))
			if (state.type == StateType::BLEND_TREE)
				AdjustIndexForRemoval(state.rootBlendNodeOrClipIndex, idx);

		AdjustIndexForRemoval(asmData.blendTreeRootNodeId, idx);

		blendNodeRegistry.Remove(id);
	}

	void ASMInterface::RemoveBlendSpace(Id id) {
		short idx = blendSpaceRegistry.Get(id);

		resources::ManagedCArray<BlendSpaceData, int> blendSpaces{ resource, asmData.blendSpaces, asmData.blendSpaceCount };

		blendSpaces.remove(idx);

		for (auto& blendNode : std::span(asmData.blendNodes, asmData.blendNodeCount))
			AdjustIndexForRemoval(blendNode.blendSpaceIndex, idx);

		blendSpaceRegistry.Remove(id);
	}

	void ASMInterface::RemoveClip(Id id) {
		short idx = clipRegistry.Get(id);

		resources::ManagedCArray<ClipData, int> clips{ resource, asmData.clips, asmData.clipCount };

		clips.remove(idx);

		for (auto& blendNode : std::span(asmData.blendNodes, asmData.blendNodeCount))
			if (blendNode.type == BlendNodeType::CLIP)
				AdjustIndexForRemoval(blendNode.childNodeArrayOffset, idx);

		for (auto& state : std::span(asmData.states, asmData.stateCount))
			if (state.type == StateType::CLIP)
				AdjustIndexForRemoval(state.rootBlendNodeOrClipIndex, idx);

		for (auto& blendSpace : std::span(asmData.blendSpaces, asmData.blendSpaceCount))
			for (auto& clipIndex : std::span(blendSpace.clipIndices, blendSpace.nodeCount))
				AdjustIndexForRemoval(clipIndex, idx);

		for (auto& clip : std::span(asmData.clips, asmData.clipCount))
			RemoveClipChild(clip, idx);

		clipRegistry.Remove(id);
	}

	void ASMInterface::RemoveVariable(Id id) {
		short idx = variableRegistry.Get(id);

		resources::ManagedCArray<const char*, int> variables{ resource, asmData.variables, asmData.variableCount };

		variables.remove(idx);

		for (auto& blendNode : std::span(asmData.blendNodes, asmData.blendNodeCount))
			AdjustIndexForRemoval(blendNode.blendFactorVariableIndex, idx);

		for (auto& state : std::span(asmData.states, asmData.stateCount)) {
			AdjustIndexForRemoval(state.speedVariableIndex, idx);
			AdjustIndexForRemoval(state.stateEndTransition.transitionTimeVariableIndex, idx);
		}

		for (auto& transition : std::span(asmData.transitions, asmData.transitionCount))
			AdjustIndexForRemoval(transition.transitionTimeVariableIndex, idx);

		AdjustIndexForRemoval(asmData.nullTransition.transitionTimeVariableIndex, idx);

		variableRegistry.Remove(id);
	}

	void ASMInterface::RemoveFlag(Id id) {
		short idx = flagRegistry.Get(id);

		resources::ManagedCArray<const char*, int> flags{ resource, asmData.flags, asmData.flagCount };

		flags.remove(idx);

		for (auto& state : std::span(asmData.states, asmData.stateCount))
			RemoveStateFlag(state, id);

		flagRegistry.Remove(id);
	}

	void ASMInterface::RemoveCollider(Id id) {
		short idx = colliderRegistry.Get(id);

		resources::ManagedCArray<const char*, int> colliders{ resource, asmData.colliders, asmData.colliderCount };

		colliders.remove(idx);

		for (auto& trigger : std::span(asmData.triggers, asmData.triggerCount))
			AdjustIndexForRemoval(trigger.colliderIndex, idx);

		colliderRegistry.Remove(id);
	}

	void ASMInterface::AddStateFlag(Id stateId, Id flagId) {
		AddStateFlag(GetState(stateId), flagId);
	}

	void ASMInterface::RemoveStateFlag(Id stateId, Id flagId) {
		RemoveStateFlag(GetState(stateId), flagId);
	}

	//void ASMInterface::AddBlendNodeChild(Id blendNodeId, Id childId) {
	//	auto& blendNode = GetBlendNode(blendNodeId);
	//	auto childNodeIdx = blendNodeRegistry.Get(childId);

	//	resources::ManagedCArray<BlendNodeData, int> blendNodes{ resource, asmData.blendNodes, asmData.blendNodeCount };

	//	if (childNodeIdx >= blendNode.childNodeArrayOffset && childNodeIdx < blendNode.childNodeArrayOffset + blendNode.childNodeArraySize)
	//		return;

	//	ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData child = blendNodes[childNodeIdx];

	//	RemoveBlendNode(childId);

	//	auto& blendNode2 = GetBlendNode(blendNodeId);

	//	if (blendNode2.childNodeArraySize == 0) {
	//		blendNodes.emplace_back() = child;

	//		auto& blendNode3 = GetBlendNode(blendNodeId);
	//		blendNode3.childNodeArrayOffset = blendNodes.size() - 1;
	//		blendNode3.childNodeArraySize = 1;

	//		blendNodeRegistry.AddWithId(childId);
	//	}
	//	else {
	//		auto newItemIdx = blendNode2.childNodeArrayOffset + blendNode2.childNodeArraySize;
	//		blendNodes.emplace(newItemIdx) = child;

	//		auto& blendNode3 = GetBlendNode(blendNodeId);

	//		for (auto& b : std::span(asmData.blendNodes, asmData.blendNodeCount))
	//			if (b.type != BlendNodeType::CLIP)
	//				AdjustOffsetAndCountForAddition(b.childNodeArrayOffset, b.childNodeArraySize, newItemIdx);

	//		blendNode3.childNodeArraySize++;

	//		blendNodeRegistry.AddWithId(newItemIdx, childId);
	//	}
	//}

	//void ASMInterface::RemoveBlendNodeChild(Id blendNodeId, Id childId) {
	//	auto& blendNode = GetBlendNode(blendNodeId);
	//	auto childNodeIdx = blendNodeRegistry.Get(childId);

	//	resources::ManagedCArray<BlendNodeData, int> blendNodes{ resource, asmData.blendNodes, asmData.blendNodeCount };

	//	if (childNodeIdx < blendNode.childNodeArrayOffset || childNodeIdx >= blendNode.childNodeArrayOffset + blendNode.childNodeArraySize)
	//		return;

	//	ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData child = blendNodes[childNodeIdx];

	//	RemoveBlendNode(childId);

	//	blendNodes.emplace_back() = child;

	//	blendNodeRegistry.AddWithId(childId);
	//}

	void ASMInterface::AddClipChild(Id clipId, Id childId) {
		AddClipChild(GetClip(clipId), childId);
	}

	void ASMInterface::RemoveClipChild(Id clipId, Id childId) {
		RemoveClipChild(GetClip(clipId), childId);
	}

	void ASMInterface::SetBlendNodeType(Id blendNodeId, BlendNodeType type) {
		RemoveBlendNodeChildren(blendNodeId);

		auto& blendNode = GetBlendNode(blendNodeId);

		blendNode.type = type;
		blendNode.blendSpaceIndex = -1;

		CreateMinimalBlendNodeChildren(blendNodeId);
	}

	void ASMInterface::AddBlendSpaceNode(Id blendSpaceId, const ucsl::math::Vector2& position) {
		auto& blendSpace = GetBlendSpace(blendSpaceId);

		unsigned short clipIndicesCount{ blendSpace.nodeCount };

		resources::ManagedCArray<ucsl::math::Vector2, unsigned short> nodes{ resource, blendSpace.nodes, blendSpace.nodeCount };
		resources::ManagedCArray<short, unsigned short> clipIndices{ resource, blendSpace.clipIndices, clipIndicesCount };

		nodes.emplace_back() = position;
		clipIndices.emplace_back() = -1;

		TriangulateBlendSpace(blendSpaceId);

		csl::ut::MoveArray<Id> nodeIds{ hh::fnd::GetTempAllocator() };
		auto blendSpaceIdx = blendSpaceRegistry.Get(blendSpaceId);
		for (unsigned short i = 0; i < asmData.blendNodeCount; i++) {
			auto& node = asmData.blendNodes[i];

			if (node.type == BlendNodeType::BLEND_SPACE && node.blendSpaceIndex == blendSpaceIdx)
				nodeIds.push_back(blendNodeRegistry.GetId(i));
		}

		for (auto nodeId : nodeIds)
			AddChildBlendNode(nodeId);
	}

	void ASMInterface::RemoveBlendSpaceNode(Id blendSpaceId, unsigned short index) {
		auto& blendSpace = GetBlendSpace(blendSpaceId);

		// Make a copy because otherwise we'll change it for both by editing one of the managed arrays.
		unsigned short clipIndicesCount{ blendSpace.nodeCount };

		resources::ManagedCArray<ucsl::math::Vector2, unsigned short> nodes{ resource, blendSpace.nodes, blendSpace.nodeCount };
		resources::ManagedCArray<short, unsigned short> clipIndices{ resource, blendSpace.clipIndices, clipIndicesCount };

		nodes.remove(index);
		clipIndices.remove(index);

		TriangulateBlendSpace(blendSpaceId);

		csl::ut::MoveArray<Id> nodeIds{ hh::fnd::GetTempAllocator() };
		auto blendSpaceIdx = blendSpaceRegistry.Get(blendSpaceId);
		for (unsigned short i = 0; i < asmData.blendNodeCount; i++) {
			auto& node = asmData.blendNodes[i];

			if (node.type == BlendNodeType::BLEND_SPACE && node.blendSpaceIndex == blendSpaceIdx)
				nodeIds.push_back(blendNodeRegistry.GetId(i));
		}

		for (auto nodeId : nodeIds)
			RemoveBlendNode(GetBlendNodeChild(nodeId, index));
	}

	void ASMInterface::TriangulateBlendSpace(Id blendSpaceId) {
		auto& blendSpace = GetBlendSpace(blendSpaceId);

		resources::ManagedCArray<BlendSpaceData::Triangle, unsigned short> triangles{ resource, blendSpace.triangles, blendSpace.triangleCount };

		triangles.clear();

		if (blendSpace.nodeCount >= 3) {
			CGAL::Delaunay_triangulation_2<CGAL::Exact_predicates_inexact_constructions_kernel> triangulation{};
			std::map<CGAL::Delaunay_triangulation_2<CGAL::Exact_predicates_inexact_constructions_kernel>::Vertex_handle, short> indices{};

			for (short i = 0; i < blendSpace.nodeCount; i++)
				indices[triangulation.insert({ blendSpace.nodes[i].x(), blendSpace.nodes[i].y() })] = i;

			for (auto it = triangulation.finite_faces_begin(); it != triangulation.finite_faces_end(); it++) {
				auto& face = *it;

				triangles.push_back({ indices[face.vertex(0)], indices[face.vertex(1)], indices[face.vertex(2)], 0 });
			}
		}
	}

	ucsl::resources::animation_state_machine::current_asm_version::ClipData& ASMInterface::GetClip(Id id) const {
		return asmData.clips[clipRegistry.Get(id)];
	}

	ucsl::resources::animation_state_machine::current_asm_version::StateData& ASMInterface::GetState(Id id) const {
		return asmData.states[stateRegistry.Get(id)];
	}

	ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData& ASMInterface::GetBlendNode(Id id) const {
		return asmData.blendNodes[blendNodeRegistry.Get(id)];
	}

	ucsl::resources::animation_state_machine::current_asm_version::BlendMaskData& ASMInterface::GetBlendMask(Id id) const {
		return asmData.blendMasks[blendMaskRegistry.Get(id)];
	}

	ucsl::resources::animation_state_machine::current_asm_version::BlendSpaceData& ASMInterface::GetBlendSpace(Id id) const {
		return asmData.blendSpaces[blendSpaceRegistry.Get(id)];
	}

	const char*& ASMInterface::GetFlag(Id id) const {
		return asmData.flags[flagRegistry.Get(id)];
	}

	const char*& ASMInterface::GetVariable(Id id) const {
		return asmData.variables[variableRegistry.Get(id)];
	}

	const char*& ASMInterface::GetCollider(Id id) const {
		return asmData.colliders[colliderRegistry.Get(id)];
	}

	void ASMInterface::ReloadResource() {
		ReloadManager::instance->ReloadSync(resource);

		gocAnimator->asmResourceManager->clipBindInfos.clear();
		gocAnimator->asmResourceManager->stateBindInfos.clear();
		gocAnimator->asmResourceManager->blendMaskInfos.clear();
		gocAnimator->asmResourceManager->defaultBlendMaskInfo.boneMasks.clear();
		gocAnimator->asmResourceManager->unk5.boneMasks.clear();
		gocAnimator->asmResourceManager->Setup({
			.animatorResource = resource,
			.skeletonResource = gocAnimator->asmResourceManager->skeletonResource,
			.resourceNameResolver = nullptr,
		});
	}

	void ASMInterface::AddStateFlag(ucsl::resources::animation_state_machine::current_asm_version::StateData& state, Id flagId) {
		auto flagIdx = flagRegistry.Get(flagId);

		resources::ManagedCArray<short, int> flagIndices{ resource, asmData.flagIndices, asmData.flagIndexCount };

		if (state.flagIndexCount == 0) {
			flagIndices.emplace_back() = flagIdx;

			state.flagIndexOffset = flagIndices.size() - 1;
			state.flagIndexCount = 1;
		}
		else {
			auto newItemIdx = state.flagIndexOffset + state.flagIndexCount;
			flagIndices.emplace(newItemIdx) = flagIdx;

			for (auto& s : std::span(asmData.states, asmData.stateCount))
				AdjustOffsetAndCountForAddition(s.flagIndexOffset, s.flagIndexCount, newItemIdx);

			state.flagIndexCount++;
		}
	}

	void ASMInterface::RemoveStateFlag(ucsl::resources::animation_state_machine::current_asm_version::StateData& state, Id flagId) {
		auto flagIdx = flagRegistry.Get(flagId);

		resources::ManagedCArray<short, int> flagIndices{ resource, asmData.flagIndices, asmData.flagIndexCount };

		auto it = std::find_if(flagIndices.begin() + state.flagIndexOffset, flagIndices.begin() + state.flagIndexOffset + state.flagIndexCount, [&](short idx) { return idx == flagIdx; });
		if (it == flagIndices.end())
			return;

		auto i = (int)std::distance(flagIndices.begin(), it);

		flagIndices.remove(i);

		for (auto& s : std::span(asmData.states, asmData.stateCount))
			AdjustOffsetAndCountForRemoval(s.flagIndexOffset, s.flagIndexCount, i);
	}

	void ASMInterface::AddClipChild(ucsl::resources::animation_state_machine::current_asm_version::ClipData& clip, Id childId) {
		auto childIdx = clipRegistry.Get(childId);

		resources::ManagedCArray<short, int> childClips{ resource, asmData.childClipIndices, asmData.childClipIndexCount };

		if (clip.childClipIndexCount == 0) {
			childClips.emplace_back() = childIdx;

			clip.childClipIndexOffset = childClips.size() - 1;
			clip.childClipIndexCount = 1;
		}
		else {
			auto newItemIdx = clip.childClipIndexOffset + clip.childClipIndexCount;
			childClips.emplace(newItemIdx) = childIdx;

			for (auto& c : std::span(asmData.clips, asmData.clipCount))
				AdjustOffsetAndCountForAddition(c.childClipIndexOffset, c.childClipIndexCount, newItemIdx);

			clip.childClipIndexCount++;
		}
	}

	void ASMInterface::RemoveClipChild(ucsl::resources::animation_state_machine::current_asm_version::ClipData& clip, Id childId) {
		auto childIdx = clipRegistry.Get(childId);

		resources::ManagedCArray<short, int> childClips{ resource, asmData.childClipIndices, asmData.childClipIndexCount };

		auto it = std::find_if(childClips.begin() + clip.childClipIndexOffset, childClips.begin() + clip.childClipIndexOffset + clip.childClipIndexCount, [&](short idx) { return idx == childIdx; });
		if (it == childClips.end())
			return;

		auto i = (int)std::distance(childClips.begin(), it);

		childClips.remove(i);

		for (auto& c : std::span(asmData.clips, asmData.clipCount))
			AdjustOffsetAndCountForRemoval(c.childClipIndexOffset, c.childClipIndexCount, i);
	}

	void ASMInterface::AdjustIndexForAddition(short& index, short addedIdx) {
		if (index == -1)
			return;

		if (index >= addedIdx)
			index++;
	}

	void ASMInterface::AdjustOffsetAndCountForAddition(short& offset, unsigned short& count, short addedIdx) {
		if (offset == -1 || count == 0)
			return;

		if (offset >= addedIdx)
			offset++;
	}

	void ASMInterface::AdjustIndexForRemoval(short& index, short removedIdx) {
		if (index == -1)
			return;

		if (index == removedIdx)
			index = -1;
		else if (index > removedIdx)
			index--;
	}

	void ASMInterface::AdjustOffsetAndCountForRemoval(short& offset, unsigned short& count, short removedIdx) {
		if (offset == -1 || count == 0)
			return;

		if (offset <= removedIdx && removedIdx < offset + count) {
			count--;

			if (count == 0)
				offset = -1;
		}
		else if (removedIdx < offset)
			offset--;
	}

	void ASMInterface::RemoveBlendNodeChildren(Id id) {
		unsigned short childCount = GetBlendNodeChildCount(id);

		csl::ut::MoveArray<Id> childIds{ hh::fnd::GetTempAllocator() };

		for (unsigned short i = 0; i < childCount; i++)
			childIds.push_back(GetBlendNodeChild(id, i));

		for (auto id : childIds)
			RemoveBlendNode(id);
	}

	void ASMInterface::CreateMinimalBlendNodeChildren(Id id) {
		for (unsigned i = 0; i < GetMinimumChildNodeCount(id); i++)
			AddChildBlendNode(id);
	}

	unsigned short ASMInterface::GetMinimumChildNodeCount(Id id) {
		auto& blendNode = GetBlendNode(id);

		switch (blendNode.type) {
		case BlendNodeType::CLIP:
		case BlendNodeType::LAYER:
			return 0;
		case BlendNodeType::MULTIPLY:
			return 1;
		case BlendNodeType::LERP:
		case BlendNodeType::ADDITIVE:
		case BlendNodeType::OVERRIDE:
		case BlendNodeType::TWO_POINT_LERP:
			return 2;
		case BlendNodeType::BLEND_SPACE:
			return blendNode.blendSpaceIndex == -1 ? 0 : asmData.blendSpaces[blendNode.blendSpaceIndex].nodeCount;
		}
	}
}
