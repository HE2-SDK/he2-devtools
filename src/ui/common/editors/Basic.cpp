#include "Basic.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/Desktop.h>

bool Editor(const char* label, hh::fnd::WorldPosition& worldPos) {
    bool edited{};
    ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", worldPos.m_Position);
	edited |= Editor("Rotation", worldPos.m_Rotation);
	ImGui::EndGroup();
    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, csl::math::Transform& transform) {
    bool edited{};
    ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", transform.position);
	edited |= Editor("Rotation", transform.rotation);
	edited |= Editor("Scale", transform.scale);
	ImGui::EndGroup();
    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, hh::game::ObjectId& id) {
    bool edited{};
	auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

	if (objWorld == nullptr) {
		Viewer(label, id);
	}
	else {
		char unkNameBuf[50];
#ifdef DEVTOOLS_TARGET_SDK_wars
		sprintf_s(unkNameBuf, 50, "<%08x>", id.id);
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
		sprintf_s(unkNameBuf, 50, "<%016zx%016zx>", id.groupId, id.objectId);
#endif

		const char* name = unkNameBuf;

		for (auto* chunk : objWorld->GetWorldChunks()) {
			int idx = chunk->GetObjectIndexById(id);

			if (idx == -1) ImGui::BeginDisabled();
			if (ImGui::Button("Select"))
				Desktop::instance->Dispatch(FocusObjectDataAction{ chunk->GetWorldObjectStatusByIndex(idx).objectData });
			if (idx == -1) ImGui::EndDisabled();

			ImGui::SameLine();

			if (idx != -1)
				name = chunk->GetWorldObjectStatusByIndex(idx).objectData->name;
		}

		bool isOpen = ImGui::BeginCombo(label, name);

		if (ImGui::BeginDragDropTarget()) {
			if (auto* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
				auto* objData = *static_cast<hh::game::ObjectData**>(payload->Data);

				id = objData->id;
				edited = true;
			}
			ImGui::EndDragDropTarget();
		}

		if (isOpen) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				for (auto* layers : chunk->GetLayers()) {
					for (auto* obj : layers->GetResource()->GetObjects()) {
						if (ImGui::Selectable(obj->name)) {
							id = obj->id;
							edited = true;
						}

						if (id == obj->id)
							ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	}
    
    return edited;
}

bool Editor(const char* label, ucsl::objectids::ObjectIdV1& id) {
	return Editor(label, (hh::game::ObjectId&)id);
}

bool Editor(const char* label, ucsl::objectids::ObjectIdV2& id) {
	return Editor(label, (hh::game::ObjectId&)id);
}

bool Editor(const char* label, hh::game::GameObject*& gameObject)
{
	bool edited{};

	ImGui::PushID(label);
	if (gameObject == nullptr) ImGui::BeginDisabled();
	if (ImGui::Button("Select"))
		Desktop::instance->Dispatch(FocusGameObjectAction{ gameObject });
	if (gameObject == nullptr) ImGui::EndDisabled();

	ImGui::SameLine();

	if (ImGui::BeginCombo(label, gameObject == nullptr ? "<none>" : gameObject->name.c_str())) {
		for (auto* obj : hh::game::GameManager::GetInstance()->objects) {
			if (ImGui::Selectable(obj->name)) {
				gameObject = obj;
				edited = true;
			}

			if (gameObject == obj)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, csl::ut::VariableString& str) {
    return InputText(label, str);
}

bool Editor(const char* label, bool& obj)
{
	return ImGui::Checkbox(label, &obj);
}

bool Editor(const char* label, csl::ut::String& str) {
    return InputText(label, str);
}

