#include "PageEditor.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <utilities/NameHash.h>

namespace ui::operation_modes::modes::dvscene_editor {
    namespace NodeEd = ax::NodeEditor;

	ImVec4 NodeEditorInterface::GetColor(PinType type)
	{
		switch (type) {
		case PinType::TRANSITION: return { 0.0f, 0.5f, 0.8f, 1.0f };
		default: assert(false); return { 0.0f, 0.0f, 0.0f, 0.0f };
		}
	}

	void NodeEditorInterface::RenderPinIcon(PinType type) {
		auto size = ImGui::GetTextLineHeight();
		auto color = ImGui::GetColorU32(GetColor(type));

		ImGui::Dummy({ size, size });

		auto itemRectMin = ImGui::GetItemRectMin();
		auto itemRectMax = ImGui::GetItemRectMax();

		switch (type) {
		case PinType::TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		default: {
			assert(false);
			break;
		}
		}
	}

    PageEditor::PageEditor(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, nodeEditor{ allocator } {}

    void PageEditor::RenderPanel() {
		auto& context = GetContext();
        if (context.goDVSC == nullptr) {
            ImGui::Text("No DvSceneControl selected");
			return;
		}
		if (ImGui::Selectable("Auto Layout"))
			nodeEditor.RunAutoLayout();

		nodeEditor.Begin();

		for (int pageIdx = 0; pageIdx < context.dvPages.size(); pageIdx++)
			if (RenderPage(pageIdx))
				context.dvPages[pageIdx]->UpdateRuntimePage();
		for (int pageIdx = 0; pageIdx < context.dvPages.size(); pageIdx++) {
			auto* page = context.dvPages[pageIdx];
			for (int transIdx = 0; transIdx < page->filePage->transition.size(); transIdx++)
				for (int condIdx = 0; condIdx < page->filePage->transition[transIdx].conditions.size(); condIdx++)
					if(RenderCondition(pageIdx, transIdx, condIdx))
						page->UpdateRuntimePage();
		}
		for (int pageIdx = 0; pageIdx < context.dvPages.size(); pageIdx++)
			for (int trans = 0; trans < context.dvPages[pageIdx]->filePage->transition.size(); trans++)
				RenderTransition(pageIdx, trans);

		/*if (ax::NodeEditor::ShowBackgroundContextMenu()) {
			ax::NodeEditor::Suspend();
			ImGui::OpenPopup("Page Context Options");
			ax::NodeEditor::Resume();
		}

		ax::NodeEditor::Suspend();
		if (ImGui::BeginPopup("Page Context Options")) {
			if (ImGui::MenuItem("Create Page")) {
				char pageName[32] = "";
				srand(time(nullptr));
				sprintf(pageName, "Page%d", static_cast<unsigned int>(rand()));
				dv::DvPage filePage = context.CreatePage(pageName, context.dvPages.size());
				hh::dv::DvPage* runtimePage = context.CreatePage(pageName, context.dvPages.size(), context.goDVSC);
				context.parsedScene->dvCommon->pages.push_back(filePage);
				context.goDVSC->timeline->pages.push_back(runtimePage);
				context.CreateWrapperPages();
			}
			ImGui::EndPopup();
		}
		ax::NodeEditor::Resume();

		if (ax::NodeEditor::BeginCreate()) {
			OutputPinId inPin;
			InputPinId outPin;

			if (QueryNewLink(inPin, outPin)) {
				if (inPin.nodeId.type == NodeType::PAGE && outPin.nodeId.type == NodeType::CONDITION) {
					DvPage* page = context.dvPages[inPin.nodeId.idx];
				}
				else if ((static_cast<unsigned int>(inPin.nodeId.type) & static_cast<unsigned int>(NodeType::CONDITION)) != 0 && outPin.nodeId.type == NodeType::PAGE)
					for (auto* page : context.dvPages) {
						for (int x = 0; x < page->filePage->transition.size(); x++) {
							auto& trans = page->filePage->transition[x];
							for (int i = 0; i < trans.conditions.size(); i++)
							{
								if (inPin.nodeId.idx == static_cast<int>(name_hash(page->filePage->name) | page->filePage->index | static_cast<int>(page->filePage->skipFrame) | x)) {
									trans.destinationPageID = outPin.nodeId.idx;
									context.CreateWrapperPages();
									break;
								}
							}
						}
					}
			}
		}

		ax::NodeEditor::EndCreate();*/
		nodeEditor.End();
    }

    bool PageEditor::RenderPage(int pageIdx) {
		bool changed = false;
		auto& context = GetContext();
		auto* page = context.dvPages[pageIdx];
        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, {0.5f, 0.5f, 0.5f, 1.0f});
		NodeId nodeId{ NodeType::PAGE, pageIdx };

		nodeEditor.BeginNode(nodeId, ImGui::CalcTextSize(page->filePage->name).x);

		nodeEditor.BeginTitle();
		ImGui::Text("Page");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		InputPinId inpin{ nodeId, PinType::TRANSITION, 0 };
        nodeEditor.BeginInputPin(inpin, PinType::TRANSITION);
        nodeEditor.EndInputPin();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		float progress = (static_cast<float>(context.goDVSC->timeline->preCurrentFrame/100) - page->filePage->frameStart) / (page->filePage->frameEnd - page->filePage->frameStart);
		if (context.goDVSC->timeline->currentPage == pageIdx)
			ImGui::ProgressBar(progress, {150, 0});
		int start = static_cast<int>(page->filePage->frameStart);
		if (changed |= Editor("Start", start))
			page->filePage->frameStart = static_cast<float>(start);
		int end = static_cast<int>(page->filePage->frameEnd);
		if (changed |= Editor("End", end))
			page->filePage->frameEnd = static_cast<float>(end);
		int skipFrame = static_cast<int>(page->filePage->skipFrame);
		if(changed |= Editor("Skip Frame", skipFrame))
			page->filePage->skipFrame = static_cast<float>(skipFrame);
        changed |= Editor("Page Name", page->filePage->name);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPinId outpin{ nodeId, PinType::TRANSITION, 0 };
        nodeEditor.BeginOutputPin(outpin, 0.0f, PinType::TRANSITION);
        nodeEditor.EndOutputPin();
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();

		ax::NodeEditor::PopStyleColor();
		return changed;
    }

