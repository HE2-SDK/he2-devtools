#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/BoundingBoxes.h>
#include <utilities/NameHash.h>
#include <algorithm>
#include "Math.h"

namespace ui::operation_modes::modes::pointcloud_editor {
	using namespace ucsl::resources::pointcloud::v2;

	InstanceData* Context::AddInstance(const Eigen::Vector3f& position) {
		auto& pointcloudData = *resource->pointcloudData;

		resources::ManagedCArray<InstanceData, unsigned int> instances{ resource, pointcloudData.instances, pointcloudData.instanceCount };

		auto& instance = instances.emplace_back();
		instance.name = "new instance";
		instance.resourceName = "";
		instance.position = position;
		instance.rotation = { 0.0f, 0.0f, 0.0f };
		instance.rotationOrder = RotationOrder::XYZ;
		instance.scale = { 1.0f, 1.0f, 1.0f };
		instance.unk2 = 0;

		return &instance;
	}

	void Context::RemoveInstance(InstanceData& instance) {
		auto& pointcloudData = *resource->pointcloudData;

		resources::ManagedCArray<InstanceData, unsigned int> instances{ resource, pointcloudData.instances, pointcloudData.instanceCount };

		instances.remove(&instance - pointcloudData.instances);
	}

	void Context::TransformUpdate(InstanceData& instance, const Eigen::Affine3f& transform) {
		// TODO: Split this off, possibly via making child classes of this editor
		if (pcType == app::gfx::ResPointcloudModel::GetTypeInfo()) {
			if (auto* terrainGround = hh::game::GameManager::GetInstance()->GetGameObject("TerrainGround")) {
				char name[0x80];
				snprintf(name, sizeof(name), "%s.%p", instance.name, resource);
				if (auto* gocVisualModel = terrainGround->GetComponent<hh::gfx::GOCVisualModel>(name_hash(name)))
					gocVisualModel->SetLocalTransform(transform);
				else {
					snprintf(name, sizeof(name), "%s.%p", instance.resourceName, resource);
					if (auto* gocVisualModel = terrainGround->GetComponent<hh::gfx::GOCVisualModel>(name_hash(name)))
						gocVisualModel->SetLocalTransform(transform);
				}
			}
		}
	}
}
