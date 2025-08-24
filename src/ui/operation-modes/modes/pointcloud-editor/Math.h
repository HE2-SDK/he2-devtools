#pragma once

namespace ui::operation_modes::modes::pointcloud_editor {
	void UpdateInstanceTransform(ucsl::resources::pointcloud::v2::InstanceData& instance, const Eigen::Affine3f& transform);
	Eigen::Affine3f GetInstanceTransform(const ucsl::resources::pointcloud::v2::InstanceData& instance);
}
