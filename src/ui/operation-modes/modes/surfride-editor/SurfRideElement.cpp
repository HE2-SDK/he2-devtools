#include "SurfRideElement.h"
#include <span>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::swif_version;

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SurfRide::Scene* runtimeScene)
		: type{ Type::SCENE }, path{ BuildScenePath(scene) }, runtimePath{ runtimeScene == nullptr ? std::nullopt : std::make_optional(BuildRuntimeScenePath(*runtimeScene)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_CAMERA& camera, const SurfRide::Camera* runtimeCamera)
		: type{ Type::CAMERA }, path{ BuildCameraPath(scene, camera) }, runtimePath{ runtimeCamera == nullptr ? std::nullopt : std::make_optional(BuildRuntimeCameraPath(*runtimeCamera)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_LAYER& layer, const SurfRide::Layer* runtimeLayer)
		: type{ Type::LAYER }, path{ BuildLayerPath(scene, layer) }, runtimePath{ runtimeLayer == nullptr ? std::nullopt : std::make_optional(BuildRuntimeLayerPath(*runtimeLayer)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_LAYER& layer, const SRS_CASTNODE& cast, const SurfRide::Cast* runtimeCast)
		: type{ Type::CAST }, path{ BuildCastPath(scene, layer, cast) }, runtimePath{ runtimeCast == nullptr ? std::nullopt : std::make_optional(BuildRuntimeCastPath(*runtimeCast)) } {}

	SurfRideElement::SurfRideElement(const SurfRide::Scene& runtimeScene)
		: SurfRideElement{ *runtimeScene.sceneData, &runtimeScene } {}

	SurfRideElement::SurfRideElement(const SurfRide::Layer& runtimeLayer)
		: SurfRideElement{ *runtimeLayer.scene->sceneData, *runtimeLayer.layerData, &runtimeLayer } {}

	SurfRideElement::SurfRideElement(const SurfRide::Cast& runtimeCast)
		: SurfRideElement{ *runtimeCast.layer->scene->sceneData, *runtimeCast.layer->layerData, *runtimeCast.castData, &runtimeCast } {}

	bool SurfRideElement::operator==(const SurfRideElement& other) const {
		return type == other.type && path == other.path && runtimePath == other.runtimePath;
	}

	SRS_SCENE* ResolveScenePathFragment(SRS_PROJECT& project, const SurfRideElementPathFragment& path) {
		for (auto& scene : std::span(project.scenes, project.sceneCount))
			if (path == scene.id)
				return &scene;

		return nullptr;
	}

	SRS_CAMERA* ResolveCameraPathFragment(SRS_SCENE& scene, const SurfRideElementPathFragment& path) {
		for (auto& camera : std::span(scene.cameras, scene.cameraCount))
			if (path == camera.id)
				return &camera;

		return nullptr;
	}

	SRS_LAYER* ResolveLayerPathFragment(SRS_SCENE& scene, const SurfRideElementPathFragment& path) {
		for (auto& layer : std::span(scene.layers, scene.layerCount))
			if (path == layer.id)
				return &layer;

		return nullptr;
	}

	SRS_CASTNODE* ResolveCastPathFragment(SRS_LAYER& layer, const SurfRideElementPathFragment& path) {
		for (auto& cast : std::span(layer.casts, layer.castCount))
			if (path == cast.id)
				return &cast;

		return nullptr;
	}

	SurfRide::Scene* ResolveRuntimeScenePathFragment(SurfRide::Project& project, const SurfRideElementPathFragment& path) {
		for (auto scene : project.GetScenes())
			if (scene->sceneData->id == path)
				return &*scene;

		return nullptr;
	}

	SurfRide::Camera* ResolveRuntimeCameraPathFragment(SurfRide::Scene& scene, const SurfRideElementPathFragment& path) {
		return nullptr;
	}

	SurfRide::Layer* ResolveRuntimeLayerPathFragment(SurfRide::Scene& scene, const SurfRideElementPathFragment& path) {
		return scene.GetLayer(path);
	}

	SurfRide::Layer* ResolveRuntimeLayerPathFragment(SurfRide::ReferenceCast& cast, const SurfRideElementPathFragment& path) {
		return path == cast.refLayer->layerData->id ? cast.refLayer : nullptr;
	}

	SurfRide::Cast* ResolveRuntimeCastPathFragment(SurfRide::Layer& layer, const SurfRideElementPathFragment& path) {
		return layer.GetCast(path);
	}

	SurfRideElementPathFragment BuildScenePathFragment(const SRS_SCENE& scene) {
		return scene.id;
	}

	SurfRideElementPathFragment BuildCameraPathFragment(const SRS_CAMERA& camera) {
		return camera.id;
	}

	SurfRideElementPathFragment BuildLayerPathFragment(const SRS_LAYER& layer) {
		return layer.id;
	}

	SurfRideElementPathFragment BuildCastPathFragment(const SRS_CASTNODE& cast) {
		return cast.id;
	}

	SurfRideElementPathFragment BuildRuntimeScenePathFragment(const SurfRide::Scene& scene) {
		return scene.sceneData->id;
	}

	SurfRideElementPathFragment BuildRuntimeCameraPathFragment(const SurfRide::Camera& camera) {
		return const_cast<SurfRide::Camera&>(camera).GetCameraData().id;
	}

	SurfRideElementPathFragment BuildRuntimeLayerPathFragment(const SurfRide::Layer& layer) {
		return layer.layerData->id;
	}

	SurfRideElementPathFragment BuildRuntimeCastPathFragment(const SurfRide::Cast& cast) {
		return cast.castData->id;
	}

	SRS_SCENE* ResolveScenePath(SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 1)
			return nullptr;

		return ResolveScenePathFragment(project, path[0]);
	}

	SRS_CAMERA* ResolveCameraPath(SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);

		return scene == nullptr ? nullptr : ResolveCameraPathFragment(*scene, path[1]);
	}

	SRS_LAYER* ResolveLayerPath(SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);

		return scene == nullptr ? nullptr : ResolveLayerPathFragment(*scene, path[1]);
	}

	SRS_CASTNODE* ResolveCastPath(SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 3)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);
		auto* layer = ResolveLayerPathFragment(*scene, path[1]);

		return layer == nullptr ? nullptr : ResolveCastPathFragment(*layer, path[2]);
	}

	SurfRide::Scene* ResolveRuntimeScenePath(SurfRide::Project& project, const SurfRideElementPath& path) {
		if (path.size() != 1)
			return nullptr;

		return ResolveRuntimeScenePathFragment(project, path[0]);
	}

	SurfRide::Camera* ResolveRuntimeCameraPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[0]);

		return scene == nullptr ? nullptr : ResolveRuntimeCameraPathFragment(*scene, path[1]);
	}

	SurfRide::Layer* ResolveRuntimeLayerPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		size_t i{};
		if (path.size() < 2)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[i++]);
		auto* layer = scene == nullptr ? nullptr : ResolveRuntimeLayerPathFragment(*scene, path[i++]);

		while (layer != nullptr && i < path.size()) {
			auto* cast = ResolveRuntimeCastPathFragment(*layer, path[i++]);

			if (cast == nullptr || i >= path.size() || cast->castData->GetType() != SRS_CASTNODE::Type::REFERENCE)
				return nullptr;

			layer = ResolveRuntimeLayerPathFragment(*static_cast<SurfRide::ReferenceCast*>(cast), path[i++]);
		}

		return layer;
	}

	SurfRide::Cast* ResolveRuntimeCastPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		size_t i{};
		if (path.size() < 3)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[i++]);
		auto* layer = scene == nullptr ? nullptr : ResolveRuntimeLayerPathFragment(*scene, path[i++]);
		auto* cast = layer == nullptr ? nullptr : ResolveRuntimeCastPathFragment(*layer, path[i++]);

		while (cast != nullptr && i < path.size()) {
			if (cast->castData->GetType() != SRS_CASTNODE::Type::REFERENCE)
				return nullptr;

			layer = ResolveRuntimeLayerPathFragment(*static_cast<SurfRide::ReferenceCast*>(cast), path[i++]);

			if (layer == nullptr || i >= path.size())
				return nullptr;

			cast = ResolveRuntimeCastPathFragment(*layer, path[i++]);
		}

		return cast;
	}

	SurfRideElementPath BuildScenePath(const SRS_SCENE& scene) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		return res;
	}

	SurfRideElementPath BuildCameraPath(const SRS_SCENE& scene, const SRS_CAMERA& camera) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildCameraPathFragment(camera));
		return res;
	}

	SurfRideElementPath BuildLayerPath(const SRS_SCENE& scene, const SRS_LAYER& layer) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildLayerPathFragment(layer));
		return res;
	}

	SurfRideElementPath BuildCastPath(const SRS_SCENE& scene, const SRS_LAYER& layer, const SRS_CASTNODE& cast) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildLayerPathFragment(layer));
		res.push_back(BuildCastPathFragment(cast));
		return res;
	}

	void AddScene(const SurfRide::Scene& scene, SurfRideElementPath& path) {
		path.push_back(BuildRuntimeScenePathFragment(scene));
	}

	void AddLayer(const SurfRide::Layer& layer, SurfRideElementPath& path);
	void AddCast(const SurfRide::Cast& cast, SurfRideElementPath& path) {
		AddLayer(*cast.layer, path);
		path.push_back(BuildRuntimeCastPathFragment(cast));
	}

	void AddLayer(const SurfRide::Layer& layer, SurfRideElementPath& path) {
		if (layer.referenceCast)
			AddCast(*layer.referenceCast, path);
		else
			AddScene(*layer.scene, path);

		path.push_back(BuildRuntimeLayerPathFragment(layer));
	}

	SurfRideElementPath BuildRuntimeScenePath(const SurfRide::Scene& scene) {
		SurfRideElementPath res{};
		AddScene(scene, res);
		return res;
	}

	SurfRideElementPath BuildRuntimeCameraPath(const SurfRide::Camera& camera) {
		return SurfRideElementPath{};
	}

	SurfRideElementPath BuildRuntimeLayerPath(const SurfRide::Layer& layer) {
		SurfRideElementPath res{};
		AddLayer(layer, res);
		return res;
	}

	SurfRideElementPath BuildRuntimeCastPath(const SurfRide::Cast& cast) {
		SurfRideElementPath res{};
		AddCast(cast, res);
		return res;
	}

	bool SurfRideElementPath::operator==(const SurfRideElementPath& other) const {
		if (size() != other.size())
			return false;

		for (size_t i = 0; i < size(); i++)
			if ((*this)[i] != other[i])
				return false;

		return true;
	}

	bool SurfRideElementPath::operator!=(const SurfRideElementPath& other) const {
		return !operator==(other);
	}
}
