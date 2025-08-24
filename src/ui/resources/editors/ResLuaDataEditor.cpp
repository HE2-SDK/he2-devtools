#include "ResLuaDataEditor.h"

ResLuaDataEditor::ResLuaDataEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResLuaData* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
}

ResLuaDataEditor* ResLuaDataEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResLuaData* resource) {
	return new (allocator) ResLuaDataEditor(allocator, resource);
}

void ResLuaDataEditor::RenderContents()
{
}
