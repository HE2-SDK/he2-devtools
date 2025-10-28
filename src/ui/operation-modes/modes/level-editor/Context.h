#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::level_editor {
	class Context : public CompatibleObject {
#ifdef DEVTOOLS_TARGET_SDK_wars
		std::mt19937 mt{ std::random_device{}() };
#else
		std::mt19937_64 mt{ std::random_device{}() };
#endif
		hh::game::ObjectWorldChunk* focusedChunk{};

		hh::game::ObjectId GenerateRandomObjectId();
		hh::game::ObjectId GenerateUniqueRandomObjectId();
		hh::game::ObjectData* CreateObject(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* objectClass, hh::game::ObjectTransformData transform, hh::game::ObjectData* parentObject = nullptr);
		hh::game::ObjectData* CopyObject(csl::fnd::IAllocator* allocator, hh::game::ObjectData* otherObject);
	public:
		hh::game::ObjectWorldChunkLayer* placementTargetLayer{};
		const hh::game::GameObjectClass* objectClassToPlace{};

		static bool setEditorStatus;

		using CompatibleObject::CompatibleObject;

		~Context();

		hh::game::ObjectData* CopyObjectForPlacement(hh::game::ObjectData* otherObject);
		hh::game::ObjectData* CopyObjectForClipboard(hh::game::ObjectData* otherObject);
		void DeleteObjects(const csl::ut::MoveArray<hh::game::ObjectData*>& objects);
		void TerminateClipboardObject(hh::game::ObjectData* objectData);

		hh::game::ObjectWorldChunk* GetFocusedChunk() const;
		void SetFocusedChunk(hh::game::ObjectWorldChunk* chunk);
		void ReleaseChunk();

		void SetLayerEnabled(const char* layerName, bool enabled);

		hh::game::ObjectData* SpawnObject(const csl::math::Vector3& position);
		void SpawnObject(hh::game::ObjectData* objData);
		void ResetPlacementState();

		void RecalculateDependentTransforms(hh::game::ObjectData* objectData);
		void RecalculateDependentTransforms(const csl::ut::MoveArray<hh::game::ObjectData*>& objectDatas);

		void NotifySelectedObject(hh::game::ObjectData* objectData);
		void NotifyUpdatedObject(hh::game::ObjectData* objectData);
		void NotifyDeselectedObject(hh::game::ObjectData* objectData);
		void NotifyParamChanged(hh::game::ObjectData* objectData);
		void NotifyObject(hh::game::ObjectData* objectData, hh::fnd::Message& msg);

		void ReloadActiveObjectParameters(hh::game::ObjectData* objectData);
		void RespawnActiveObject(hh::game::ObjectData* objectData);

		void UpdatePaths();

		void SetObjectParent(hh::game::ObjectData* child, hh::game::ObjectData* parent);

		void SetObjectClassToPlace(const hh::game::GameObjectClass* gameObjectClass);
	};
}
