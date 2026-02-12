#include "SetObjectList.h"
#include "Context.h"
#include "Behaviors.h"
#include "Actions.h"
#include <ui/common/Icons.h>
#include <ui/resources/ResourceBrowser.h>
#include <utilities/ObjectDataUtils.h>

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, ObjectData* objData) : list{ list }, type{ SetObjectListTreeViewNode::Type::OBJECT }, object{ objData } {}
	SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, ObjectWorldChunkLayer* layer) : list{ list }, type{ SetObjectListTreeViewNode::Type::LAYER }, layer{ layer } {}
	SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, const char* label) : list{ list }, type{ SetObjectListTreeViewNode::Type::GROUP }, group{ label } {}
	SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectListTreeViewNode&& other) : list{ other.list }, type{ other.type } {
		switch (type) {
		case SetObjectListTreeViewNode::Type::OBJECT: object = other.object;
		case SetObjectListTreeViewNode::Type::LAYER: layer = other.layer;
		case SetObjectListTreeViewNode::Type::GROUP: group = other.group;
		}
	}

	const void* SetObjectListTreeViewNode::GetID() const {
		switch (type) {
		case SetObjectListTreeViewNode::Type::OBJECT: return object.object;
		case SetObjectListTreeViewNode::Type::LAYER: return layer.layer;
		case SetObjectListTreeViewNode::Type::GROUP: return group.label;
		default: return nullptr;
		}
	}

	const char* SetObjectListTreeViewNode::GetLabel() const {
		switch (type) {
		case SetObjectListTreeViewNode::Type::OBJECT: return GetObjectName(object.object);
		case SetObjectListTreeViewNode::Type::LAYER: return layer.layer->GetName();
		case SetObjectListTreeViewNode::Type::GROUP: return group.label;
		default: return nullptr;
		}
	}

	bool SetObjectListTreeViewNode::MatchesSearchString(const char* searchString) const
	{
		return strstr(GetLabel(), searchString);
	}

	bool SetObjectListTreeViewNode::IsSelected() const
	{
		auto* selectionBehavior = list.GetBehavior<SelectionBehavior<Context>>();

		return type == SetObjectListTreeViewNode::Type::OBJECT && selectionBehavior->IsSelected(object.object);
	}

	void SetObjectListTreeViewNode::PreRender() const
	{
		if (type == SetObjectListTreeViewNode::Type::OBJECT && !object.object->id.IsNonNull())
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });

		if (type == SetObjectListTreeViewNode::Type::LAYER && !layer.layer->IsEnable()) {
			auto textCol = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			textCol.w = 0.5;
			ImGui::PushStyleColor(ImGuiCol_Text, textCol);
		}
	}

	void SetObjectListTreeViewNode::PostRender() const
	{
		auto* selectionBehavior = list.GetBehavior<SelectionBehavior<Context>>();

		if (type == SetObjectListTreeViewNode::Type::OBJECT && !object.object->id.IsNonNull()) {
			ImGui::PopStyleColor();
			ImGui::SetItemTooltip("Invalid object ID: this object's ID is NULL, which is a reserved value!");
		}

		if (type == SetObjectListTreeViewNode::Type::LAYER && !layer.layer->IsEnable())
			ImGui::PopStyleColor();

		if (type == SetObjectListTreeViewNode::Type::OBJECT) {
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				selectionBehavior->Select(object.object);
			if (ImGui::BeginDragDropSource()) {
				ObjectData* obj = object.object;
				ImGui::SetDragDropPayload("ObjectData", &obj, sizeof(obj));
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
					ObjectData* parent = object.object;
					ObjectData* child = *static_cast<ObjectData**>(payload->Data);

					// Unparent an object
					if (child->parentID.IsNonNull() 
						&& child->parentID == parent->id) 
					{
						list.GetContext().SetObjectParent(child, nullptr);
						list.InvalidateTree();
					}
					// Parent an object
					else if (parent != child) {
						list.GetContext().SetObjectParent(child, parent);
						list.InvalidateTree();
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (type == SetObjectListTreeViewNode::Type::LAYER) {
			ImGui::PushID(GetID());
			if (ImGui::BeginPopupContextItem("Layer context menu")) {
				bool enabled{ layer.layer->IsEnable() };

				if (ImGui::MenuItem("Enabled", nullptr, &enabled))
					list.Dispatch(SetLayerEnabledAction{ { layer.layer->GetName(), enabled } });

				ImGui::Separator();

				if (ImGui::MenuItem("Load from file..."))
					ResourceBrowser::ShowLoadResourceDialog(layer.layer->GetResource());

				if (ImGui::MenuItem("Export..."))
					ResourceBrowser::ShowExportResourceDialog(layer.layer->GetResource());

				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
	}

	void SetObjectList::ProcessAction(const ActionBase& action)
	{
		switch (action.id) {
		case FocusedChunkChangedAction::id:
		case SceneChangedAction::id:
			InvalidateTree();
			break;
		}
	}

	void SetObjectList::RenderPanel() {
		auto focusedChunk = GetContext().GetFocusedChunk();
		auto* objWorld = GameManager::GetInstance()->GetService<ObjectWorld>();

		if (objWorld == nullptr) {
			ImGui::Text("ObjectWorld service not active. Load a level first.");
			return;
		}

		char chunkid[30];
		strcpy_s(chunkid, "<none>");

		int i = 0;
		for (auto* chunk : objWorld->GetWorldChunks())
			if (chunk == focusedChunk)
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i++);

		if (ImGui::BeginCombo("Chunk to be edited", chunkid)) {
			bool none_selected = focusedChunk == nullptr;
			strcpy_s(chunkid, "<none>");

			if (ImGui::Selectable(chunkid, none_selected))
				Dispatch(SetFocusedChunkAction{ nullptr });
			if (none_selected)
				ImGui::SetItemDefaultFocus();

			i = 0;
			for (auto* chunk : objWorld->GetWorldChunks()) {
				bool is_selected = focusedChunk == chunk;
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i++);

				if (ImGui::Selectable(chunkid, is_selected))
					Dispatch(SetFocusedChunkAction{ chunk });

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (!focusedChunk) ImGui::BeginDisabled();

		if (ImGui::Button("Export All")) {
			auto& layers = focusedChunk->GetLayers();

			csl::ut::MoveArray<hh::fnd::ManagedResource*> resources{ layers.size(), hh::fnd::GetTempAllocator() };

			for (auto* layer : layers) resources.push_back(layer->GetResource());

			ResourceBrowser::ShowExportResourceDialog(resources.begin(), resources.size());

			resources.clear();
		}

		if (!focusedChunk) ImGui::EndDisabled();

		if (dirty)
			RebuildTree();

		if (focusedChunk && &*tree && ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				tree->Render("Content");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				layerTree->Render("Content");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	PanelTraits ui::operation_modes::modes::level_editor::SetObjectList::GetPanelTraits() const
	{
		return { "World objects", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	size_t count = 0;

	TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildTreeNode(csl::ut::PointerMap<ObjectWorldChunkLayer*, ObjectMap<csl::ut::MoveArray<ObjectData*>>>& childMaps, ObjectData* objData) {
		auto objId = objData == nullptr ? ObjectId{} : objData->id;
		TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, objData } };

		// Some tools generate faulty objects with ID 0. Avoid crashing on those.
		if (objData == nullptr || objId.IsNonNull()) {
			for (auto* layer : GetContext().GetFocusedChunk()->GetLayers()) {
				auto childLayerMapIt = childMaps.Find(layer);
				auto childMapIt = childLayerMapIt->Find(objId);

				if (childMapIt == childLayerMapIt->end())
					continue;

				for (auto* object : *childMapIt)
					node.AddChild(BuildTreeNode(childMaps, object));
			}
		}

		return node;
	}

	TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildSingleLayerTreeNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer, ObjectData* objData) {
		TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, objData } };

		// Some tools generate faulty objects with ID 0. Avoid crashing on those.
		if (objData->id.IsNonNull()) {
			auto childMapIt = childMap.Find(objData->id);

			if (childMapIt == childMap.end())
				return node;

			for (auto* object : *childMapIt)
				node.AddChild(BuildSingleLayerTreeNode(childMap, layer, object));
		}

		return node;
	}

	TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildSingleLayerRootNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer) {
		TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, layer } };

		auto childMapIt = childMap.Find({});

		if (childMapIt == childMap.end())
			return node;

		for (auto* object : *childMapIt)
			node.AddChild(BuildSingleLayerTreeNode(childMap, layer, object));

		return node;
	}

	void SetObjectList::RebuildTree() {
		dirty = false;
		tree = nullptr;
		layerTree = nullptr;

		auto* focusedChunk = GetContext().GetFocusedChunk();

		if (focusedChunk != nullptr) {
			csl::ut::PointerMap<ObjectWorldChunkLayer*, ObjectMap<csl::ut::MoveArray<ObjectData*>>> childMaps{ GetAllocator() };

			for (auto* layer : focusedChunk->GetLayers()) {
				auto& childMap = *childMaps.InsertAndGet(layer, { GetAllocator() });

				for (auto* object : layer->GetResource()->GetObjects()) {
					auto parentIt = childMap.Find(object->parentID);

					if (parentIt == childMap.end())
						parentIt = childMap.InsertAndGet(object->parentID, { GetAllocator() });

					parentIt->push_back(object);
				}
			}

			tree = new (GetAllocator()) TreeView<SetObjectListTreeViewNode>(GetAllocator(), BuildTreeNode(childMaps, nullptr));
			layerTree = new (GetAllocator()) TreeView<SetObjectListTreeViewNode>(GetAllocator(), { GetAllocator(), { *this, "ROOT" } });

			for (auto* layer : focusedChunk->GetLayers())
				layerTree->rootNode.AddChild(BuildSingleLayerRootNode(*childMaps.Find(layer), layer));
		}
	}

	void SetObjectList::InvalidateTree() {
		dirty = true;
	}
}