	bool PageEditor::RenderCondition(int pageIdx, int transitionIdx, int conditionIdx)
	{
		bool changed = false;
		auto& context = GetContext();
		auto* page = context.dvPages[pageIdx]->filePage;
		auto& condition = page->transition[transitionIdx].conditions[conditionIdx];
		ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, { 0.5f, 0.5f, 0.5f, 1.0f });
		NodeId nodeId{ NodeType::CONDITION, static_cast<int>(name_hash(page->name) | pageIdx | static_cast<int>(page->skipFrame) | transitionIdx) };

		nodeEditor.BeginNode(nodeId, ImGui::CalcTextSize("Condition").x);

		nodeEditor.BeginTitle();
		ImGui::Text("Condition");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		InputPinId inpin{ nodeId, PinType::TRANSITION, 0 };
		nodeEditor.BeginInputPin(inpin, PinType::TRANSITION);
		nodeEditor.EndInputPin();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		Viewer("Type", static_cast<int>(condition.type) == 1000 ? "QTE" : "Page End");
		switch (condition.type) {
		case dv::DvCondition::ConditionType::QTE:
			auto* qteCond = reinterpret_cast<app::dv::DvPageConditionQTE::Description*>(condition.data);
			changed |= Editor("Failed", qteCond->failed);
			break;
		}
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPinId outpin{ nodeId, PinType::TRANSITION, 0 };
		nodeEditor.BeginOutputPin(outpin, 0.0f, PinType::TRANSITION);
		nodeEditor.EndOutputPin();
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();

