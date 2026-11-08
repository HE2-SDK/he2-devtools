#include "MemoryInspector.h"

using namespace csl::fnd;
using namespace hh::fnd;

const char* unitNames[4] = { "B", "kiB", "MiB", "GiB" };

void DataSizeText(size_t value, size_t total, int unit) {
	if (unit == 0)
		ImGui::Text("%zd (%d%%)", value, value * 100 / total);
	else
		ImGui::Text("%.2f (%d%%)", static_cast<float>(value) / (1 << 10 * unit), value * 100 / total);
}

HeapInspector::HeapInspector(IAllocator* allocator, MemoryInspector* memoryInspector, HeapBase* target) : CompatibleObject{ allocator }, memoryInspector{ memoryInspector }, target { target }
{
}

void HeapInspector::Tick() {
#ifndef DEVTOOLS_TARGET_SDK_miller
	HeapInformation info{};
	target->CollectHeapInformation(info);

	heapInfoAllocationsHistory[nextFrame] = info.liveAllocations;
	heapInfoTotalFreeSizeHistory[nextFrame] = info.freeSize.totalFreeSize;
	heapInfoMaxFreeSizeHistory[nextFrame] = info.freeSize.maxFreeSize;
#endif


#ifndef DEVTOOLS_TARGET_SDK_wars
	HeapStatistics stats{};
	target->GetStatistics(stats);

	// We do this separately so that the frames on all inspectors line up even if they aren't being rendered.
	statisticsAllocationsHistory[nextFrame] = stats.liveAllocations;
	statisticsAllocatedHistory[nextFrame] = stats.allocated;
	statisticsUsedHistory[nextFrame] = stats.used;
#endif

	nextFrame = (nextFrame + 1) % numSamples;

	for (auto i = 0; i < childHeapInspectors.size(); i++) {
		bool found{ false };

		for (auto& child : target->GetChildren()) {
			if (childHeapInspectors[i]->target == &child) {
				found = true;
				break;
			}
		}

		if (!found)
			childHeapInspectors.remove(i);
	}

	for (auto& child : target->GetChildren()) {
		bool found{ false };

		for (auto& inspector : childHeapInspectors) {
			if (inspector->target == &child) {
				found = true;
				break;
			}
		}
		
		if (!found)
			childHeapInspectors.push_back(new (GetAllocator()) HeapInspector{ GetAllocator(), memoryInspector, &child });
	}

	for (auto& inspector : childHeapInspectors)
		inspector->Tick();
}

