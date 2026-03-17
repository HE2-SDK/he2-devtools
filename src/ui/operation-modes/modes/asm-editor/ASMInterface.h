#pragma once

#include <utilities/CompatibleObject.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
#define current_asm_version v103_rangers
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
#define current_asm_version v103_miller
#endif

namespace ui::operation_modes::modes::asm_editor {
	class ASMInterface : public CompatibleObject {
	public:
		typedef size_t Id;

		class IdRegistry : public CompatibleObject {
			short nextId{};
			csl::ut::PointerMap<Id, short> indicesById{ GetAllocator() };
			csl::ut::MoveArray<Id> ids{ GetAllocator() };

		public:
			IdRegistry(csl::fnd::IAllocator* allocator);
			Id Add();
			Id AddWithId(Id id);
			Id Add(short index);
			Id AddWithId(short index, Id id);
			void Remove(Id id);
			short Get(Id id) const;
			Id GetId(short idx) const;
			Id GetCount() const;
		};

		hh::fnd::Reference<hh::anim::ResAnimator> resource;
		hh::fnd::Reference<hh::anim::GOCAnimator> gocAnimator{};
		ucsl::resources::animation_state_machine::current_asm_version::AsmData& asmData{ *resource->binaryData };

		ASMInterface(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator* resource, hh::anim::GOCAnimator* gocAnimator);

		IdRegistry clipRegistry{ GetAllocator() };
		IdRegistry stateRegistry{ GetAllocator() };
		IdRegistry blendNodeRegistry{ GetAllocator() };
		IdRegistry flagRegistry{ GetAllocator() };
		IdRegistry variableRegistry{ GetAllocator() };
		IdRegistry blendMaskRegistry{ GetAllocator() };
		IdRegistry colliderRegistry{ GetAllocator() };
		IdRegistry blendSpaceRegistry{ GetAllocator() };

		void SetClipBlendMask(Id clipId, Id blendMaskId);
		void ClearClipBlendMask(Id clipId);
		unsigned short GetBlendNodeChildCount(Id blendNodeId) const;
		Id GetBlendNodeChild(Id blendNodeId, unsigned short childIndex) const;
		void SetBlendNodeClip(Id blendNodeId, Id clipId);
		void ClearBlendNodeClip(Id blendNodeId);
		//void AddBlendNodeNode(Id blendNodeId);
		//void RemoveBlendNodeNode(Id blendNodeId);
		void SetBlendNodeBlendSpace(Id blendNodeId, Id blendSpaceId);
		void ClearBlendNodeBlendSpace(Id blendNodeId);
		std::optional<Id> GetBlendNodeBlendSpace(Id blendNodeId) const;
		void SetBlendNodeBlendFactorVariable(Id blendNodeId, Id variableId);
		void ClearBlendNodeBlendFactorVariable(Id blendNodeId);
		void SetBlendSpaceClip(Id blendSpaceId, Id clipIndex, Id clipId);
		void ClearBlendSpaceClip(Id blendSpaceId, Id clipIndex);
		void SetBlendSpaceXVariable(Id blendSpaceId, Id variableId);
		void ClearBlendSpaceXVariable(Id blendSpaceId);
		void SetBlendSpaceYVariable(Id blendSpaceId, Id variableId);
		void ClearBlendSpaceYVariable(Id blendSpaceId);
		void SetStateClip(Id stateId, Id clipId);
		void ClearStateClip(Id stateId);
		void SetStateBlendNode(Id stateId, Id blendNodeId);
		void ClearStateBlendNode(Id stateId);

		Id AddState();
		Id AddBlendMask();
		Id AddBlendNode();
		Id AddChildBlendNode(Id parent);
		Id AddBlendSpace();
		Id AddClip();
		Id AddVariable();
		Id AddFlag();
		Id AddCollider();

		void RemoveState(Id id);
		void RemoveBlendMask(Id id);
		void RemoveBlendNode(Id id);
		void RemoveBlendSpace(Id id);
		void RemoveClip(Id id);
		void RemoveVariable(Id id);
		void RemoveFlag(Id id);
		void RemoveCollider(Id id);

		void AddStateFlag(Id stateId, Id flagId);
		void RemoveStateFlag(Id stateId, Id flagId);
		//void AddBlendNodeChild(Id blendNodeId, Id childId);
		//void RemoveBlendNodeChild(Id blendNodeId, Id childId);
		void AddClipChild(Id clipId, Id childId);
		void RemoveClipChild(Id clipId, Id childId);

		void SetBlendNodeType(Id blendNodeId, ucsl::resources::animation_state_machine::current_asm_version::BlendNodeType type);
		void AddBlendSpaceNode(Id blendSpaceId, const ucsl::math::Vector2& position);
		void RemoveBlendSpaceNode(Id blendSpaceId, unsigned short index);
		void TriangulateBlendSpace(Id blendSpaceId);

		ucsl::resources::animation_state_machine::current_asm_version::ClipData& GetClip(Id id) const;
		ucsl::resources::animation_state_machine::current_asm_version::StateData& GetState(Id id) const;
		ucsl::resources::animation_state_machine::current_asm_version::BlendNodeData& GetBlendNode(Id id) const;
		ucsl::resources::animation_state_machine::current_asm_version::BlendMaskData& GetBlendMask(Id id) const;
		ucsl::resources::animation_state_machine::current_asm_version::BlendSpaceData& GetBlendSpace(Id id) const;
		const char*& GetFlag(Id id) const;
		const char*& GetVariable(Id id) const;
		const char*& GetCollider(Id id) const;

		void ReloadResource();

	private:
		void AddStateFlag(ucsl::resources::animation_state_machine::current_asm_version::StateData& state, Id flagId);
		void RemoveStateFlag(ucsl::resources::animation_state_machine::current_asm_version::StateData& state, Id flagId);
		void AddClipChild(ucsl::resources::animation_state_machine::current_asm_version::ClipData& clip, Id childId);
		void RemoveClipChild(ucsl::resources::animation_state_machine::current_asm_version::ClipData& clip, Id childId);

		void AdjustIndexForAddition(short& index, short addedIdx);
		void AdjustOffsetAndCountForAddition(short& offset, unsigned short& count, short addedIdx);
		void AdjustIndexForRemoval(short& index, short removedIdx);
		void AdjustOffsetAndCountForRemoval(short& offset, unsigned short& count, short removedIdx);

		void RemoveBlendNodeChildren(Id id);
		void CreateMinimalBlendNodeChildren(Id id);
		unsigned short GetMinimumChildNodeCount(Id id);
	};
}
