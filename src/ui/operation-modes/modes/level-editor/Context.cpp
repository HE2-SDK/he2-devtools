#include "Context.h"
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
#define CreateComponentData CreateComponentDataV2
#define CreateObjectData CreateObjectDataV2
#define CopyObjectData CopyObjectDataV2
#else
#define CreateComponentData CreateComponentDataV3
#define CreateObjectData CreateObjectDataV3
#define CopyObjectData CopyObjectDataV3
#endif

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	bool Context::setEditorStatus{ true };

	Context::~Context() {
		SetFocusedChunk(nullptr);
	}

	ObjectId Context::GenerateRandomObjectId() {
#ifdef DEVTOOLS_TARGET_SDK_wars
		return { mt() };
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
		return { mt(), mt() };
#endif
	}

	// This is extremely unlikely to happen with V3 object IDs and I would probably not even bother if we were only handling those,
	// but it is less unlikely to happen with V2 object IDs (though still unlikely). 
	ObjectId Context::GenerateUniqueRandomObjectId() {
		ObjectId result{};
		boolean isUnique{ true };

		// This has the potential to infinitely loop if the user has 2^32 objects in one chunk (V2).
		// It also has the potential to take a long time in collisions if there are less but still an extreme amount of
		// object IDs on the map.
		// I'm currently ignoring this possibility, but the second one could be avoided by only rolling once and then simply incrementing until we find a gap.
		// I just prefer a more random distribution, and we'll see if issues happen in practice. (Why have tests when you have end users :) )
		do {
			isUnique = true;
			result = GenerateRandomObjectId();

			for (auto& status : focusedChunk->GetObjectStatuses())
				if (status.objectData->id == result)
					isUnique = false;
		} while (!result.IsNonNull() || !isUnique);

		return result;
	}

	void Context::RecalculateDependentTransforms(const csl::ut::MoveArray<ObjectData*>& objectDatas) {
		for (auto* obj : objectDatas)
			RecalculateDependentTransforms(obj);
	}

	void Context::NotifySelectedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgStartActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyUpdatedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgUpdateActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);

	}

	void Context::NotifyDeselectedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgFinishActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyParamChanged(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgParamChangedInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyObject(hh::game::ObjectData* objectData, hh::fnd::Message& msg)
	{
		if (auto* obj = focusedChunk->GetGameObject(objectData))
			obj->SendMessageImm(msg);
	}

	void Context::UpdatePaths()
	{
		// Live update grind rails
		for (auto* obj : GameManager::GetInstance()->objects) {
			if (obj->GetComponent<hh::path::PathComponent>()) {
				hh::dbg::MsgUpdateActiveObjectInEditor msg;
				obj->SendMessageImm(msg);
			}
			
#ifndef DEVTOOLS_TARGET_SDK_wars
			if (auto* grind = obj->GetComponent<app::game::GOCGrind>())
				grind->updater->flags.set(app::game::GrindModelUpdater::Flag::UPDATE);
#endif
		}
	}

	void Context::ReloadActiveObjectParameters(hh::game::ObjectData* objectData)
	{
		NotifyParamChanged(objectData);

		// The only reason I do this is because apparently sending MsgParamChanged sets the debug visual visibility to false on volumes??
		NotifySelectedObject(objectData);
		NotifyUpdatedObject(objectData);
	}

	// This is a hack to force the object to reload its parameters
	void Context::RespawnActiveObject(hh::game::ObjectData* objectData)
	{
		focusedChunk->Despawn(objectData);
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Spawn(objectData);
		NotifySelectedObject(objectData);
	}

	void Context::RecalculateDependentTransforms(hh::game::ObjectData* objectData) {
		// Copy changes to the live object if any.
		if (auto* obj = focusedChunk->GetGameObject(objectData))
			if (auto* gocTransform = obj->GetComponent<GOCTransform>())
				UpdateGOCTransform(*objectData, *gocTransform);

		// Some tools generate faulty objects with ID 0. Avoid crashing on those.
		if (!objectData->id.IsNonNull())
			return;

		for (auto* layer : focusedChunk->GetLayers()) {
			for (auto* child : layer->GetResource()->GetObjects()) {
				if (child->parentID == objectData->id) {
					RecalculateAbsoluteTransform(*objectData, *child);
					RecalculateDependentTransforms(child);
				}
			}
		}
	}

	ObjectData* Context::CreateObject(csl::fnd::IAllocator* allocator, const GameObjectClass* gameObjectClass, ObjectTransformData transform, ObjectData* parentObject) {
#ifdef DEVTOOLS_TARGET_SDK_wars
		// FIXME: We're leaking memory here because the string does not get freed.
		auto name = new char[100];
		snprintf(name, 100, "%s_%d", gameObjectClass->GetName(), mt() % 0x1000000);
#else
		char name[100];
		snprintf(name, 100, "%s_%zd", gameObjectClass->GetName(), mt() % 0x1000000);
#endif

		auto* objData = CreateObjectData(
			allocator,
			gameObjectClass,
			GenerateUniqueRandomObjectId(),
			name,
			parentObject,
			transform
		);

		auto* rangeSpawning = CreateComponentData<GOCActivatorSpawner>(allocator, "RangeSpawning");
		auto* rangeSpawningData = static_cast<GOCActivatorSpawner*>(rangeSpawning->data);

		rangeSpawningData->m_range = 500.0f;
		rangeSpawningData->m_distance = 200.0f;

		objData->componentData.push_back(rangeSpawning);

		return (ObjectData*)objData;
	}

	ObjectData* Context::CopyObject(csl::fnd::IAllocator* allocator, ObjectData* otherObject) {
#ifdef DEVTOOLS_TARGET_SDK_wars
		// FIXME: We're leaking memory here because the string does not get freed.
		auto name = new char[100];
		snprintf(name, 100, "%s_%d", otherObject->gameObjectClass, mt() % 0x1000000);
#else
		char name[100];
		snprintf(name, 100, "%s_%zd", otherObject->gameObjectClass, mt() % 0x1000000);
#endif

		auto* objData = CopyObjectData(
			allocator,
			GenerateUniqueRandomObjectId(),
			name,
			otherObject
		);

		auto* otherRangeSpawning = *std::find_if(otherObject->componentData.begin(), otherObject->componentData.end(), [](const auto* c) { return !strcmp(c->type, "RangeSpawning"); });
		auto* newRangeSpawning = CreateComponentData<GOCActivatorSpawner>(allocator, "RangeSpawning");

		auto* newRangeSpawningData = static_cast<GOCActivatorSpawner*>(newRangeSpawning->data);
		
		if (otherRangeSpawning) {
			auto* otherRangeSpawningData = static_cast<GOCActivatorSpawner*>(otherRangeSpawning->data);
			*newRangeSpawningData = *otherRangeSpawningData;
		}
		else {
			newRangeSpawningData->m_range = 500.0f;
			newRangeSpawningData->m_distance = 200.0f;
		}

		objData->componentData.push_back(newRangeSpawning);

		return (ObjectData*)objData;
	}

	hh::game::ObjectData* Context::CopyObjectForPlacement(hh::game::ObjectData* otherObject)
	{
		return CopyObject(placementTargetLayer->GetAllocator(), otherObject);
	}

	hh::game::ObjectData* Context::CopyObjectForClipboard(hh::game::ObjectData* otherObject)
	{
		return CopyObject(GetAllocator(), otherObject);
	}

	void Context::DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects)
	{
		for (auto* obj : objects)
			focusedChunk->Despawn(obj);

		focusedChunk->ShutdownPendingObjects();

		for (auto* obj : objects)
			for (auto* layer : focusedChunk->GetLayers())
				for (auto* object : layer->GetResource()->GetObjects())
					if (object == obj)
						layer->RemoveObjectData(obj);
	}

	void Context::TerminateClipboardObject(hh::game::ObjectData* objectData)
	{
		TerminateObjectData(GetAllocator(), objectData);
	}

	hh::game::ObjectData* Context::SpawnObject(const csl::math::Vector3& position) {
		auto* result = CreateObject(placementTargetLayer->GetAllocator(), objectClassToPlace, { { position, { 0.0f, 0.0f, 0.0f } } }, nullptr);
		SpawnObject(result);
		return result;
	}

	void Context::SpawnObject(ObjectData* objData) {
		auto* resource = placementTargetLayer->GetResource();

		if (auto* objInfoName = static_cast<const char*>(hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(objData->gameObjectClass)->GetAttributeValue("objinfo"))) {
			auto* objInfoContainer = GameManager::GetInstance()->GetService<ObjInfoContainer>();
			auto* objInfoClass = RESOLVE_STATIC_VARIABLE(ObjInfoRegistry::instance)->objInfosByName.GetValueOrFallback(objInfoName, nullptr);
			auto* objInfo = objInfoClass->instantiator(GetAllocator());

			objInfoContainer->Register(objInfo->GetInfoName(), objInfo);
		}

		resource->AddObject(objData);
#ifdef DEVTOOLS_TARGET_SDK_miller
		focusedChunk->AddWorldObjectStatus(placementTargetLayer, objData, true);
#else
		focusedChunk->AddWorldObjectStatus(objData, true);
#endif
	}

	hh::game::ObjectWorldChunk* Context::GetFocusedChunk() const
	{
		return focusedChunk;
	}

	void Context::SetFocusedChunk(hh::game::ObjectWorldChunk* chunk)
	{
		if (focusedChunk) {
			focusedChunk->DespawnAll();
			focusedChunk->ShutdownPendingObjects();
			focusedChunk->SetEditorStatus(false);
			focusedChunk->SpawnByAttribute(GetStatusEternal);
		}
		focusedChunk = chunk;
		if (focusedChunk) {
			focusedChunk->SetEditorStatus(setEditorStatus);
			focusedChunk->DespawnAll();
			focusedChunk->ShutdownPendingObjects();
			focusedChunk->Restart(true);
			focusedChunk->SpawnByAttribute(GetStatusEternal);
		}
		ResetPlacementState();
	}

	void Context::ReleaseChunk()
	{
		focusedChunk = nullptr;
		ResetPlacementState();
	}

	void Context::SetLayerEnabled(const char* layerName, bool enabled)
	{
		focusedChunk->SetLayerEnabled(layerName, enabled);
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Restart(true);
		focusedChunk->SpawnByAttribute(GetStatusEternal);
	}

	void Context::ResetPlacementState()
	{
		placementTargetLayer = nullptr;
		objectClassToPlace = nullptr;
	}

	void Context::SetObjectParent(hh::game::ObjectData* child, hh::game::ObjectData* parent)
	{
		if (parent) {
			auto parentAbsoluteTransform = ObjectTransformDataToAffine3f(parent->transform);
			auto childAbsoluteTransform = ObjectTransformDataToAffine3f(child->transform);

			child->localTransform = Affine3fToObjectTransformData(parentAbsoluteTransform.inverse() * childAbsoluteTransform);
			child->parentID = parent->id;
		}
		else {
			hh::game::ObjectData* currentParent{ nullptr };

			for (auto* layer : focusedChunk->GetLayers()) {
				for (auto* object : layer->GetResource()->GetObjects()) {
					if (child->parentID == object->id) {
						currentParent = object;
						break;
					}
				}
			}

			if (currentParent) {
				auto parentAbsoluteTransform = ObjectTransformDataToAffine3f(currentParent->transform);
				auto childAbsoluteTransform = ObjectTransformDataToAffine3f(child->transform);

				child->localTransform = Affine3fToObjectTransformData(parentAbsoluteTransform * childAbsoluteTransform);
			}
			
			child->parentID = {};
		}
		focusedChunk->Despawn(child);
		focusedChunk->Restart(focusedChunk->GetObjectIndexByObjectData(child), true);
	}

	void Context::SetObjectClassToPlace(const hh::game::GameObjectClass* gameObjectClass) {
		objectClassToPlace = gameObjectClass;
	}
}
