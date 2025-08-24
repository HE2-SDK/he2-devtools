#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/BoundingBoxes.h>
#include <algorithm>

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
}