#ifndef DEVTOOLS_TARGET_SDK_miller
void HeapInspector::RenderHeapInformation()
{
	auto* name = target->GetName();

	HeapInformation info{};
	target->CollectHeapInformation(info);

	ImGui::PushID(target);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto isOpen = ImGui::TreeNodeEx("Node", ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | (childHeapInspectors.empty() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0), "%s", name);

	ImGui::TableNextColumn();
	DataSizeText(info.bufferSize, info.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(info.freeSize.totalFreeSize, info.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(info.freeSize.maxFreeSize, info.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	ImGui::Text("%d", info.liveAllocations);
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Free memory", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Memory size", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, static_cast<double>(info.bufferSize), ImPlotCond_Always);
		ImPlot::SetNextLineStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::PlotLine("Free", heapInfoTotalFreeSizeHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::PlotLine("Max free", heapInfoMaxFreeSizeHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Activity", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Allocations", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y2, "Used", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Opposite);
		ImPlot::SetNextLineStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
		ImPlot::PlotLine("Live allocations", heapInfoAllocationsHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
		ImPlot::PlotLine("Free", heapInfoTotalFreeSizeHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}

	if (!childHeapInspectors.empty() && isOpen) {
		for (auto& inspector : childHeapInspectors)
			inspector->RenderHeapInformation();

		ImGui::TreePop();
	}

	ImGui::PopID();
}
#endif

#ifndef DEVTOOLS_TARGET_SDK_wars
void HeapInspector::RenderStatistics()
{
	auto* name = target->GetName();

	HeapStatistics stats{};
	target->GetStatistics(stats);

	ImGui::PushID(target);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto isOpen = ImGui::TreeNodeEx("Node", ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | (childHeapInspectors.empty() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0), "%s", name);

	ImGui::TableNextColumn();
	DataSizeText(stats.bufferSize, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.used, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.allocated, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.liveAllocations);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.totalAllocations);
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Usage", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Memory size", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, static_cast<double>(stats.bufferSize), ImPlotCond_Always);
		ImPlot::SetNextLineStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::PlotLine("Allocated", statisticsAllocatedHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::PlotLine("Used", statisticsUsedHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Activity", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Allocations", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y2, "Used", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Opposite);
		ImPlot::SetNextLineStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
		ImPlot::PlotLine("Live allocations", statisticsAllocationsHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
		ImPlot::PlotLine("Used", statisticsUsedHistory, numSamples, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}

	if (!childHeapInspectors.empty() && isOpen) {
		for (auto& inspector : childHeapInspectors)
			inspector->RenderStatistics();

		ImGui::TreePop();
	}

	ImGui::PopID();
}
#endif

inline static size_t hashAvalanche(size_t key)
{
	size_t k = (size_t)key;

	k ^= k >> 33;
	k *= 0xff51afd7ed558ccd;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53;
	k ^= k >> 33;

	return k;
}

class RenderMapIterator : public MemoryBlockFunction {
	ImDrawList* dl;
	size_t startAddr;
	size_t endAddr;
	ImVec2 graphStart;
	ImVec2 graphEnd;
	float multiplier;
public:
	RenderMapIterator(ImDrawList* dl, size_t startAddr, size_t endAddr, ImVec2 graphStart, ImVec2 graphEnd)
		: dl{ dl }
		, startAddr{ startAddr }
		, endAddr{ endAddr }
		, graphStart{ graphStart }
		, graphEnd{ graphEnd }
		, multiplier{ static_cast<float>(graphEnd.x - graphStart.x) / static_cast<float>(endAddr - startAddr) }
	{}

	virtual void operator()(void* ptr, size_t size) {
		size_t sPtr = reinterpret_cast<size_t>(ptr);

		dl->AddRectFilled(
			{ graphStart.x + multiplier * (sPtr - startAddr), graphStart.y },
			{ graphStart.x + multiplier * (sPtr - startAddr + size), graphEnd.y },
			ImGui::GetColorU32(ImPlot::GetColormapColor(static_cast<int>((hashAvalanche(sPtr) ^ hashAvalanche(size)) & 0x7FFFFFFF)))
		);
	}
};

void HeapInspector::RenderAllocationMaps() {
	ImGui::PushID(target);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto isOpen = ImGui::TreeNodeEx("Node", ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | (childHeapInspectors.empty() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0), "%s", target->GetName());

	ImGui::TableNextColumn();

	auto* dl = ImGui::GetWindowDrawList();

	auto cpos = ImGui::GetCursorPos();

	auto startAddr = target->GetBufferTop();
	auto endAddr = target->GetBufferEnd();

	auto graphStart = ImGui::GetCursorScreenPos();
	auto graphEnd = ImVec2{ graphStart.x + ImGui::GetContentRegionAvail().x, graphStart.y + 50.0f };

	RenderMapIterator renderMap{ dl, startAddr, endAddr, graphStart, graphEnd };

	dl->AddRectFilled(graphStart, graphEnd, ImGui::GetColorU32(ImGuiCol_FrameBg));
	target->ForEachAllocatedBlock(renderMap);

	ImGui::SetCursorPos({ cpos.x, cpos.y + 50.0f });

	if (!childHeapInspectors.empty() && isOpen) {
		for (auto& inspector : childHeapInspectors)
			inspector->RenderAllocationMaps();

		ImGui::TreePop();
	}

	ImGui::PopID();
}

void HeapInspector::RenderDetails()
{
}

MemoryInspector::MemoryInspector(IAllocator* allocator) : StandaloneWindow{ allocator }
{
	auto* lh = RESOLVE_STATIC_VARIABLE(hh::fw::heap::LocalHeap::instance);

	heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, &lh->baseHeap });

#ifdef DEVTOOLS_TARGET_SDK_wars
	for (auto& heap : lh->heaps1)
		heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, heap.second });
	for (auto& heap : lh->heaps2)
		heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, heap.second });
