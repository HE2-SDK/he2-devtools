#include "Pointcloud.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <utilities/NameHash.h>

#ifndef DEVTOOLS_TARGET_SDK_wars
const char* rotationOrderNames[]{ "NONE", "XYZ", "YZX", "ZXY", "XZY", "YXZ", "ZYX" };

bool Editor(const char* label, ucsl::resources::pointcloud::v2::InstanceData& instance, hh::gfx::ResPointcloud* resource) {
	bool edited{}; 
	if (resource->resourceTypeInfo == app::gfx::ResPointcloudModel::GetTypeInfo()) {
		char ogName[0x200];
		strcpy(ogName, instance.name);
		if (edited |= InputText("Name", instance.name, resource)) {
			if (auto* terrainGround = hh::game::GameManager::GetInstance()->GetGameObject("TerrainGround")) {
				char nameHash[0x250];
				snprintf(nameHash, sizeof(nameHash), "%s.%p", ogName, resource);
				if (auto* gocVisualModel = terrainGround->GetComponent<hh::gfx::GOCVisualModel>(name_hash(nameHash))) {
					snprintf(nameHash, sizeof(nameHash), "%s.%p", instance.name, resource);
					gocVisualModel->SetNameHash(nameHash);
				}
			}
		}
	}
	else
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
