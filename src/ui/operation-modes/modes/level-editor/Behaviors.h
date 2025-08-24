#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include "Context.h"

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::game;

	template<> struct MousePickingPhysicsBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		using ObjectType = ObjectData*;
		const char* GetObjectName(ObjectData* obj) { return obj->name; }
		ObjectData* GetObjectForGameObject(GameObject* obj) { return obj->GetWorldObjectStatus()->objectData; }
		bool IsSelectable(GameObject* obj) {
			auto* focusedChunk = context.GetFocusedChunk();

			if (!focusedChunk)
				return false;

			auto* status = obj->GetWorldObjectStatus();

			return status != nullptr && focusedChunk->GetObjectIndexByObjectData(status->objectData) != -1;
		}
		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ObjectData*>& results) {
			auto* focusedChunk = context.GetFocusedChunk();

			if (!focusedChunk)
				return;

			for (auto* layer : focusedChunk->GetLayers())
				for (auto* object : layer->GetResource()->GetObjects())
					if (frustum.Test(object->transform.position))
						results.push_back(object);
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePickingPhysics.h>

namespace ui::operation_modes::modes::level_editor {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePickingPhysicsMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePickingPhysicsMousePicking3DBehaviorTraitsImpl::MousePickingPhysicsMousePicking3DBehaviorTraitsImpl;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DCameraManagerCameraProvider.h>

namespace ui::operation_modes::modes::level_editor {
	template<> struct MousePicking3DCameraProvider<Context> : MousePicking3DCameraManagerCameraProvider<Context> {
		using MousePicking3DCameraManagerCameraProvider::MousePicking3DCameraManagerCameraProvider;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3D.h>

namespace ui::operation_modes::modes::level_editor {
	template<> struct MousePickingBehaviorTraits<Context> : MousePicking3DMousePickingBehaviorTraitsImpl<Context> {
		using MousePicking3DMousePickingBehaviorTraitsImpl::MousePicking3DMousePickingBehaviorTraitsImpl;
	};
}

#ifndef DEVTOOLS_TARGET_SDK_rangers
namespace app::gfx {
	class GOCVisualGeometryInstance : public hh::game::GOComponent {
#ifdef DEVTOOLS_TARGET_SDK_wars
		GOCOMPONENT_CLASS_DECLARATION_INLINE_GETCLASS(GOCVisualGeometryInstance)
#else
		GOCOMPONENT_CLASS_DECLARATION(GOCVisualGeometryInstance)
#endif
	};
}
#endif

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::game;

	struct ClipboardEntry {
		Context& context;
		ObjectData* proto;

		ClipboardEntry(Context& context, ObjectData* proto) : context{ context }, proto{ context.CopyObjectForClipboard(proto) } {}
		ClipboardEntry(const ClipboardEntry& other) : context{ other.context }, proto{ context.CopyObjectForClipboard(other.proto) } {}
		ClipboardEntry(ClipboardEntry&& other) noexcept : context{ other.context }, proto{ other.proto } { other.proto = nullptr; }
		~ClipboardEntry() { context.TerminateClipboardObject(proto); }
	};

	template<> struct SelectionBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = ObjectData*;
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<ObjectData*>& objects, csl::geom::Aabb& aabb) {
			return CalcApproxAabb(context.GetFocusedChunk(), objects, aabb);
		}
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(ObjectData* obj) { return true; }
		bool IsRoot(ObjectData* obj) { return !obj->parentID.IsNonNull(); }
		ObjectData* GetParent(ObjectData* obj) { return context.GetFocusedChunk()->GetWorldObjectStatusByObjectId(obj->parentID).objectData; }
		Eigen::Affine3f GetSelectionSpaceTransform(ObjectData* obj) const { return ObjectTransformDataToAffine3f(obj->transform); }
		void SetSelectionSpaceTransform(ObjectData* obj, const Eigen::Affine3f& transform) {
			UpdateAbsoluteTransform(transform, *obj);
			context.RecalculateDependentTransforms(obj);
		}
	};

	template<> struct DeleteBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		void DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects) { context.DeleteObjects(objects); }
	};

	template<> struct ClipboardTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		using Entry = ClipboardEntry;
		bool IsRoot(ObjectData* obj) { return !obj->parentID.IsNonNull(); }
		ObjectData* GetParent(ObjectData* obj) { return context.GetFocusedChunk()->GetWorldObjectStatusByObjectId(obj->parentID).objectData; }
		ClipboardEntry CreateClipboardEntry(ObjectData* objData) { return { context, objData }; }
		ObjectData* CreateObject(const ClipboardEntry& entry, std::optional<ObjectData*> parent) {
			auto* obj = context.CopyObjectForPlacement(entry.proto);
			if (parent)
				obj->parentID = parent.value()->id;
			context.SpawnObject(obj);
			return obj;
		}
	};

	template<> struct ClipboardBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CanPlace() { return context.placementTargetLayer != nullptr; }
		void DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects) { context.DeleteObjects(objects); }
	};
}

template<> struct ObjectLocationVisual3DBehaviorTraits<ui::operation_modes::modes::level_editor::Context> : BehaviorTraitsImpl<ui::operation_modes::modes::level_editor::Context> {
	using BehaviorTraitsImpl::BehaviorTraitsImpl;
	static constexpr const char* dragDropCategory = "ObjectData";
	Eigen::Affine3f GetWorldTransform(hh::game::ObjectData* object) const { return ObjectTransformDataToAffine3f(object->transform); }
	const char* GetPrimaryTag(hh::game::ObjectData* object) const { return GetObjectName(object); }
	const char* GetSecondaryTag(hh::game::ObjectData* object) const { return object->gameObjectClass; }
	template<typename F> void ForInvisibleObjects(F f) const {
		auto* focusedChunk = context.GetFocusedChunk();

		if (!focusedChunk)
			return;

		for (auto& status : focusedChunk->GetObjectStatuses()) {
			auto* gameObject = focusedChunk->GetGameObject(status.objectData);

			if (gameObject && ((!gameObject->GetComponent<hh::gfx::GOCVisual>() || gameObject->GetComponent<hh::gfx::GOCVisual>() == gameObject->GetComponent<hh::gfx::GOCVisualDebugDraw>()) && !gameObject->GetComponent<app::gfx::GOCVisualGeometryInstance>()))
				f(status.objectData);
		}
	}
	template<typename F> void ForAllObjects(F f) const {
		auto* focusedChunk = context.GetFocusedChunk();

		if (!focusedChunk)
			return;

		for (auto& status : focusedChunk->GetObjectStatuses())
			f(status.objectData);
	}
};

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::game;

	template<> struct PlacementBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		constexpr static bool is3D = true;
		bool CanPlace() const { return context.placementTargetLayer != nullptr && context.objectClassToPlace != nullptr; }
		ObjectData* PlaceObject(csl::math::Vector3 location) { return context.SpawnObject(location); }
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = false;
	};
}

#include <ui/operation-modes/behaviors/Clipboard.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <ui/operation-modes/behaviors/Placement.h>
#include <ui/operation-modes/behaviors/GroundContextMenu.h>
#include <ui/operation-modes/behaviors/DebugCommentsVisual.h>
#include <ui/operation-modes/behaviors/ObjectLocationVisual3D.h>
