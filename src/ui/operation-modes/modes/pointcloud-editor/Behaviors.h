#pragma once
#include "Context.h"
#include "Math.h"
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/BoundingBoxes.h>
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include <span>

namespace ui::operation_modes::modes::pointcloud_editor {
	using namespace ucsl::resources::pointcloud::v2;

	template<> struct MousePicking3DRecursiveRaycastBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = InstanceData*;
		using LocationType = csl::math::Vector3;

		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		const char* GetObjectName(InstanceData* object) { return object->name; }
		bool IsSelectable(InstanceData* object) { return true; }
		bool Intersects(InstanceData* object, const Ray3f& ray) { return false; }
		void GetRootObjects(csl::ut::MoveArray<InstanceData*>& rootObjects) {}
		void GetChildren(InstanceData* object, csl::ut::MoveArray<InstanceData*>& children) {}
		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<InstanceData*>& results) {
			if (context.resource == nullptr)
				return;

			for (auto& instance : std::span(context.resource->pointcloudData->instances, context.resource->pointcloudData->instanceCount)) {
				if (frustum.Test(instance.position))
					results.push_back(&instance);
			}
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DRecursiveRaycastWithPhysicsLocation.h>

namespace ui::operation_modes::modes::pointcloud_editor {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl::MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DCameraManagerCameraProvider.h>

namespace ui::operation_modes::modes::pointcloud_editor {
	template<> struct MousePicking3DCameraProvider<Context> : MousePicking3DCameraManagerCameraProvider<Context> {
		using MousePicking3DCameraManagerCameraProvider::MousePicking3DCameraManagerCameraProvider;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3D.h>

namespace ui::operation_modes::modes::pointcloud_editor {
	template<> struct MousePickingBehaviorTraits<Context> : MousePicking3DMousePickingBehaviorTraitsImpl<Context> {
		using MousePicking3DMousePickingBehaviorTraitsImpl::MousePicking3DMousePickingBehaviorTraitsImpl;
	};
}

namespace ui::operation_modes::modes::pointcloud_editor {
	using namespace ucsl::resources::pointcloud::v2;

	template<> struct SelectionBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = InstanceData*;
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(InstanceData* obj) { return true; }
		bool IsRoot(InstanceData* obj) { return true; }
		InstanceData* GetParent(InstanceData* obj) { return nullptr; }
		Eigen::Affine3f GetSelectionSpaceTransform(InstanceData* obj) const { return GetInstanceTransform(*obj); }
		void SetSelectionSpaceTransform(InstanceData* obj, const Eigen::Affine3f& transform) { UpdateInstanceTransform(*obj, transform); }
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<InstanceData*>& objects, csl::geom::Aabb& aabb) {
			aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

			if (objects.size() == 0)
				return false;

			for (auto* instance : objects)
				aabb.AddPoint(instance->position);

			return true;
		}
	};
}

template<> struct ObjectLocationVisual3DBehaviorTraits<ui::operation_modes::modes::pointcloud_editor::Context> : BehaviorTraitsImpl<ui::operation_modes::modes::pointcloud_editor::Context> {
	using BehaviorTraitsImpl::BehaviorTraitsImpl;
	static constexpr const char* dragDropCategory = nullptr;
	Eigen::Affine3f GetWorldTransform(ucsl::resources::pointcloud::v2::InstanceData* object) const { return ui::operation_modes::modes::pointcloud_editor::GetInstanceTransform(*object); }
	const char* GetPrimaryTag(ucsl::resources::pointcloud::v2::InstanceData* object) const { return object->name; }
	const char* GetSecondaryTag(ucsl::resources::pointcloud::v2::InstanceData* object) const { return object->resourceName; }
	template<typename F> void ForInvisibleObjects(F f) const {
		if (context.resource == nullptr)
			return;
		
		if (auto* data = context.resource->pointcloudData)
			for (auto& instance : std::span(data->instances, data->instanceCount))
				f(&instance);
	}
	template<typename F> void ForAllObjects(F f) const {
		if (context.resource == nullptr)
			return;
		
		if (auto* data = context.resource->pointcloudData)
			for (auto& instance : std::span(data->instances, data->instanceCount))
				f(&instance);
	}
};
	
namespace ui::operation_modes::modes::pointcloud_editor {
	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = true;
	};

	template<> struct PlacementBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		constexpr static bool is3D = true;
		bool CanPlace() const { return true; }
		InstanceData* PlaceObject(const csl::math::Vector3& location) {
			return context.AddInstance(location);
		}
	};

	template<> struct DeleteBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		void DeleteObjects(const csl::ut::MoveArray<InstanceData*>& objects) {
			for (auto* obj : objects)
				context.RemoveInstance(*obj);
		}
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/ObjectLocationVisual3D.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/GroundContextMenu.h>
#include <ui/operation-modes/behaviors/Placement.h>
#include <ui/operation-modes/behaviors/Delete.h>
