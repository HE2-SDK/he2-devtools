#pragma once
#include <imgui_node_editor.h>
#include <ui/common/NodeEditorAutoLayout.h>
#include <utilities/CompatibleObject.h>

template<typename NodeEditorInterface>
class NodeEditor : public CompatibleObject {
	ax::NodeEditor::EditorContext* context{};
	NodeEditorAutoLayout autoLayout{ GetAllocator(), context };
	//csl::ut::PointerMap<ax::NodeEditor::NodeId, float> outputPinWidths{ GetAllocator() };
	ax::NodeEditor::NodeId currentNode{};
	ax::NodeEditor::PinId currentPin{};
	typename NodeEditorInterface::PinType currentPinType{};
	float currentNodeMaxOutputPinLabelWidth{};

public:
	NodeEditor(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
		ax::NodeEditor::Config config{};
		context = ax::NodeEditor::CreateEditor(&config);
	}

	virtual ~NodeEditor() {
		ax::NodeEditor::DestroyEditor(context);
	}

	void RunAutoLayout() {
		autoLayout.Recalculate();
	}

	void BeginContext() {
		ax::NodeEditor::SetCurrentEditor(context);
	}

	void EndContext() {
		ax::NodeEditor::SetCurrentEditor(nullptr);
	}

	void Begin() {
		ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_FlowDuration, 0.25f);
		ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_FlowMarkerDistance, 45.0f);
		ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_FlowSpeed, 300.0f);
		ax::NodeEditor::Begin("Editor");
		autoLayout.Begin();
	}

	void End() {
		autoLayout.End();
		ax::NodeEditor::End();
		ax::NodeEditor::PopStyleVar(3);
	}

	void BeginNode(ax::NodeEditor::NodeId nodeId, float maxOutputPinLabelWidth) {
		ax::NodeEditor::BeginNode(nodeId);
		ImGui::PushID(reinterpret_cast<void*>(nodeId.Get()));
		ImGui::PushItemWidth(100.0f * ImGui::GetFontSize() / 14.0f);

		currentNode = nodeId;
		currentNodeMaxOutputPinLabelWidth = maxOutputPinLabelWidth;
	}

	void EndNode() {
		ImGui::PopItemWidth();
		ImGui::PopID();

		ax::NodeEditor::EndNode();
		autoLayout.AddNode(currentNode);
	}

	void BeginTitle() {
		ImGui::BeginGroup();
	}

	void EndTitle() {
		ImGui::EndGroup();
	}

	//void SetNodeOutputPinWidth(float width) {
	//	outputPinWidths.Insert(currentNode, width);
	//}

	void BeginInputPins() {
		ImGui::BeginGroup();
	}

	void EndInputPins() {
		ImGui::EndGroup();
	}

	void BeginControls() {
		ImGui::SameLine();
		ImGui::BeginGroup();
	}

	void EndControls() {
		ImGui::EndGroup();
	}

	void BeginOutputPins() {
		ImGui::SameLine();
		ImGui::BeginGroup();
	}

	void EndOutputPins() {
		ImGui::EndGroup();

		//outputPinWidths.Insert(currentNode, ImGui::GetItemRectSize().x);
	}

	void BeginInputPin(ax::NodeEditor::PinId id, typename NodeEditorInterface::PinType type) {
		ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);
		ax::NodeEditor::PinPivotAlignment({ 0.0f, 0.5f });
		NodeEditorInterface::RenderPinIcon(type);
		ImGui::SameLine();
	}

	void EndInputPin() {
		ax::NodeEditor::EndPin();
	}

	void BeginOutputPin(ax::NodeEditor::PinId id, float labelWidth, typename NodeEditorInterface::PinType type) {
		//auto pinWidth = outputPinWidths.GetValueOrFallback(currentNode, 10.0f);
		ImGui::Dummy({ currentNodeMaxOutputPinLabelWidth - labelWidth, 0.0f });
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, NodeEditorInterface::GetColor(type));
		currentPin = id;
		currentPinType = type;
	}

	void EndOutputPin() {
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ax::NodeEditor::BeginPin(currentPin, ax::NodeEditor::PinKind::Output);
		NodeEditorInterface::RenderPinIcon(currentPinType);
		ax::NodeEditor::EndPin();
	}

	void Link(ax::NodeEditor::LinkId linkId, ax::NodeEditor::PinId from, ax::NodeEditor::PinId to, typename NodeEditorInterface::PinType fromPinType) {
		ax::NodeEditor::Link(linkId, from, to, NodeEditorInterface::GetColor(fromPinType));
	}

	void LayoutLink(ax::NodeEditor::NodeId from, ax::NodeEditor::NodeId to) {
		autoLayout.AddLink(from, to);
	}

	void Flow(ax::NodeEditor::LinkId linkId, ax::NodeEditor::FlowDirection direction = ax::NodeEditor::FlowDirection::Forward) {
		ax::NodeEditor::Flow(linkId, direction);
	}
};