#else
	for (auto& heap : lh->heaps)
		heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, heap.second });
	heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, lh->memoryRouter->moduleAllocator->heap });
#endif

	SetTitle("Memory Inspector");
}

void MemoryInspector::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(1600, 1100), ImGuiCond_Once);
}

void MemoryInspector::RenderContents()
{
	for (auto& inspector : heapInspectors)
		inspector->Tick();

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Units")) {
			for (int i = 0; i < 4; i++)
				if (ImGui::MenuItem(unitNames[i], nullptr, units == i))
					units = i;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));

	if (ImGui::BeginTabBar("Memory inspector type")) {
#ifndef DEVTOOLS_TARGET_SDK_wars
		if (ImGui::BeginTabItem("Used memory")) {
			char totalHeader[20]{};
			char usedHeader[20]{};
			char allocatedHeader[20]{};

			snprintf(totalHeader, sizeof(totalHeader), "Total (%s)", unitNames[units]);
			snprintf(usedHeader, sizeof(usedHeader), "Used (%s)", unitNames[units]);
			snprintf(allocatedHeader, sizeof(allocatedHeader), "Allocated (%s)", unitNames[units]);

			if (ImGui::BeginTable("Allocated memory", 8, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn(totalHeader);
				ImGui::TableSetupColumn(usedHeader);
				ImGui::TableSetupColumn(allocatedHeader);
				ImGui::TableSetupColumn("Live allocations");
				ImGui::TableSetupColumn("Total allocations");
				ImGui::TableSetupColumn("Usage plot (teal = allocated unused memory, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
				ImGui::TableSetupColumn("Activity plot (teal = live allocations, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
				ImGui::TableHeadersRow();

				for (auto& inspector : heapInspectors)
					inspector->RenderStatistics();

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
#endif

#ifndef DEVTOOLS_TARGET_SDK_miller
		if (ImGui::BeginTabItem("Free memory")) {
			char totalHeader[20]{};
			char freeHeader[20]{};
			char maxFreeHeader[20]{};

			snprintf(totalHeader, sizeof(totalHeader), "Total (%s)", unitNames[units]);
			snprintf(freeHeader, sizeof(freeHeader), "Free (%s)", unitNames[units]);
			snprintf(maxFreeHeader, sizeof(maxFreeHeader), "Max free (%s)", unitNames[units]);

			if (ImGui::BeginTable("Free memory", 8, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn(totalHeader);
				ImGui::TableSetupColumn(freeHeader);
				ImGui::TableSetupColumn(maxFreeHeader);
				ImGui::TableSetupColumn("Live allocations");
				ImGui::TableSetupColumn("Usage plot (teal = allocated unused memory, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
				ImGui::TableSetupColumn("Activity plot (teal = live allocations, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
				ImGui::TableHeadersRow();

				for (auto& inspector : heapInspectors)
					inspector->RenderHeapInformation();

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
#endif

		if (ImGui::BeginTabItem("Allocation maps")) {
			if (ImGui::BeginTable("Heap inspectors", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn("Allocation map", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				for (auto& inspector : heapInspectors)
					inspector->RenderAllocationMaps();

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Details")) {
			if (ImGui::BeginCombo("Heap", detailsInspector == nullptr ? "" : detailsInspector->target->GetName())) {
				for (auto& inspector : heapInspectors)
					RenderHeapSelectComboItem(inspector);

				ImGui::EndCombo();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImPlot::PopStyleVar();
}

void MemoryInspector::RenderHeapSelectComboItem(HeapInspector* inspector)
{
	if (ImGui::Selectable(inspector->target->GetName(), detailsInspector == inspector))
		detailsInspector = inspector;

	if (detailsInspector == inspector)
		ImGui::SetItemDefaultFocus();

	for (auto& child : inspector->childHeapInspectors)
		RenderHeapSelectComboItem(child);
}
