#pragma once
#include <utilities/CompatibleObject.h>
#include <utilities/ResourceTypes.h>
#include <ucsl/resources/pointcloud/v2.h>

namespace ui::operation_modes::modes::pointcloud_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::gfx::ResPointcloud> resource{};
		const hh::fnd::ResourceTypeInfo* pcType;

		ucsl::resources::pointcloud::v2::InstanceData* AddInstance(const Eigen::Vector3f& position);
		void RemoveInstance(ucsl::resources::pointcloud::v2::InstanceData& instance);
	};
}
