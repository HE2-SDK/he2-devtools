#include "RflComparer.h"

using namespace hh::fnd;

RflComparer::RflComparer(csl::fnd::IAllocator* allocator) : StandaloneWindow(allocator)
{
	SetTitle("RFL Comparer");
}

void RflComparer::RenderContents()
{
	if (ImGui::BeginChild("DropRegion1", ImVec2(600, 50))) {
		if (resource1 == nullptr) {
			ImGui::Text("Drag an RFL resource here.");
		}
		else {
			ImGui::Text("Linked to resource \"%s\" (ResReflection @ 0x%zx)", resource1->GetName(), &*resource1);
		}
	}
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection::GetTypeInfo())
				resource1 = static_cast<ResReflection*>(res);
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginChild("DropRegion2", ImVec2(600, 50))) {
		if (resource2 == nullptr) {
			ImGui::Text("Drag an RFL resource here.");
		}
		else {
			ImGui::Text("Linked to resource \"%s\" (ResReflection @ 0x%zx)", resource2->GetName(), &*resource2);
		}
	}
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection::GetTypeInfo())
				resource2 = static_cast<ResReflection*>(res);
		}
		ImGui::EndDragDropTarget();
	}

	if (!resource1 || !resource2)
		return;

	//if (resource1->GetSize() != resource2->GetSize()) {
	//	ImGui::Text("Resources are not same size!");
	//	return;
	//}

	ImGui::InputText("Search", searchStr, 200);
	auto searchLen = strlen(searchStr);
	const char* previewValue = rflClass == nullptr ? "<none>" : rflClass->GetName();

	if (searchLen != 0) {
		for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
			for (auto* h = rflc->GetName(); *h; h++) {
				if (!_strnicmp(h, searchStr, searchLen)) {
					if (ImGui::Selectable(rflc->GetName(), rflClass == rflc)) {
						rflClass = rflc;
						diffResult = RflDiffStruct(GetAllocator(), resource1->GetData(), resource2->GetData(), rflc);
					}
					break;
				}
			}
		}
	}
	else if (ImGui::BeginCombo("RflClass", previewValue)) {
		for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
			//if (resource1->GetSize() == rflc->GetSizeInBytes() || resource2->GetSize() == rflc->GetSizeInBytes()) {
				bool is_selected = rflClass == rflc;

				if (ImGui::Selectable(rflc->GetName(), is_selected)) {
					rflClass = rflc;
					diffResult = RflDiffStruct(GetAllocator(), resource1->GetData(), resource2->GetData(), rflc);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			//}
		}

		ImGui::EndCombo();
	}

	if (!rflClass)
		return;

	for (auto& change : diffResult.changes) {
		ImGui::Text(change.type == RflDiffChange::Type::ADD ? "ADD: " : change.type == RflDiffChange::Type::REMOVE ? "REMOVE: " : "UPDATE: ");
		ImGui::SameLine();
		for (size_t i = change.path.size(); i > 0; i--) {
			auto& pathEl = change.path[i - 1];
			if (pathEl.type == RflDiffChange::Index::Type::INDEX) {
				ImGui::Text("%d", pathEl.arrayIdx);
			}
			else {
				ImGui::Text("%s", pathEl.propertyName);
			}
			ImGui::SameLine();
		}
		ImGui::Text("");
	}
}
