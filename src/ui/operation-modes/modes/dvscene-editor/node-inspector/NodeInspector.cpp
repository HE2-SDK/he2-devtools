#include "../Behaviors.h"
#include "NodeInspector.h"
#include "../DvNode.h"
#include "Nodes.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>
#include <dieventlib/dvnode.h>

namespace ui::operation_modes::modes::dvscene_editor {
	bool NodeInspector::NodeEditor(dv::DvNode* node){
		auto& ctx = GetContext();

		bool changed = false;

		char guidBuffer[37];
		unsigned char guid[16];
		memcpy(guid, reinterpret_cast<void*>(&node->guid), 16);
		sprintf(guidBuffer,
			"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid[0], guid[1], guid[2], guid[3],
			guid[4], guid[5],
			guid[6], guid[7],
			guid[8], guid[9],
			guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);

		ImGui::Text("GUID: {%s}", guidBuffer);

		if (ImGui::BeginPopupContextItem("Options")) {
			if (changed |= ImGui::Selectable("Generate GUID")) 
				GetContext().GenerateGUID(reinterpret_cast<char*>(&node->guid));
			ImGui::EndPopup();
		}

		changed |= Editor("Node Name", node->name);

		auto type = static_cast<hh::dv::DvNodeBase::NodeType>(node->category);
		Viewer("Node Type", GetNodeName(static_cast<unsigned int>(type)));

		if (type == hh::dv::DvNodeBase::NodeType::CHARACTER        ||
			type == hh::dv::DvNodeBase::NodeType::MODEL			   ||
			type == hh::dv::DvNodeBase::NodeType::CAMERA_MOTION    ||
			type == hh::dv::DvNodeBase::NodeType::MODEL_MOTION     ||
			type == hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION) {
			auto* resource = ctx.GetResourceByFileNode(node);
			char curRes[192] = "NULL";
			if (resource)
				strcpy(curRes, resource->filename);
			if (Editor("Resource filename", curRes))
			{
				if (!resource) {
					dv::Resource res{};
					memcpy(&res.guid, &node->guid, 16);
					switch (type) {
					case hh::dv::DvNodeBase::NodeType::CHARACTER:
						res.type = dv::Resource::Type::Character;
						break;
					case hh::dv::DvNodeBase::NodeType::MODEL:
						res.type = dv::Resource::Type::Model;
						break;
					case hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION:
					case hh::dv::DvNodeBase::NodeType::MODEL_MOTION:
						res.type = dv::Resource::Type::CharacterMotion;
						break;
					case hh::dv::DvNodeBase::NodeType::CAMERA_MOTION:
						res.type = dv::Resource::Type::CameraMotion;
						break;
					}
					res.flags0 = 0;
					res.flags1 = 1;
					strcpy(curRes, res.filename);
					ctx.parsedScene->dvResource.push_back(res);
					resource = &res;
				}
				if (resource) {
					memset(resource->filename, 0, sizeof(resource->filename));
					strcpy(resource->filename, curRes);
				}
			}
		}

		if (IsUnknownNode(node->category))
			return changed;

		NodeFuncType render = GetNodeInspectorRender(static_cast<hh::dv::DvNodeBase::NodeType>(node->category));
		if (render){
			ImGui::SeparatorText("Properties");
			changed |= render(node->data);
		}

		return changed;
	}

	void NodeInspector::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr) {
			ImGui::Text("No DvSceneControl selected");
			return;
		}

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0)
			ImGui::Text("Select an item in the Node Tree.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple items selected.");
		else {
			DvNode focusedNode = selection[0];
			dv::DvNode* dvNode = focusedNode.fileNode;
			if(memcmp(&dvNode->guid, focusedNode.node->guid, 16) == 0)
				if (NodeEditor(dvNode))
					focusedNode.UpdateRuntimeNode();
		}
	}

	PanelTraits NodeInspector::GetPanelTraits() const
	{
		return { "Node Inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 200) };
	}
}