		ax::NodeEditor::PopStyleColor();
		return changed;
	}

	void PageEditor::RenderTransition(int pageIdx, int transitionIdx)
	{
		auto& ctx = GetContext();
		auto* page = ctx.dvPages[pageIdx]->filePage;
		auto& trans = page->transition[transitionIdx];
		for (int cond = 0; cond < trans.conditions.size(); cond++) {
			auto x = OutputPinId{ { NodeType::PAGE, pageIdx }, PinType::TRANSITION, 0 };
			auto y = InputPinId{ { NodeType::CONDITION, static_cast<int>(name_hash(page->name) | pageIdx | static_cast<int>(page->skipFrame) | transitionIdx) }, PinType::TRANSITION, 0 };
			Link(x, y);
			x = OutputPinId{ { NodeType::CONDITION, static_cast<int>(name_hash(page->name) | pageIdx | static_cast<int>(page->skipFrame) | transitionIdx) }, PinType::TRANSITION, 0 };
			y = InputPinId{ { NodeType::PAGE, trans.destinationPageID }, PinType::TRANSITION, 0 };
			Link(x, y);
		}
	}

    PanelTraits PageEditor::GetPanelTraits() const
	{
		return { "Page Editor", ImVec2(0, 0), ImVec2(250, 250) };
	}

    NodeId::NodeId(NodeType type, int idx) : type{ type }, idx{ idx } {}

	NodeId::NodeId(unsigned long long nodeId)
	{
		type = static_cast<NodeType>((nodeId >> 16) & 0xF);
		idx = nodeId & 0xFFFFFFFF;
	}

	NodeId::NodeId(ax::NodeEditor::NodeId nodeId) : NodeId(nodeId.Get()) {}

	NodeId::operator unsigned long long() const
	{
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lType << 16) | lIdx;
	}

	NodeId::operator ax::NodeEditor::NodeId() const
	{
		return static_cast<unsigned long long>(*this);
	}

	PinId::PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, int idx) : nodeId{ nodeId }, kind{ kind }, type{ type }, idx{ idx } {}

	PinId::PinId(unsigned long long pinId)
	{
		ax::NodeEditor::NodeId lNodeId = (pinId >> 12) & 0xFFFFF;

		nodeId = lNodeId;
		kind = static_cast<ax::NodeEditor::PinKind>((pinId >> 11) & 0x1);
		type = static_cast<PinType>((pinId >> 7) & 0xF);
		idx = pinId & 0x7F;
	}

	PinId::PinId(ax::NodeEditor::PinId pinId) : PinId(pinId.Get()) {}

	PinId::operator unsigned long long() const
	{
		unsigned long long lNodeId = nodeId;
		unsigned long long lKind = static_cast<unsigned long long>(kind);
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lNodeId << 12) | (lKind << 11) | (lType << 7) | lIdx;
	}

	PinId::operator ax::NodeEditor::PinId() const
	{
		return static_cast<unsigned long long>(*this);
	}

    InputPinId::InputPinId(const NodeId& nodeId, PinType type, int idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Input, type, idx } {}
	InputPinId::InputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	InputPinId::InputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	OutputPinId::OutputPinId(const NodeId& nodeId, PinType type, int idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Output, type, idx } {}
	OutputPinId::OutputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	OutputPinId::OutputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	ax::NodeEditor::LinkId PageEditor::GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId) {
		unsigned long long lFromPinId = fromPinId.Get();
		unsigned long long lToPinId = toPinId.Get();

		return (lFromPinId << 32) | lToPinId;
	}

	void PageEditor::Link(const OutputPinId& fromPin, const InputPinId& toPin)
	{
		nodeEditor.Link(GetLinkId(fromPin, toPin), fromPin, toPin, fromPin.type);
	}

	bool PageEditor::QueryNewLink(OutputPinId& startPinId, InputPinId& endPinId)
	{
		ax::NodeEditor::PinId startPin, endPin;

		bool result = ax::NodeEditor::QueryNewLink(&startPin, &endPin);

		if (!result || !startPin || !endPin)
			return false;

		PinId iStartPin = startPin;
		PinId iEndPin = endPin;

		if (iStartPin.kind == iEndPin.kind)
			return false;

		startPinId = iStartPin.kind == ax::NodeEditor::PinKind::Output ? OutputPinId{ static_cast<unsigned long long>(iStartPin) } : OutputPinId{ static_cast<unsigned long long>(iEndPin) };
		endPinId = iStartPin.kind == ax::NodeEditor::PinKind::Input ? InputPinId{ static_cast<unsigned long long>(iStartPin) } : InputPinId{ static_cast<unsigned long long>(iEndPin) };

		return true;
	}
}