#pragma once
#include <utilities/CompatibleObject.h>
#include "TextureElement.h"
#include <optional>

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	class Context : public CompatibleObject {
		static hh::ui::ResSurfRideProject* GetResourceForComponent(hh::ui::GOCSprite* goc);
		unsigned short GenerateUniqueId();
		bool IsIdTaken(unsigned short id) const;
	public:
		using CompatibleObject::CompatibleObject;
		hh::fnd::Reference<hh::ui::GOCSprite> gocSprite{};
		std::optional<TextureRef> focusedTexture{};
		std::mt19937 mt{ std::random_device{}() };

		void AddTexture(const TextureListRef& textureListRef, hh::gfnd::ResTexture* textureResource);
		void ReplaceTexture(const TextureRef& textureRef, hh::gfnd::ResTexture* textureResource);
		void RemoveTexture(const TextureRef& textureRef);
		CropRef AddCrop(const TextureRef& textureRef, const ucsl::resources::swif::swif_version::SRS_CROP& crop);
		void RemoveCrop(const CropRef& cropRef);

		void RefreshAfterTextureUpdate(const TextureRef& textureRef);
		void RefreshCastAfterTextureUpdate(const TextureRef& textureRef, SurfRide::Cast* cast);
		static bool IsAffectedByTextureUpdate(const TextureRef& textureRef, ucsl::resources::swif::swif_version::SRS_CASTNODE& cast);
		void RefreshAfterCropUpdate(const CropRef& cropRef);
		void RefreshCastAfterCropUpdate(const CropRef& cropRef, SurfRide::Cast* cast);
		static bool IsAffectedByCropUpdate(const CropRef& cropRef, ucsl::resources::swif::swif_version::SRS_CASTNODE& cast);
	};
}
