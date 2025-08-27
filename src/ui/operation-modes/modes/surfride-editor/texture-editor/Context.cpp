#include "Context.h"
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <span>

using namespace ucsl::resources::swif::swif_version;

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
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

	void Context::AddTexture(const TextureListRef& textureListRef, hh::gfnd::ResTexture* textureResource) {
		auto* projectResource = GetResourceForComponent(gocSprite);
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		auto* textureList = textureListRef.GetTextureListInstance();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*projectResource->textureDatas[textureListRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

#ifndef DEVTOOLS_TARGET_SDK_wars
		char* name = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(name, textureResource->GetName());
#endif

		char* filename = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(filename, textureResource->GetName());

		SRS_TEXTURE srsTexture{};
		srsTexture.width = static_cast<unsigned short>(textureResource->GetTexture()->format.width);
		srsTexture.height = static_cast<unsigned short>(textureResource->GetTexture()->format.height);
		srsTexture.id = GenerateUniqueId();
#ifndef DEVTOOLS_TARGET_SDK_wars
		srsTexture.name = name;
#endif
		srsTexture.filename = filename;

		resources::ManagedCArray textures{ projectResource, textureList->textureListData->textures, textureList->textureListData->textureCount };

		textures.push_back(srsTexture);
		resourceTextureData->textures.push_back(textureResource->GetTexture());
		instanceTextureData->textures.push_back(textureResource->GetTexture());
	}

	void Context::ReplaceTexture(const TextureRef& textureRef, hh::gfnd::ResTexture* textureResource) {
		auto* projectResource = GetResourceForComponent(gocSprite);
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		auto* textureList = textureRef.GetTextureListInstance();
		auto& texture = textureRef.GetTexture();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*projectResource->textureDatas[textureRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

		managedAllocator.Free(const_cast<char*>(texture.filename));

		char* filename = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(filename, textureResource->GetName());

		texture.filename = filename;
		texture.width = static_cast<unsigned short>(textureResource->GetTexture()->format.width);
		texture.height = static_cast<unsigned short>(textureResource->GetTexture()->format.height);
		resourceTextureData->textures[textureRef.textureIndex] = textureResource->GetTexture();
		instanceTextureData->textures[textureRef.textureIndex] = textureResource->GetTexture();
	}

	void Context::RemoveTexture(const TextureRef& textureRef) {
		auto* projectResource = GetResourceForComponent(gocSprite);
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(projectResource);

		auto* textureList = textureRef.GetTextureListInstance();
		auto& texture = textureRef.GetTexture();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*projectResource->textureDatas[textureRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

#ifndef DEVTOOLS_TARGET_SDK_wars
		managedAllocator.Free(const_cast<char*>(texture.name));
#endif
		managedAllocator.Free(const_cast<char*>(texture.filename));
		managedAllocator.Free(const_cast<SRS_CROP*>(texture.crops));

		resources::ManagedCArray textures{ gocSprite->projectResource, textureList->textureListData->textures, textureList->textureListData->textureCount };

		textures.remove(static_cast<unsigned int>(textureRef.textureIndex));
		resourceTextureData->textures.remove(textureRef.textureIndex);
		instanceTextureData->textures.remove(textureRef.textureIndex);
	}

	CropRef Context::AddCrop(const TextureRef& textureRef, const SRS_CROP& crop)
	{
		auto& texture = textureRef.GetTexture();

		resources::ManagedCArray crops{ gocSprite->projectResource, texture.crops, texture.cropCount };

		crops.push_back(crop);

		return { textureRef.project, textureRef.textureListIndex, textureRef.textureIndex, crops.size() - 1 };
	}

	void Context::RemoveCrop(const CropRef& cropRef) {
		auto& texture = cropRef.GetTexture();

		resources::ManagedCArray crops{ gocSprite->projectResource, texture.crops, texture.cropCount };

		crops.remove(static_cast<unsigned int>(cropRef.cropIndex));
	}

	void Context::RefreshAfterTextureUpdate(const TextureRef& textureRef)
	{
		for (auto* scene : gocSprite->GetProject()->GetScenes())
			for (auto* layer : scene->GetLayers())
				for (auto* cast : layer->GetCasts())
					if (IsAffectedByTextureUpdate(textureRef, *cast->castData)) {
						cast->transform->dirtyFlag.SetCellAll();
						cast->transform->dirtyFlag.SetTransformAll();
					}
	}

	void Context::RefreshCastAfterTextureUpdate(const TextureRef& textureRef, SurfRide::Cast* cast)
	{
		if (IsAffectedByTextureUpdate(textureRef, *cast->castData)) {
			cast->transform->dirtyFlag.SetCellAll();
			cast->transform->dirtyFlag.SetTransformAll();
		}

		for (auto* child : cast->GetChildren())
			RefreshCastAfterTextureUpdate(textureRef, child);
	}

	bool Context::IsAffectedByTextureUpdate(const TextureRef& textureRef, SRS_CASTNODE& cast)
	{
		switch (cast.GetType()) {
		case SurfRide::SRS_CASTNODE::Type::IMAGE: {
			auto* data = cast.data.image;

			for (size_t i = 0; i < data->cropRef0Count; i++)
				if (data->cropRefs0[i].textureListIndex == textureRef.textureListIndex && data->cropRefs0[i].textureIndex == textureRef.textureIndex)
					return true;

			for (size_t i = 0; i < data->cropRef1Count; i++)
				if (data->cropRefs1[i].textureListIndex == textureRef.textureListIndex && data->cropRefs1[i].textureIndex == textureRef.textureIndex)
					return true;
		}
		//case SurfRide::SRS_CASTNODE::Type::SLICE: {
		//	auto* data = static_cast<SurfRide::SRS_SLICECAST*>(cast.data);

		//	for (size_t i = 0; i < data->cropRef0Count; i++)
		//		if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
		//			return true;
		//}
		}

		return false;
	}

	void Context::RefreshAfterCropUpdate(const CropRef& cropRef)
	{
		for (auto* scene : gocSprite->GetProject()->GetScenes())
			for (auto* layer : scene->GetLayers())
				for (auto* cast : layer->GetCasts())
					RefreshCastAfterCropUpdate(cropRef, cast);
	}

	void Context::RefreshCastAfterCropUpdate(const CropRef& cropRef, SurfRide::Cast* cast)
	{
		if (IsAffectedByCropUpdate(cropRef, *cast->castData))
			cast->transform->dirtyFlag.SetCellCropUV();

		for (auto* child : cast->GetChildren())
			RefreshCastAfterCropUpdate(cropRef, child);
	}

	bool Context::IsAffectedByCropUpdate(const CropRef& cropRef, SRS_CASTNODE& cast)
	{
		switch (cast.GetType()) {
		case SurfRide::SRS_CASTNODE::Type::IMAGE: {
			auto* data = cast.data.image;

			for (size_t i = 0; i < data->cropRef0Count; i++)
				if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
					return true;

			for (size_t i = 0; i < data->cropRef1Count; i++)
				if (data->cropRefs1[i].textureListIndex == cropRef.textureListIndex && data->cropRefs1[i].textureIndex == cropRef.textureIndex && data->cropRefs1[i].cropIndex == cropRef.cropIndex)
					return true;
		}
		//case SurfRide::SRS_CASTNODE::Type::SLICE: {
		//	auto* data = static_cast<SurfRide::SRS_SLICECAST*>(cast.data);

		//	for (size_t i = 0; i < data->cropRef0Count; i++)
		//		if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
		//			return true;
		//}
		}

		return false;
	}
}
