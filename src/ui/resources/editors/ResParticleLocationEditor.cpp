#include "ResParticleLocationEditor.h"
#include <ui/common/editors/ParticleLocation.h>

ResParticleLocationEditor::ResParticleLocationEditor(csl::fnd::IAllocator* allocator, hh::animeff::ResParticleLocation* resource) : StandaloneWindow{ allocator }, resource{ resource } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
}

ResParticleLocationEditor* ResParticleLocationEditor::Create(csl::fnd::IAllocator* allocator, hh::animeff::ResParticleLocation* resource) {
	return new (allocator) ResParticleLocationEditor{ allocator, resource };
}

void ResParticleLocationEditor::RenderContents() {
	auto* effdbData = static_cast<ucsl::resources::effdb::v100::EffdbData*>(resource->unpackedBinaryData);

	if (Editor(*effdbData, resource)) {
		resource->bindings.clear();
		resource->Load(effdbData, 0);
	}
}
