#include "ObjectData.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/viewers/ObjectData.h>
#include <utilities/ObjectDataUtils.h>
#include "ObjectData.h"

using namespace hh::game;

#ifndef DEVTOOLS_TARGET_SDK_wars
ucsl::resources::object_world::v3::ComponentData* CreateComponentData(csl::fnd::IAllocator* allocator, const GOComponentRegistry::GOComponentRegistryItem* gocRegItem) {
	if (!strcmp(gocRegItem->GetName(), "Model")) return CreateComponentDataV3<hh::gfx::GOCVisualModelSpawner>(allocator, "Model");
	else if (!strcmp(gocRegItem->GetName(), "SimpleAnimation")) return CreateComponentDataV3<hh::anim::GOCSimpleAnimationSpawner>(allocator, "SimpleAnimation");
	else if (!strcmp(gocRegItem->GetName(), "RangeSpawning")) return CreateComponentDataV3<GOCActivatorSpawner>(allocator, "RangeSpawning");
	else CreateComponentDataV3(allocator, gocRegItem);
}
#endif

bool Editor(const char* label, hh::game::ObjectTransformData& obj)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", obj.position);
	edited |= EulerEditor("Rotation", obj.rotation);
	ImGui::EndGroup();
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::game::ObjectData& obj, hh::game::ResObjectWorld* objectWorld)
{
	bool edited{};
	auto* objSystem = GameObjectSystem::GetInstance();

	Viewer("Id", obj.id);
#ifdef DEVTOOLS_TARGET_SDK_wars
	if (objectWorld)
		edited |= InputText("Name", obj.name, objectWorld);
	else
		Viewer("Name", obj.name);
#else
	edited |= Editor("Name", obj.name);
#endif
	ImGui::Text("Class: %s", obj.gameObjectClass);
	Viewer("Parent object id", obj.parentID);

	ImGui::SeparatorText("Transform");
	Viewer("Transform", obj.transform);

	ObjectTransformData localTransformData{ obj.localTransform };

	ImGui::SeparatorText("Local transform");
	bool editedTransform = Editor("Local transform", localTransformData);
	edited |= editedTransform;

	if (editedTransform)
		UpdateLocalTransform(ObjectTransformDataToAffine3f(localTransformData), obj);

	ImGui::SeparatorText("Components");
#ifdef DEVTOOLS_TARGET_SDK_wars
	for (auto* componentConfig : obj.componentData) {
		if (ImGui::CollapsingHeader(componentConfig->type)) {
			ImGui::Text("Configuration:");
			if (!strcmp(componentConfig->type, "RangeSpawning"))
				edited |= ReflectionEditor("Component properties", componentConfig->data, &GOCActivatorSpawner::rflClass, true);
		}
	}
#else
	for (auto it = obj.componentData.begin(); it != obj.componentData.end(); it++) {
		auto* componentData = *it;

		ImGui::PushID(componentData);
		auto* gocInfo = objSystem->goComponentRegistry->GetComponentInformationByName(componentData->type);
		if (ImGui::CollapsingHeader(gocInfo->GetName())) {
			ImGui::Text("Component type: %s", gocInfo->GetComponentClass()->category);
			ImGui::Text("Configuration:");
			edited |= ReflectionEditor("Component properties", componentData->data, gocInfo->GetSpawnerDataClass(), true);

			if (ImGui::Button("Remove component")) {
				auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();

				//if (!obj.flags.test(ObjectData::Flag::DEALLOCATE)) {
				//	csl::ut::MoveArray<ComponentData*> componentDatas{ allocator };

				//	for (auto* origCData : obj.componentData) {
				//		auto* gocRegItem = GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(origCData->type);
				//		auto* componentData = CreateComponentData(allocator, gocRegItem);
				//		rflops::Copy::Apply(componentData->data, origCData->data, gocRegItem->rflClass);
				//		componentDatas.push_back(componentData);
				//	}

				//	obj.componentData = std::move(componentDatas);
				//	obj.flags.set(ObjectData::Flag::DEALLOCATE);
				//}

				if (obj.flags.test(ObjectData::Flag::DEALLOCATE))
					allocator->Free(componentData);
				obj.componentData.erase(it);
				edited = true;
			}
		}
		ImGui::PopID();
	}

	ImGui::Separator();
	if (ImGui::Button("Add component..."))
		ImGui::OpenPopup("ComponentConfigSelection");

	if (ImGui::BeginPopup("ComponentConfigSelection")) {
		for (auto* gocRegItem : objSystem->goComponentRegistry->GetComponents()) {
			if (ImGui::Selectable(gocRegItem->GetName())) {
				auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();

				//if (!obj.flags.test(ObjectData::Flag::DEALLOCATE)) {
				//	csl::ut::MoveArray<ComponentData*> componentDatas{ allocator };

				//	for (auto* origCData : obj.componentData) {
				//		auto* gocRegItem = GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(origCData->type);
				//		auto* componentData = CreateComponentData(allocator, gocRegItem);
				//		rflops::Copy::Apply(componentData->data, origCData->data, gocRegItem->rflClass);
				//		componentDatas.push_back(componentData);
				//	}

				//	obj.componentData = std::move(componentDatas);
				//	obj.flags.set(ObjectData::Flag::DEALLOCATE);
				//}

				// TODO: Currently leaks some memory, because we would have to copy the whole object when doing that and currently we are not.
				if (!obj.flags.test(ObjectData::Flag::DEALLOCATE))
					obj.componentData.change_allocator(allocator);
				obj.componentData.push_back(CreateComponentData(allocator, gocRegItem));
				edited = true;
			}
		}
		ImGui::EndPopup();
	}
#endif

	if (obj.spawnerData) {
		auto* objClass = objSystem->gameObjectRegistry->GetGameObjectClassByName(obj.gameObjectClass);

		ImGui::SeparatorText("Object properties");
		edited |= ReflectionEditor("Object properties", obj.spawnerData, objClass->GetSpawnerDataClass(), true);
	}

	return edited;
}
