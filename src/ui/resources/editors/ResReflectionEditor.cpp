#include "ResReflectionEditor.h"
#include <ucsl-reflection/reflections/resources/rfl/v1.h>
#include <ucsl-reflection/reflections/resources/rfl/v2.h>
#include <ucsl-reflection/accessors/binary.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/algorithms/copy.h>
#include <rip/binary/containers/binary-file/v2.h>
#include <rip/serialization/binary.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/viewers/RflDiff.h>
#include <ui/GlobalSettings.h>

using namespace hh::fnd;

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection* resource, const RflClass* rflClass) : StandaloneWindow{ allocator }, forcedRflClass{ true }, resource{ resource }, rflClass{ rflClass }, diffResult{allocator} {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx) - using RflClass %s", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)resource->GetData(), rflClass->GetName());
	SetTitle(namebuf);
}

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection* resource) : StandaloneWindow{ allocator }, forcedRflClass{ false }, resource{ resource }, rflClass{ nullptr }, diffResult{allocator} {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)resource->GetData());
	SetTitle(namebuf);
}

ResReflectionEditor::~ResReflectionEditor()
{
	FreeOriginalCopy();
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource, const hh::fnd::RflClass* rflClass) {
	return new (allocator) ResReflectionEditor(allocator, resource, rflClass);
}

void ResReflectionEditor::MakeOriginalCopy()
{
	origData = new (std::align_val_t(16), GetAllocator()) char[rflClass->GetSize()];
	hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(GetAllocator(), origData, rflClass->GetName());

	ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::RootType refl{ rflClass };
	ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> acc1{ origData, refl };
	ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> acc2{ resource->GetData(), refl };
	ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>::process_type(acc1, acc2);
}

void ResReflectionEditor::FreeOriginalCopy()
{
	if (origData) {
		hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(origData, rflClass->GetName());
		GetAllocator()->Free(origData);
	}
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource) {
	return new (allocator) ResReflectionEditor(allocator, resource);
}

void ResReflectionEditor::RenderContents() {
	bool clicked_export = false;
	bool clicked_showdiff = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Export", nullptr, &clicked_export);
		ImGui::MenuItem("Show diff", nullptr, &clicked_showdiff);
		ImGui::EndMenuBar();
	}

	if (clicked_showdiff)
		showdiff = !showdiff;

	if (!forcedRflClass) {
		const char* previewValue = rflClass == nullptr ? "<none>" : rflClass->GetName();
		csl::ut::MoveArray<const RflClass*> matchingRflClasses{ hh::fnd::MemoryRouter::GetTempAllocator() };

		ImGui::InputText("Search", searchStr, 200);
		for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
#ifdef DEVTOOLS_TARGET_SDK_miller
			auto resSize = resource->GetSize() - 0x10;
#else
			auto resSize = resource->GetSize();
#endif
			auto rflSize = rflc->GetSize();

			if ((resSize == rflSize || resSize == ((rflSize + 0xFF) & ~0xFF)) && (strlen(searchStr) == 0 || strstr(rflc->GetName(), searchStr)))
				matchingRflClasses.push_back(rflc);
		}

		if (ImGui::BeginCombo("RflClass", previewValue)) {
			if (matchingRflClasses.size() > 0) {
				for (auto* rflc : matchingRflClasses) {
					bool is_selected = rflClass == rflc;

					if (ImGui::Selectable(rflc->GetName(), is_selected)) {
						FreeOriginalCopy();
						rflClass = rflc;
						MakeOriginalCopy();
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}
			else {
				for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
					bool is_selected = rflClass == rflc;

					if (is_selected || strlen(searchStr) == 0 || strstr(rflc->GetName(), searchStr)) {
						if (ImGui::Selectable(rflc->GetName(), is_selected)) {
							FreeOriginalCopy();
							rflClass = rflc;
							MakeOriginalCopy();
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	}

	if (rflClass != nullptr) {
		if (showdiff) {
			if (ImGui::BeginChild("Properties", ImVec2(ImGui::GetWindowSize().x / 2, 0))) {
				if (ResettableReflectionEditor("Properties", resource->GetData(), origData, rflClass))
					diffResult = std::move(RflDiffStruct(GetAllocator(), origData, resource->GetData(), rflClass));
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Diff", ImVec2(ImGui::GetWindowSize().x / 2, 0))) {
				if (ImGui::BeginTabBar("Diff Views")) {
					if (ImGui::BeginTabItem("Tree view")) {
						Viewer("Diff", diffResult, origData, rflClass);
						ImGui::EndTabItem();
					}
					//if (ImGui::BeginTabItem("RFL2HMM")) {
					//	Rfl2HmmViewer("Diff", diffResult, origData, rflClass);
					//	ImGui::EndTabItem();
					//}
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();
		}
		else {
			if (ImGui::BeginChild("Properties")) {
				ResettableReflectionEditor("Properties", resource->GetData(), origData, rflClass);
			}
			ImGui::EndChild();
		}

		if (clicked_export) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = this;
			ImGuiFileDialog::Instance()->OpenDialog("ResReflectionExportDialog", "Choose File", ".rfl", cfg);
		}

		RenderExportDialog();
	}
}

thread_local const RflClass* gRflClass{};
inline std::string get_rfl_class() { return gRflClass->GetName(); }

void ResReflectionEditor::RenderExportDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ResReflectionExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* self = static_cast<ResReflectionEditor*>(ImGuiFileDialog::Instance()->GetUserDatas());

			gRflClass = self->rflClass;

#if DEVTOOLS_TARGET_SDK_miller
			ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::rfl::v2::reflections::Ref2Data<ucsl::resources::rfl::v2::Ref2RflData, simplerfl::selector_resolver<std::string>::impl<get_rfl_class>>> refl{};
			ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueConstAccessor<decltype(refl)> acc{ self->resource->ManagedResource::GetData(), refl };
#else
			ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::rfl::v1::reflections::Ref1Data<ucsl::resources::rfl::v1::Ref1RflData, simplerfl::selector_resolver<std::string>::impl<get_rfl_class>>> refl{};
			ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueConstAccessor<decltype(refl)> acc{ self->resource->unpackedBinaryData, refl };
#endif

			rip::models::BinaryFileV2<decltype(refl), he2sdk::ucsl::GameInterface::AllocatorSystem> model{ rip::binary::serializeBinaryToAllocatorSystemBuffer<he2sdk::ucsl::GameInterface::AllocatorSystem, size_t>(acc) };

			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::fast_ostream fos{ ofs };
			rip::binary::binary_ostream<rip::binary::fast_ostream, size_t> bos{ fos };

			rip::serialization::binary<decltype(model)>::save(bos, model);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}