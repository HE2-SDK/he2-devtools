#include "ObjectWorld.h"
#include "Common.h"
#include <ucsl/resources/object-world/v2.h>
#include <ucsl/resources/object-world/v3.h>
#include <rip/serialization/rfl/any.h>

namespace devtools::api::object_world {
	struct ObjectWorldChunk {
		size_t id{};
	};

	struct ObjectWorldChunkLayer {
		std::string id{};
		bool enabled{};
	};

	struct ComponentData {
		std::string type{};
		rfl::Generic parameters{};
	};

	struct ObjectData {
		std::string id{};
		std::string parentId{};
		std::string name{};
		std::string type{};
		Vector3 position{};
		Vector3 rotation{};
		std::vector<ComponentData> components{};
		rfl::Generic parameters{};
	};

	hh::game::ObjectWorld* getObjectWorld() {
		auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

		if (!objWorld) {
			throw ErrorResponse{
				.status = "400 Bad Request",
				.message = "ObjectWorld service is not active",
			};
		}

		return objWorld;
	}

	hh::game::ObjectWorldChunk* getObjectWorldChunk(hh::game::ObjectWorld* objWorld, size_t chunkId) {
		if (chunkId > objWorld->GetWorldChunks().size()) {
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "chunkId: invalid index",
			};
		}

		return objWorld->GetWorldChunks()[chunkId];
	}

	hh::game::ObjectWorldChunkLayer* getObjectWorldChunkLayer(hh::game::ObjectWorldChunk* chunk, const std::string& layerId) {
		auto* layer = chunk->GetLayerByName(layerId.c_str());

		if (!layer) {
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "layerId: invalid id",
			};
		}

		return layer;
	}

	hh::game::ObjectData* getObjectData(hh::game::ObjectWorldChunkLayer* layer, const std::string& objectId) {
		auto objId = rip::util::fromGUID<hh::game::ObjectId>(objectId.c_str());

		auto& objects = layer->GetResource()->GetObjects();
		auto object_it = std::find_if(objects.begin(), objects.end(), [objId](auto* obj) { return hh::game::ObjectId{ obj->id } == objId; });

		if (object_it == objects.end()) {
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "objectId: invalid id",
			};
		}

		return *object_it;
	}

	hh::game::ObjectData* getObjectData(hh::game::ObjectWorldChunk* chunk, const std::string& objectId) {
		auto objId = rip::util::fromGUID<hh::game::ObjectId>(objectId.c_str());
		auto objectDataAccessor = chunk->GetObjectDataByObjectId(rip::util::fromGUID<hh::game::ObjectId>(objectId.c_str()));

		if (!objectDataAccessor.objectData) {
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "objectId: invalid id",
			};
		}

		return objectDataAccessor.objectData;
	}

	ObjectWorldChunk buildChunk(size_t idx) {
		return {
			.id = idx,
		};
	}

	ObjectWorldChunkLayer buildLayer(hh::game::ObjectWorldChunkLayer* layer) {
		return {
			.id = layer->GetName(),
			.enabled = layer->IsEnable(),
		};
	}

	ComponentData buildComponentData(ucsl::resources::object_world::v2::ComponentData* component) {
		return {
			.type = component->type,
			.parameters = getRflClassSerialization(component->data, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(component->type)->GetSpawnerDataClass()),
		};
	}

	ComponentData buildComponentData(ucsl::resources::object_world::v3::ComponentData* component) {
		return {
			.type = component->type,
			.parameters = getRflClassSerialization(component->data, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(component->type)->GetSpawnerDataClass()),
		};
	}

	ObjectData buildObjectData(hh::game::ObjectData* object) {
		std::vector<ComponentData> components{};

		for (auto* component : object->componentData)
			components.push_back(buildComponentData(component));

		return {
			.id = rip::util::toGUID(object->id),
			.parentId = rip::util::toGUID(object->parentID),
#ifdef DEVTOOLS_TARGET_SDK_wars
			.name = object->name,
#else
			.name = object->name.c_str(),
#endif
			.type = object->gameObjectClass,
			.position = { object->localTransform.position.x(), object->localTransform.position.y(), object->localTransform.position.z() },
			.rotation = { object->localTransform.rotation.x(), object->localTransform.rotation.y(), object->localTransform.rotation.z() },
			.components = components,
			.parameters = getRflClassSerialization(object->spawnerData, hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(object->gameObjectClass)->GetSpawnerDataClass()),
		};
	}

	void RegisterRoutes(APIContext& ctx) {
		ctx.app->get("/object-world/chunks", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, []() {
				auto* objWorld = getObjectWorld();

				std::vector<ObjectWorldChunk> results{};

				for (size_t i = 0; i < objWorld->GetWorldChunks().size(); i++)
					results.push_back(buildChunk(i));

				return results;
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId")]() {
				auto* objWorld = getObjectWorld();
				getObjectWorldChunk(objWorld, chunkId);

				return buildChunk(chunkId);
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId/layers", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId"), layerId = ctx.getParameter<std::string>(req, "layerId")]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);

				std::vector<ObjectWorldChunkLayer> results{};

				for (auto* layer : chunk->GetLayers())
					results.push_back(buildLayer(layer));

				return results;
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId/layers/:layerId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId"), layerId = ctx.getParameter<std::string>(req, "layerId")]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);

				return buildLayer(layer);
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId/layers/:layerId/objects", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId"), layerId = ctx.getParameter<std::string>(req, "layerId")]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);

				std::vector<ObjectData> results{};

				for (auto* object : layer->GetResource()->GetObjects())
					results.push_back(buildObjectData(object));

				return results;
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId/layers/:layerId/objects/:objectId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId"), layerId = ctx.getParameter<std::string>(req, "layerId"), objectId = ctx.getParameter<std::string>(req, "objectId")]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);
				auto* object = getObjectData(layer, objectId);

				return buildObjectData(object);
			});
		});
		ctx.app->get("/object-world/chunks/:chunkId/objects/:objectId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [chunkId = ctx.getParameter<size_t>(req, "chunkId"), objectId = ctx.getParameter<std::string>(req, "objectId")]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* object = getObjectData(chunk, objectId);

				return buildObjectData(object);
			});
		});
	}
}