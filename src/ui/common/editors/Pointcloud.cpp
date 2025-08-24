#include "Pointcloud.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>

#ifndef DEVTOOLS_TARGET_SDK_wars
const char* rotationOrderNames[]{ "NONE", "XYZ", "YZX", "ZXY", "XZY", "YXZ", "ZYX" };

bool Editor(const char* label, ucsl::resources::pointcloud::v2::InstanceData& instance, hh::gfx::ResPointcloud* resource) {
	bool edited{};
	edited |= InputText("Name", instance.name, resource);
	edited |= InputText("Resource name", instance.resourceName, resource);
	edited |= Editor("Position", instance.position);
	edited |= EulerEditor("Rotation", instance.rotation);
	edited |= ComboEnum("Rotation order", instance.rotationOrder, rotationOrderNames);
	edited |= Editor("Scale", instance.scale);
	//edited |= Editor("Unk2", instance.unk2);
	return edited;
}
#endif
