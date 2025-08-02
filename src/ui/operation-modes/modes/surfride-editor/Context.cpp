#include "Context.h"
#include <span>
#include <resources/ReloadManager.h>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::swif_version;

#ifdef DEVTOOLS_TARGET_SDK_wars
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene) {
		auto resolution = SurfRide::GetScreenResolution();
		auto projectionMatrix = csl::math::CreatePerspectiveProjectionMatrix(
			static_cast<float>(scene->camera.camera.cameraData.fov) * 0.000095873802f,
			static_cast<float>(resolution.width) / static_cast<float>(resolution.height),
			scene->camera.camera.cameraData.nearPlane,
			scene->camera.camera.cameraData.farPlane
		);
		return projectionMatrix * scene->camera.viewMatrix;
	}
#else
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene) {
		return scene->camera.projectionMatrix * scene->camera.viewMatrix;
	}
#endif

	csl::math::Matrix44 Context::GetFocusedSceneCameraMatrix() const {
		return GetSceneCameraMatrix(FindRuntimeScene(focusedScene->id));
	}

	csl::math::Matrix44 Context::GetFullCastTransform(const SurfRide::Cast* cast) {
		return GetSceneCameraMatrix(cast->layer->scene) * cast->transform->transformationMatrix;
	}

	void Context::ReloadResource() {
		ReloadManager::instance->ReloadSync(projectResource);

		if (!gocSprite)
			return;

#ifdef DEVTOOLS_TARGET_SDK_rangers
		if (auto* gocUICompo = gocSprite->owner->GetComponent<hh::ui::GOCUIComposition>()) {
			class ReloadVisitor : public hh::ui::UIElementTraverser {
			public:
				Context& ctx;

				ReloadVisitor(Context& ctx) : ctx{ ctx } {}

				virtual void BeginElement(hh::ui::UIElement* element) override {
					if (element->GetRuntimeTypeInfo() == (void*)0x143CE6880) {
						auto* el = (hh::ui::UIListViewElement*)element;
						auto* castHandle = *el->hCast;

						csl::ut::MoveArray<hh::fnd::Reference<hh::ui::UIObject>> tmpElements{ hh::fnd::MemoryRouter::GetTempAllocator() };

						for (auto& child : el->layout.panel->children)
							tmpElements.push_back(child);
						
						el->Reset();
						el->SetupLayout(castHandle->GetCast());
						el->Deinitialize();
						el->initialized = 0;
						el->Initialize();
						el->Reset();

						size_t i{};
						el->NUnkFunc1(-1, 0, 0);
						for (auto& child : tmpElements) {
							child->position = { 0, 0 };
							el->AddItem((hh::ui::UIListViewItem*)&*child);
							el->NUnkFunc1(i++, 0, 0);
						}
					}
					//element->MarkUpdateRequired(true);
				}
			};

			ReloadVisitor v{ *this };

			for (auto* group : gocUICompo->elementGroups)
				group->Traverse(&v);
		}
#endif
		//for (auto* tou : textObjectUpdaters) {
		//	if (tou->textObject == nullptr)
		//		continue;

		//	csl::ut::VariableString str{ hh::fnd::MemoryRouter::GetTempAllocator() };
		//	hh::ui::TextStyleParameters styleParams{ hh::fnd::MemoryRouter::GetTempAllocator() };

		//	if (tou->LoadTextParameters(str, styleParams)) {
		//		hh::fnd::Reference<hh::ui::TextData> txtData{ tou->textObject->textData };
		//		tou->Clear();
		//		tou->SetTextData(txtData);
		//		tou->SetStyleParameters(styleParams);
		//		tou->textObject->SetTextData(txtData);
		//		tou->Update();
		//	}
		//}
		//project = gocSprite == nullptr ? nullptr : gocSprite->GetProject()->projectData;
	}

	unsigned short Context::GenerateUniqueId() {
		unsigned short res{};

		do { res = mt(); } while (IsIdTaken(res));

		return res;
	}

	bool Context::IsIdTaken(unsigned short id) const {
		auto& project = *gocSprite->GetProject()->projectData;

		for (auto& textureList : std::span(project.textureLists, project.textureListCount))
			for (auto& texture : std::span(textureList.textures, textureList.textureCount))
				if (texture.id == id)
					return true;

		for (auto& scene : std::span(project.scenes, project.sceneCount)) {
			if (scene.id == id)
				return true;

			for (auto& layer : std::span(scene.layers, scene.layerCount)) {
				if (layer.id == id)
					return true;

				for (auto& cast : std::span(layer.casts, layer.castCount))
					if (cast.id == id)
						return true;

				for (auto& animation : std::span(layer.animations, layer.animationCount))
					if (animation.id == id)
						return true;
			}

			for (auto& camera : std::span(scene.cameras, scene.cameraCount))
				if (camera.id == id)
					return true;
		}
		
		for (auto& font : std::span(project.fonts, project.fontCount))
			if (font.id == id)
				return true;

		return false;
	}

	SRS_CASTNODE* Context::CreateCast(SRS_LAYER& layer)
	{
		resources::ManagedCArray<SRS_CASTNODE, int> casts{ projectResource, layer.casts, layer.castCount };

		auto prevSize = casts.size();

		SRS_CASTNODE newNode{};
		newNode.name = "newcast";
		newNode.id = GenerateUniqueId();
		newNode.flags = 0x750;
		newNode.data.none = nullptr;
		newNode.SetType(SRS_CASTNODE::Type::NORMAL);

		casts.push_back(std::move(newNode));

		resources::ManagedCArray<SRS_TRS3D, int> transforms{ projectResource, layer.transforms.transforms3d, prevSize };
		transforms.emplace_back();
		transforms[transforms.size() - 1].materialColor = { 255, 255, 255, 255 };
		transforms[transforms.size() - 1].display = true;

		return &casts[casts.size() - 1];
	}

	SRS_CASTNODE* Context::CreateImageCast(SRS_LAYER& layer) {
		auto* cast = CreateCast(layer);

		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		auto* imageCast = new (&managedAllocator) SRS_IMAGECAST{};
		imageCast->flags = 0x00202000;
		imageCast->size = { 100.0f, 100.0f };
		imageCast->SetPivotType(EPivotType::CENTER_CENTER);
		imageCast->vertexColorTopLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorTopRight = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomRight = { 255, 255, 255, 255 };

		cast->data.image = imageCast;
		cast->SetType(SRS_CASTNODE::Type::IMAGE);

		return cast;
	}

	SRS_CASTNODE* Context::CreateSliceCast(SRS_LAYER& layer) {
		auto* cast = CreateCast(layer);

		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		char* buffer = new (&managedAllocator) char[sizeof(SRS_SLICECAST) + 4 * sizeof(SRS_SLICE)];

		auto* sliceCast = new (buffer) SRS_SLICECAST{};
		auto* slices = new (buffer + sizeof(SRS_SLICECAST)) SRS_SLICE[4]{};

		sliceCast->flags = 0x00202000;
		sliceCast->size = { 100.0f, 100.0f };
		sliceCast->SetPivotType(EPivotType::CENTER_CENTER);
		sliceCast->vertexColorTopLeft = { 255, 255, 255, 255 };
		sliceCast->vertexColorBottomLeft = { 255, 255, 255, 255 };
		sliceCast->vertexColorTopRight = { 255, 255, 255, 255 };
		sliceCast->vertexColorBottomRight = { 255, 255, 255, 255 };
		sliceCast->sliceHorizontalCount = 2;
		sliceCast->sliceVerticalCount = 2;

		cast->data.slice = sliceCast;
		cast->SetType(SRS_CASTNODE::Type::SLICE);

		return cast;
	}

	SRS_CASTNODE* Context::CreateReferenceCast(SRS_LAYER& layer) {
		auto* cast = CreateCast(layer);

		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		auto* referenceCast = new (&managedAllocator) SRS_REFERENCECAST{};

		referenceCast->flags = 0x0;

		cast->data.reference = referenceCast;
		cast->SetType(SRS_CASTNODE::Type::REFERENCE);

		return cast;
	}

	void Context::SetResource(hh::ui::GOCSprite* goc) {
		gocSprite = static_cast<hh::ui::GOCSprite*>(goc);
		projectResource = GetResourceForComponent(goc);
		project = goc == nullptr ? nullptr : gocSprite->GetProject()->projectData;
		focusedScene = nullptr;
	}

	hh::ui::ResSurfRideProject* Context::GetResourceForComponent(hh::ui::GOCSprite* goc) {
#ifdef DEVTOOLS_TARGET_SDK_wars
		if (!goc || goc->projectContexts.size() == 0)
			return nullptr;

		auto* resourceManager = hh::fnd::ResourceManager::GetInstance();

		if (auto* res = resourceManager->GetResource<hh::ui::ResSurfRideProject>(goc->projectContexts[0].name.c_str()))
			return res;

		auto* packFileContainer = resourceManager->GetResourceContainer(hh::fnd::Packfile::GetTypeInfo());
			
		auto packFileCount = packFileContainer->GetNumResources();
			
		for (int i = 0; i < packFileCount; i++)
			if (auto* packFileRes = static_cast<hh::fnd::Packfile*>(packFileContainer->GetResourceByIndex(i))->GetResourceByName<hh::ui::ResSurfRideProject>(goc->projectContexts[0].name.c_str()))
				return packFileRes;

		return nullptr;
#else
		return goc == nullptr ? nullptr : goc->projectResource;
#endif
	}
	
	SRS_CASTNODE* Context::CreateCast(SRS_LAYER& layer, SRS_CASTNODE::Type type) {
		switch (type) {
		case SRS_CASTNODE::Type::NORMAL: return CreateCast(layer);
		case SRS_CASTNODE::Type::IMAGE: return CreateImageCast(layer);
		case SRS_CASTNODE::Type::SLICE: return CreateSliceCast(layer);
		case SRS_CASTNODE::Type::REFERENCE: return CreateReferenceCast(layer);
		}
	}

	void Context::AddCast(SRS_LAYER& layer, SRS_CASTNODE::Type type) {
		auto* cast = CreateCast(layer, type);

		if (layer.castCount > 1)
			FindLastSibling(layer, 0).siblingIndex = static_cast<short>(cast - layer.casts);

		ReloadResource();
	}

	void Context::AddCast(SRS_CASTNODE& parent, SRS_CASTNODE::Type type) {
		SRS_LAYER& layer = *FindCastLayer(parent.id);
		size_t parentIdx = &parent - layer.casts;

		auto* cast = CreateCast(layer, type);

		// Creating a cast might have reallocated the casts array, so need to look it up again.
		auto& newParent = layer.casts[parentIdx];

		if (newParent.childIndex == -1)
			newParent.childIndex = static_cast<short>(cast - layer.casts);
		else
			FindLastSibling(layer, newParent.childIndex).siblingIndex = static_cast<short>(cast - layer.casts);

		ReloadResource();
	}

	void Context::RemoveCast(SRS_CASTNODE& cast_)
	{
		auto castId = cast_.id;

		auto& layer = *FindCastLayer(castId);

		ForEachChild(layer, cast_, [this](auto& child) { RemoveCast(child); });

		// Removing children could have shifted the cast. Look it up again by ID.
		auto& cast = *FindCast(layer, castId);
		size_t castIndex = &cast - layer.casts;

		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		switch (cast.GetType()) {
		case SRS_CASTNODE::Type::IMAGE:
			if (cast.data.image->textData)
				managedAllocator.Free(cast.data.image->textData);
			if (cast.data.image->effectData.blur)
				managedAllocator.Free(cast.data.image->effectData.blur);
			managedAllocator.Free(cast.data.image);
			break;
		case SRS_CASTNODE::Type::SLICE:
			if (cast.data.slice->effectData.blur)
				managedAllocator.Free(cast.data.slice->effectData.blur);
			managedAllocator.Free(cast.data.slice);
			break;
		}

		auto siblingIndex = cast.siblingIndex;

		auto tempCount = layer.castCount;

		resources::ManagedCArray<SRS_CASTNODE, int> casts{ projectResource, layer.casts, layer.castCount };

		for (auto& cast : casts) {
			if (cast.siblingIndex == castIndex)
				cast.siblingIndex = siblingIndex;

			if (cast.childIndex == castIndex)
				cast.childIndex = siblingIndex;
		}

		for (auto& cast : casts) {
			if (cast.siblingIndex != -1 && cast.siblingIndex > castIndex)
				cast.siblingIndex--;

			if (cast.childIndex != -1 && cast.childIndex > castIndex)
				cast.childIndex--;
		}

		casts.remove(castIndex);

		if (layer.Is3D())
			resources::ManagedCArray<SRS_TRS3D, int>{ projectResource, layer.transforms.transforms3d, tempCount }.remove(castIndex);
		else
			resources::ManagedCArray<SRS_TRS2D, int>{ projectResource, layer.transforms.transforms2d, tempCount }.remove(castIndex);

		ReloadResource();
	}

	void Context::AddMotion(SRS_ANIMATION& animation, SRS_CASTNODE& cast) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		resources::ManagedCArray<SRS_MOTION, unsigned int> motions{ gocSprite->projectResource, animation.motions, animation.motionCount };

		auto& motion = motions.emplace_back();
		motion.castId = static_cast<unsigned short>(cast.id);
		motion.tracks = nullptr;
		motion.trackCount = 0;

		ReloadResource();
	}

	SRS_TRACK& Context::AddTrack(SRS_MOTION& motion, ECurveType type, unsigned int firstFrame, unsigned int lastFrame, ucsl::resources::swif::swif_version::EInterpolationType interpolationType) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		resources::ManagedCArray<SRS_TRACK, unsigned short> tracks{ gocSprite->projectResource, motion.tracks, motion.trackCount };

		auto& track = tracks.emplace_back();
		track.trackType = type;
		track.flags = static_cast<unsigned int>(interpolationType) | (static_cast<unsigned int>(GetTrackDataTypeForCurveType(type)) << 4);
		track.firstFrame = firstFrame;
		track.lastFrame = lastFrame;
		track.keyFrames.constantBool = nullptr;
		track.keyCount = 0;

		return track;
	}

	void Context::AddKeyFrame(SRS_TRACK& track, unsigned int frame) {
		switch (track.GetInterpolationType()) {
		case EInterpolationType::CONSTANT:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<Key<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<Key<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<Key<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<Key<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<Key<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::LINEAR:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyLinear<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyLinear<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyLinear<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyLinear<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyLinear<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::HERMITE:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyHermite<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyHermite<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyHermite<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyHermite<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyHermite<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::INDIVIDUAL:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyIndividual<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyIndividual<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyIndividual<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyIndividual<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyIndividual<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		default: assert(false && "Invalid track flags"); break;
		}
	}

	void Context::RemoveMotion(SRS_ANIMATION& animation, SRS_MOTION& motion) {
		resources::ManagedCArray<SRS_MOTION, unsigned int> motions{ gocSprite->projectResource, animation.motions, animation.motionCount };

		size_t motionIndex = &motion - animation.motions;
		motions.remove(motionIndex);

		ReloadResource();
	}

	void Context::RemoveTrack(SRS_MOTION& motion, SRS_TRACK& track) {
		resources::ManagedCArray<SRS_TRACK, unsigned short> tracks{ gocSprite->projectResource, motion.tracks, motion.trackCount };

		size_t trackIndex = &track - motion.tracks;
		tracks.remove(trackIndex);
	}

	void Context::RemoveKeyFrame(SRS_TRACK& track, unsigned int frameIndex)
	{
		switch (track.GetInterpolationType()) {
		case EInterpolationType::CONSTANT:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: RemoveKeyFrameEx<Key<float>>(track, frameIndex); break;
			case ETrackDataType::INDEX: RemoveKeyFrameEx<Key<int>>(track, frameIndex); break;
			case ETrackDataType::INT: RemoveKeyFrameEx<Key<int>>(track, frameIndex); break;
			case ETrackDataType::BOOL: RemoveKeyFrameEx<Key<bool>>(track, frameIndex); break;
			case ETrackDataType::COLOR: RemoveKeyFrameEx<Key<Color>>(track, frameIndex); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::LINEAR:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: RemoveKeyFrameEx<KeyLinear<float>>(track, frameIndex); break;
			case ETrackDataType::INDEX: RemoveKeyFrameEx<KeyLinear<int>>(track, frameIndex); break;
			case ETrackDataType::INT: RemoveKeyFrameEx<KeyLinear<int>>(track, frameIndex); break;
			case ETrackDataType::BOOL: RemoveKeyFrameEx<KeyLinear<bool>>(track, frameIndex); break;
			case ETrackDataType::COLOR: RemoveKeyFrameEx<KeyLinear<Color>>(track, frameIndex); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::HERMITE:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: RemoveKeyFrameEx<KeyHermite<float>>(track, frameIndex); break;
			case ETrackDataType::INDEX: RemoveKeyFrameEx<KeyHermite<int>>(track, frameIndex); break;
			case ETrackDataType::INT: RemoveKeyFrameEx<KeyHermite<int>>(track, frameIndex); break;
			case ETrackDataType::BOOL: RemoveKeyFrameEx<KeyHermite<bool>>(track, frameIndex); break;
			case ETrackDataType::COLOR: RemoveKeyFrameEx<KeyHermite<Color>>(track, frameIndex); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::INDIVIDUAL:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: RemoveKeyFrameEx<KeyIndividual<float>>(track, frameIndex); break;
			case ETrackDataType::INDEX: RemoveKeyFrameEx<KeyIndividual<int>>(track, frameIndex); break;
			case ETrackDataType::INT: RemoveKeyFrameEx<KeyIndividual<int>>(track, frameIndex); break;
			case ETrackDataType::BOOL: RemoveKeyFrameEx<KeyIndividual<bool>>(track, frameIndex); break;
			case ETrackDataType::COLOR: RemoveKeyFrameEx<KeyIndividual<Color>>(track, frameIndex); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		default: assert(false && "Invalid track flags"); break;
		}
	}

	void Context::SetTrackStart(SRS_TRACK& track, unsigned int frame) {
		track.firstFrame = frame;
	}

	void Context::SetTrackEnd(SRS_TRACK& track, unsigned int frame) {
		track.lastFrame = frame;
	}

	void Context::MoveTrack(SRS_TRACK& track, int delta) {
		VisitKeyFrames(track, [&track, delta](auto* keys) {
			for (auto& key : std::span(keys, track.keyCount))
				key.frame = static_cast<unsigned int>(static_cast<int>(key.frame) + delta);
		});
	}

	void Context::SetTrackInterpolationType(ucsl::resources::swif::swif_version::SRS_TRACK& track, ucsl::resources::swif::swif_version::EInterpolationType interpolationType) {
		VisitKeyFrames(track, [this, &track, interpolationType](auto*& kf1){
			using KeyFrameType1 = std::remove_pointer_t<std::remove_reference_t<decltype(kf1)>>;
			using ValueType1 = typename KeyFrameType1::ValueType;

			csl::ut::MoveArray<std::tuple<unsigned int, ValueType1>> savedData{ hh::fnd::MemoryRouter::GetTempAllocator() };

			resources::ManagedCArray kfs1{ projectResource, kf1, track.keyCount };

			for (auto& kf : kfs1)
				savedData.push_back({ kf.frame, kf.value });

			kfs1.clear();

			track.SetInterpolationType(interpolationType);

			VisitKeyFrames(track, [this, &track, &savedData](auto*& kf2) {
				using KeyFrameType2 = std::remove_pointer_t<std::remove_reference_t<decltype(kf2)>>;
				using ValueType2 = typename KeyFrameType2::ValueType;

				if constexpr (!std::is_same_v<ValueType1, ValueType2>)
					assert(false && "impossible situation");
				else {
					resources::ManagedCArray kfs2{ projectResource, kf2, track.keyCount };

					for (auto& savedDatum : savedData) {
						auto& kf = kfs2.emplace_back();
						kf.frame = std::get<0>(savedDatum);
						kf.value = std::get<1>(savedDatum);
					}
				}
			});
		});
	}

	//SRS_SCENE* Context::FindScene(unsigned int id) const {
	//	for (auto& scene : std::span(project->scenes, project->sceneCount))
	//		if (scene.id == id)
	//			return &scene;

	//	return nullptr;
	//}

	//SRS_LAYER* Context::FindLayer(SRS_SCENE& scene, unsigned int id) const {
	//	for (auto& layer : std::span(scene.layers, scene.layerCount))
	//		if (layer.id == id)
	//			return &layer;

	//	return nullptr;
	//}

	//SRS_LAYER* Context::FindLayer(unsigned int id) const {
	//	for (auto& scene : std::span(project->scenes, project->sceneCount))
	//		if (auto* layer = FindLayer(scene, id))
	//			return layer;

	//	return nullptr;
	//}

	//SRS_CAMERA* Context::FindCamera(SRS_SCENE& scene, unsigned int id) const {
	//	for (auto& camera : std::span(scene.cameras, scene.cameraCount))
	//		if (camera.id == id)
	//			return &camera;

	//	return nullptr;
	//}

	//SRS_CAMERA* Context::FindCamera(unsigned int id) const {
	//	for (auto& scene : std::span(project->scenes, project->sceneCount))
	//		if (auto* camera = FindCamera(scene, id))
	//			return camera;

	//	return nullptr;
	//}

	SRS_CASTNODE* Context::FindCast(SRS_LAYER& layer, unsigned int id) const {
		for (auto& cast : std::span(layer.casts, layer.castCount))
			if (cast.id == id)
				return &cast;

		return nullptr;
	}

	//SRS_CASTNODE* Context::FindCast(SRS_SCENE& scene, unsigned int id) const {
	//	for (auto& layer : std::span(scene.layers, scene.layerCount))
	//		if (auto* cast = FindCast(layer, id))
	//			return cast;

	//	return nullptr;
	//}

	//SRS_CASTNODE* Context::FindCast(unsigned int id) const {
	//	for (auto& scene : std::span(project->scenes, project->sceneCount))
	//		if (auto* cast = FindCast(scene, id))
	//			return cast;

	//	return nullptr;
	//}

	ucsl::resources::swif::swif_version::SRS_SCENE* Context::ResolveScene(const SurfRideElement& element) const {
		return ResolveScenePath(*project, element.path);
	}

	ucsl::resources::swif::swif_version::SRS_CAMERA* Context::ResolveCamera(const SurfRideElement& element) const {
		return ResolveCameraPath(*project, element.path);
	}

	ucsl::resources::swif::swif_version::SRS_LAYER* Context::ResolveLayer(const SurfRideElement& element) const {
		return ResolveLayerPath(*project, element.path);
	}

	ucsl::resources::swif::swif_version::SRS_CASTNODE* Context::ResolveCast(const SurfRideElement& element) const {
		return ResolveCastPath(*project, element.path);
	}

	SurfRide::Scene* Context::ResolveRuntimeScene(const SurfRideElement& element) const {
		return !element.runtimePath ? nullptr : ResolveRuntimeScenePath(*gocSprite->GetProject(), *element.runtimePath);
	}

	SurfRide::Camera* Context::ResolveRuntimeCamera(const SurfRideElement& element) const {
		return !element.runtimePath ? nullptr : ResolveRuntimeCameraPath(*gocSprite->GetProject(), *element.runtimePath);
	}

	SurfRide::Layer* Context::ResolveRuntimeLayer(const SurfRideElement& element) const {
		return !element.runtimePath ? nullptr : ResolveRuntimeLayerPath(*gocSprite->GetProject(), *element.runtimePath);
	}

	SurfRide::Cast* Context::ResolveRuntimeCast(const SurfRideElement& element) const {
		return !element.runtimePath ? nullptr : ResolveRuntimeCastPath(*gocSprite->GetProject(), *element.runtimePath);
	}

	SRS_LAYER* Context::FindCastLayer(unsigned int castId) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			for (auto& layer : std::span(scene.layers, scene.layerCount))
				for (auto& cast : std::span(layer.casts, layer.castCount))
					if (cast.id == castId)
						return &layer;

		return nullptr;
	}

	SRS_CASTNODE* Context::FindCastParent(unsigned int castId) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount)) {
			for (auto& layer : std::span(scene.layers, scene.layerCount)) {
				for (auto& cast : std::span(layer.casts, layer.castCount)) {
					bool found{};

					ForEachChild(layer, cast, [&found, castId](auto& child) {
						if (child.id == castId)
							found = true;
					});

					if (found)
						return &cast;
				}
			}
		}

		return nullptr;
	}

	SRS_CASTNODE& Context::FindLastSibling(const SRS_LAYER& layer, int sibling) const {
		auto* res = &layer.casts[sibling];

		while (res->siblingIndex != -1)
			res = &layer.casts[res->siblingIndex];

		return *res;
	}

	SurfRide::Scene* Context::FindRuntimeScene(unsigned int id) const {
		if (gocSprite == nullptr)
			return nullptr;

		auto* runtimeProject = gocSprite->GetProject();

		if (runtimeProject == nullptr)
			return nullptr;

		for (auto scene : gocSprite->GetProject()->GetScenes())
			if (scene->sceneData->id == id)
				return &*scene;

		return nullptr;
	}

	//SurfRide::Layer* Context::FindRuntimeLayer(unsigned int id) const {
	//	if (gocSprite == nullptr)
	//		return nullptr;

	//	for (auto scene : gocSprite->GetProject()->GetScenes())
	//		if (auto* layer = scene->GetLayer(id))
	//			return layer;

	//	return nullptr;
	//}

	//SurfRide::Cast* Context::FindRuntimeCast(unsigned int id) const {
	//	auto* layer = FindCastLayer(id);

	//	if (!layer)
	//		return nullptr;

	//	auto* runtimeLayer = FindRuntimeLayer(layer->id);

	//	if (!runtimeLayer)
	//		return nullptr;

	//	return runtimeLayer->GetCast(id);
	//}

	void Context::StartAnimation(SurfRide::Layer* runtimeLayer, int animationId) {
		runtimeLayer->ApplyAnimation(animationId);
	}

	float Context::GetAnimationFrame(SurfRide::Layer* runtimeLayer) const {
		return runtimeLayer->currentFrame3;
	}

	void Context::SetAnimationFrame(SurfRide::Layer* runtimeLayer, float frame) {
		runtimeLayer->SetCurrentFrame(frame);
	}

	ETrackDataType Context::GetTrackDataTypeForCurveType(ECurveType curveType)
	{
		switch (curveType) {
		case ECurveType::TranslationX: return ETrackDataType::FLOAT;
		case ECurveType::TranslationY: return ETrackDataType::FLOAT;
		case ECurveType::TranslationZ: return ETrackDataType::FLOAT;
		case ECurveType::RotationX: return ETrackDataType::INT;
		case ECurveType::RotationY: return ETrackDataType::INT;
		case ECurveType::RotationZ: return ETrackDataType::INT;
		case ECurveType::ScaleX: return ETrackDataType::FLOAT;
		case ECurveType::ScaleY: return ETrackDataType::FLOAT;
		case ECurveType::ScaleZ: return ETrackDataType::FLOAT;
		case ECurveType::MaterialColorR: return ETrackDataType::FLOAT;
		case ECurveType::MaterialColorG: return ETrackDataType::FLOAT;
		case ECurveType::MaterialColorB: return ETrackDataType::FLOAT;
		case ECurveType::MaterialColorA: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopLeftR: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopLeftG: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopLeftB: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopLeftA: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopRightR: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopRightG: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopRightB: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopRightA: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomLeftR: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomLeftG: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomLeftB: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomLeftA: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomRightR: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomRightG: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomRightB: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomRightA: return ETrackDataType::FLOAT;
		case ECurveType::IlluminationColorR: return ETrackDataType::FLOAT;
		case ECurveType::IlluminationColorG: return ETrackDataType::FLOAT;
		case ECurveType::IlluminationColorB: return ETrackDataType::FLOAT;
		case ECurveType::IlluminationColorA: return ETrackDataType::FLOAT;
		case ECurveType::Display: return ETrackDataType::BOOL;
		case ECurveType::Width: return ETrackDataType::INT;
		case ECurveType::Height: return ETrackDataType::INT;
		case ECurveType::CropIndex0: return ETrackDataType::INDEX;
		case ECurveType::CropIndex1: return ETrackDataType::INDEX;
		case ECurveType::VertexColorTopLeft: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorTopRight: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomLeft: return ETrackDataType::FLOAT;
		case ECurveType::VertexColorBottomRight: return ETrackDataType::FLOAT;
		case ECurveType::MaterialColor: return ETrackDataType::COLOR;
		case ECurveType::IlluminationColor: return ETrackDataType::COLOR;
		default: return ETrackDataType::FLOAT;
		}
	}

	Eigen::Translation3f Context::GetPivotTranslation(EPivotType pivotType, const Vector2& customPivot, const Vector2& size)
	{
		switch (pivotType) {
		case EPivotType::TOP_LEFT: return Eigen::Translation3f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f };
		case EPivotType::TOP_CENTER: return Eigen::Translation3f{ 0.0f, -size.y() / 2.0f, 0.0f };
		case EPivotType::TOP_RIGHT: return Eigen::Translation3f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f };
		case EPivotType::CENTER_LEFT: return Eigen::Translation3f{ size.x() / 2.0f, 0.0f, 0.0f };
		case EPivotType::CENTER_CENTER: return Eigen::Translation3f{ 0.0f, 0.0f, 0.0f };
		case EPivotType::CENTER_RIGHT: return Eigen::Translation3f{ -size.x() / 2.0f, 0.0f, 0.0f };
		case EPivotType::BOTTOM_LEFT: return Eigen::Translation3f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f };
		case EPivotType::BOTTOM_CENTER: return Eigen::Translation3f{ 0.0f, size.y() / 2.0f, 0.0f };
		case EPivotType::BOTTOM_RIGHT: return Eigen::Translation3f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f };
		case EPivotType::CUSTOM: return Eigen::Translation3f{ size.x() / 2.0f - customPivot.x(), -size.y() / 2.0f + customPivot.y(), 0.0f };
		default: assert("invalid pivot type"); return Eigen::Translation3f{ 0.0f, 0.0f, 0.0f };
		}
	}

	Eigen::Translation3f Context::GetPivotTranslation(const SurfRide::Cast* cast)
	{
		switch (cast->castData->GetType()) {
		case SRS_CASTNODE::Type::IMAGE: {
			auto* imageCast = static_cast<const SurfRide::ImageCast*>(cast);

			return GetPivotTranslation(imageCast->imageCastData->GetPivotType(), imageCast->imageCastData->pivot, imageCast->size);
		}
		case SRS_CASTNODE::Type::SLICE: {
			auto* sliceCast = static_cast<const SurfRide::SliceCast*>(cast);

			return GetPivotTranslation(sliceCast->sliceCastData->GetPivotType(), sliceCast->sliceCastData->pivot, sliceCast->size);
		}
		default: return {};
		}
	}

	void Context::UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, EPivotType pivotType, const Vector2& customPivot, const Vector2& size, csl::geom::Aabb& aabb)
	{
		auto pivotTrans = GetPivotTranslation(pivotType, customPivot, size);

		aabb.AddPoint((transform * pivotTrans * Eigen::Vector3f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
		aabb.AddPoint((transform * pivotTrans * Eigen::Vector3f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
		aabb.AddPoint((transform * pivotTrans * Eigen::Vector3f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
		aabb.AddPoint((transform * pivotTrans * Eigen::Vector3f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
	}

	bool Context::UpdateAabbWithoutChildren(const SurfRide::Cast* cast, csl::geom::Aabb& aabb)
	{
		bool updated{};

		//if (cast->flags & 0x1000 || !cast->transform->fullDisplay)
		//	return false;

		switch (cast->castData->GetType()) {
		case SRS_CASTNODE::Type::IMAGE: {
			auto* imageCast = static_cast<const SurfRide::ImageCast*>(cast);

			UpdateAabb(GetFullCastTransform(cast), imageCast->imageCastData->GetPivotType(), imageCast->imageCastData->pivot, imageCast->size, aabb);
			updated |= true;
			break;
		}
		case SRS_CASTNODE::Type::SLICE: {
			auto* sliceCast = static_cast<const SurfRide::SliceCast*>(cast);

			UpdateAabb(GetFullCastTransform(cast), sliceCast->sliceCastData->GetPivotType(), sliceCast->sliceCastData->pivot, sliceCast->size, aabb);
			updated |= true;
			break;
		}
		case SRS_CASTNODE::Type::REFERENCE: {
			if (auto* refLayer = static_cast<const SurfRide::ReferenceCast*>(cast)->refLayer)
				updated |= UpdateAabb(refLayer, aabb);
			break;
		}
		}

		return updated;
	}

	bool Context::UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb)
	{
		bool updated{};

		//if (cast->flags & 0x1000 || !cast->transform->fullDisplay)
		//	return false;

		for (auto child : cast->GetChildren())
			updated |= UpdateAabb(child, aabb);

		updated |= UpdateAabbWithoutChildren(cast, aabb);

		return updated;
	}

	bool Context::UpdateAabb(const SurfRide::Layer* layer, csl::geom::Aabb& aabb)
	{
		bool updated{};

		//if (layer->flags & 0x100)
		//	return false;

		for (auto cast : layer->GetCasts())
			updated |= UpdateAabb(cast, aabb);

		return updated;
	}

	void Context::ApplyTransformChange(const SRS_CASTNODE& cast) {
		auto* layer = FindCastLayer(cast.id);
		size_t castIndex = &cast - layer->casts;

		ForEachRuntimeCast(cast.id, [layer, castIndex, &cast](auto& runtimeCast) {
			if (layer->Is3D()) {
				auto& transform = layer->transforms.transforms3d[castIndex];

#ifdef DEVTOOLS_TARGET_SDK_wars
				auto* castTransform = reinterpret_cast<SRS_TRS3D*>(runtimeCast.transformData);
				castTransform->position = transform.position;
				castTransform->rotation = transform.rotation;
				castTransform->scale = transform.scale;
				static_cast<SurfRide::Cast3D&>(runtimeCast).position = transform.position;
#else
				runtimeCast.transform->position = transform.position;
				runtimeCast.transform->rotation = transform.rotation;
				runtimeCast.transform->scale = transform.scale;
#endif
				runtimeCast.flags = cast.flags;
				runtimeCast.transform->materialColor = transform.materialColor;
				runtimeCast.transform->illuminationColor = transform.illuminationColor;
				runtimeCast.transform->display = transform.display;

				runtimeCast.transform->dirtyFlag.SetCellAll();
				runtimeCast.transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
				runtimeCast.UpdateParentsAndThisTransformRecursively();
#endif
			}
			else {
				auto& transform = layer->transforms.transforms2d[castIndex];

#ifdef DEVTOOLS_TARGET_SDK_wars
				auto* castTransform = reinterpret_cast<SRS_TRS3D*>(runtimeCast.transformData);
				castTransform->position = { transform.position.x(), transform.position.y(), 0.0f };
				castTransform->rotation = { 0, 0, transform.rotation };
				castTransform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
				static_cast<SurfRide::Cast3D&>(runtimeCast).position = { transform.position.x(), transform.position.y(), 0.0f };
#else
				runtimeCast.transform->position = { transform.position.x(), transform.position.y(), 0.0f };
				runtimeCast.transform->rotation = { 0, 0, transform.rotation };
				runtimeCast.transform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
#endif
				runtimeCast.flags = cast.flags;
				runtimeCast.transform->materialColor = transform.materialColor;
				runtimeCast.transform->illuminationColor = transform.illuminationColor;
				runtimeCast.transform->display = transform.display;

				runtimeCast.transform->dirtyFlag.SetCellAll();
				runtimeCast.transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
				runtimeCast.UpdateParentsAndThisTransformRecursively();
#endif
			}
		});
	}

	void Context::ApplyImageCastChange(const SRS_CASTNODE& cast) {
		auto& imageCast = *cast.data.image;

		ForEachRuntimeCast(cast.id, [&imageCast](auto& runtimeCast_) {
			auto& runtimeCast = static_cast<SurfRide::ImageCast&>(runtimeCast_);

			runtimeCast.size = imageCast.size;
#ifndef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.vertexColorTopLeft = imageCast.vertexColorTopLeft;
			runtimeCast.vertexColorBottomLeft = imageCast.vertexColorBottomLeft;
			runtimeCast.vertexColorTopRight = imageCast.vertexColorTopRight;
			runtimeCast.vertexColorBottomRight = imageCast.vertexColorBottomRight;
#endif
			runtimeCast.cropIndex[0] = imageCast.cropIndex0;
			runtimeCast.cropIndex[1] = imageCast.cropIndex1;
			//runtimeCast.cropRectDirty[0] = true;
			//runtimeCast.cropRectDirty[1] = true;

#ifndef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.blurEffect = nullptr;
			runtimeCast.reflectEffect = nullptr;
			runtimeCast.CreateEffectCast(imageCast.effectData.blur);
#endif

			runtimeCast.transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.UpdateParentsAndThisTransformRecursively();
#endif
		});
	}

	void Context::ApplyReferenceCastChange(const SRS_CASTNODE& cast) {
	}

	void Context::ApplySliceCastChange(const SRS_CASTNODE& cast) {
		auto& sliceCast = *cast.data.slice;

		ForEachRuntimeCast(cast.id, [&sliceCast](auto& runtimeCast_) {
			auto& runtimeCast = static_cast<SurfRide::SliceCast&>(runtimeCast_);

			runtimeCast.size = sliceCast.size;
#ifndef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.vertexColorTopLeft = sliceCast.vertexColorTopLeft;
			runtimeCast.vertexColorBottomLeft = sliceCast.vertexColorBottomLeft;
			runtimeCast.vertexColorTopRight = sliceCast.vertexColorTopRight;
			runtimeCast.vertexColorBottomRight = sliceCast.vertexColorBottomRight;
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.blurEffect = nullptr;
			runtimeCast.reflectEffect = nullptr;
			runtimeCast.CreateEffectCast(sliceCast.effectData.blur);
#endif
			runtimeCast.transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			runtimeCast.UpdateParentsAndThisTransformRecursively();
#endif
		});
	}
}
