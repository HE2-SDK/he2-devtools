#include "Pch.h"
#include "ResourceBrowser.h"
#include "editors/ResReflectionEditor.h"
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "editors/ResEffectEditor.h"
#include "editors/ResGismoConfigDesignEditor.h"
#include "editors/ResGismoConfigPlanEditor.h"
#include "editors/ResMirageLightEditor.h"
#include "editors/ResVibrationEditor.h"
#include "editors/ResModelEditor.h"
#endif
//#include "editors/ResObjectWorldEditor.h"
#include "editors/ResMaterialEditor.h"
#ifndef DEVTOOLS_TARGET_SDK_wars
#include "editors/ResPhysicalSkeletonEditor.h"
#endif
#include <resources/ReloadManager.h>
#include <ui/common/Textures.h>
#include <ui/common/Icons.h>
#include <ui/GlobalSettings.h>
#include <ui/common/viewers/Basic.h>
#include <utilities/ResourceTypes.h>
#include <ucsl-reflection/reflections/resources/object-world/v2.h>
#include <ucsl-reflection/reflections/resources/object-world/v3.h>
#include <rip/binary/containers/binary-file/v2.h>

using namespace hh::fnd;

IconId ResourceTypeToIconId(const ResourceTypeInfo* typeInfo) {
	if (typeInfo == hh::snd::ResAtomConfig::GetTypeInfo()) return IconId::RESOURCE_ATOM_CONFIG;
	if (typeInfo == hh::snd::ResAtomCueSheet::GetTypeInfo()) return IconId::RESOURCE_ATOM_PLAYLIST;
	if (typeInfo == hh::gfx::ResMirageLight::GetTypeInfo()) return IconId::RESOURCE_LIGHTING;
	if (typeInfo == hh::gfx::ResMaterial::GetTypeInfo()) return IconId::RESOURCE_MATERIAL;
	if (typeInfo == hh::gfx::ResTerrainModel::GetTypeInfo()) return IconId::RESOURCE_TERRAIN_MODEL;
	if (typeInfo == hh::gfx::ResModel::GetTypeInfo()) return IconId::RESOURCE_MODEL;
	if (typeInfo == hh::gfnd::ResFragmentShader::GetTypeInfo()) return IconId::RESOURCE_FRAGMENT_SHADER;
	if (typeInfo == hh::gfnd::ResVertexShader::GetTypeInfo()) return IconId::RESOURCE_VERTEX_SHADER;
	if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo()) return IconId::RESOURCE_TEXTURE;
	if (typeInfo == hh::font::ResBitmapFont::GetTypeInfo()) return IconId::RESOURCE_BITMAP_FONT;
	if (typeInfo == hh::font::ResScalableFontSet::GetTypeInfo()) return IconId::RESOURCE_SCALABLE_FONT;
	if (typeInfo == hh::fnd::ResReflection::GetTypeInfo()) return IconId::RESOURCE_REFLECTION;
	if (typeInfo == hh::fnd::Packfile::GetTypeInfo()) return IconId::RESOURCE_PACKFILE;
	if (typeInfo == hh::eff::ResEffect::GetTypeInfo()) return IconId::RESOURCE_PARTICLESYSTEM;
#ifdef DEVTOOLS_TARGET_SDK_rangers
	if (typeInfo == hh::gfnd::ResComputeShader::GetTypeInfo()) return IconId::RESOURCE_COMPUTE_SHADER;
	if (typeInfo == hh::ui::ResSurfRideProject::GetTypeInfo()) return IconId::RESOURCE_SURFRIDE_PROJECT;
	if (typeInfo == heur::resources::ResLevel::GetTypeInfo()) return IconId::RESOURCE_LEVEL;
	if (typeInfo == heur::resources::ResMasterLevel::GetTypeInfo()) return IconId::RESOURCE_MASTER_LEVEL;
	//if (typeInfo == Res::GetTypeInfo()) return IconId::RESOURCE_PACKFILE_LEVELS;
	if (typeInfo == hh::pba::ResPhysicalSkeleton::GetTypeInfo()) return IconId::RESOURCE_PHYSICS_BONE;
	//if (typeInfo == heur::resources::ResSoftBody::GetTypeInfo()) return IconId::RESOURCE_SOFTBODY;
	//if (typeInfo == heur::resources::ResHelperBone::GetTypeInfo()) return IconId::RESOURCE_HELPER_BONE;
	//if (typeInfo == heur::resources::ResOrcaData::GetTypeInfo()) return IconId::RESOURCE_ORCA;
	//if (typeInfo == Res::GetTypeInfo()) return IconId::RESOURCE_UI;
	if (typeInfo == heur::resources::ResAnimation::GetTypeInfo()) return IconId::RESOURCE_ANIMATION;
	if (typeInfo == heur::resources::ResSkeleton::GetTypeInfo()) return IconId::RESOURCE_SKELETON;
	if (typeInfo == hh::dv::ResDvScene::GetTypeInfo()) return IconId::RESOURCE_DVSCENE;
#endif
	return IconId::RESOURCE_MODEL;
}

ResourceBrowser::ResourceBrowser(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }, browser{ ImBrowser::CreateContext() } {
	SetTitle("Resource browser");
}

ResourceBrowser::~ResourceBrowser() {
	ImBrowser::DestroyContext(browser);
}

void ResourceBrowser::PreRender() {
	ImGui::SetNextWindowPos(ImVec2(250 + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().DisplaySafeAreaPadding.x + 2, ImGui::GetMainViewport()->WorkSize.y - 40), ImGuiCond_Once, ImVec2(0, 1));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250 - 800 - ImGui::GetStyle().ItemSpacing.x * 2, 300), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
}

void ResourceBrowser::RenderContents() {
	if (ImGui::BeginMenuBar()) {
		//if (ImGui::MenuItem("Load file...")) {
		//	//ImGuiFileDialog::Instance()->OpenDialog("ResourceLoadFromFileDialog", "Choose File", ".*", ".", ImGuiFileDialogFlags_Modal);


		//	//auto* fileLoader = FileLoader::GetInstance();

		//	//fileLoader->UnkFunc8(foo);

		//	//InplaceTempUri<128> uri{ foo, sizeof(foo) };
		//	//ResourceLoader::Unk1 unk{ 1, "" };
		//	//Desktop::instance->resourceLoader->LoadResource(uri, hh::gfnd::ResTexture::GetTypeInfo(), 0, 1, unk);
		//}
#ifndef DEVTOOLS_TARGET_SDK_wars
		if (ImGui::MenuItem("Load resource...")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal;
			loadNewDialog.OpenDialog("ResourceLoadNewDialog", "Choose File", ".*", cfg);
		}
#endif

		if (ImGui::MenuItem("Watch directory...")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal;
			ImGuiFileDialog::Instance()->OpenDialog("ResourceBrowserWatchDirectory", "Choose directory", nullptr, cfg);
		}

		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Thumbnails", nullptr, currentView == ImBrowser::ViewType::THUMBNAILS))
				currentView = ImBrowser::ViewType::THUMBNAILS;
			if (ImGui::MenuItem("List", nullptr, currentView == ImBrowser::ViewType::LIST))
				currentView = ImBrowser::ViewType::LIST;
			if (ImGui::MenuItem("Tiles", nullptr, currentView == ImBrowser::ViewType::TILES))
				currentView = ImBrowser::ViewType::TILES;
			if (ImGui::MenuItem("Details", nullptr, currentView == ImBrowser::ViewType::DETAILS))
				currentView = ImBrowser::ViewType::DETAILS;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (ImGuiFileDialog::Instance()->Display("ResourceBrowserWatchDirectory", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk())
			ReloadManager::instance->WatchDirectory(ImGuiFileDialog::Instance()->GetCurrentPath());

		ImGuiFileDialog::Instance()->Close();
	}

	if (ImGui::Button("ROOT"))
		currentPath.clear();

	for (size_t i = 0; i < currentPath.size(); i++) {
		ImGui::SameLine();
		if (ImGui::Button(currentPath[i]->GetName()))
			for (size_t j = currentPath.size() - 1; i < j; j--)
				currentPath.remove(j);
	}

	if (ImGui::BeginChild("Main area", {}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		RenderMainArea();
	ImGui::EndChild();

	RenderDialogs();
}

const char* rank1Props[] = { "Type", "Size" };

void ResourceBrowser::RenderMainArea() {
	ImBrowser::Begin(browser);

	if (ImBrowser::BeginBrowser("Resource browser", currentView, rank1Props, 2, nullptr, 0)) {
		if (currentPath.size() == 0) {
			auto* resourceManager = ResourceManager::GetInstance();

#ifdef DEVTOOLS_TARGET_SDK_miller
			//for (auto* container : resourceManager->unpackedResourceContainer.uniqueResourceContainers)
			//	RenderContainerContents(container);
			RenderContainerContents(resourceManager->unpackedResourceContainer.uniqueResourceContainersByTypeInfo.GetValueOrFallback(hh::fnd::Packfile::GetTypeInfo(), nullptr));
#else
			for (auto* container : resourceManager->GetResourceContainers())
				RenderContainerContents(container);
#endif
		}
		else {
			for (auto* container : currentPath[currentPath.size() - 1]->GetResourceContainers())
				RenderContainerContents(container);
		}
		ImBrowser::EndBrowser();
	}

	ImBrowser::End();
}

void ResourceBrowser::RenderContainerContents(const ResourceContainer* container) {
	auto resourceCount = container->GetNumResources();

	ImGui::PushID(container);
	for (int i = 0; i < resourceCount; i++)
		RenderResource(container->GetResourceByIndex(i));
	ImGui::PopID();
}

void ResourceBrowser::RenderResource(ManagedResource* resource) {
	if (!resource)
		return;
	const ResourceTypeInfo* typeInfo = &resource->GetClass();
	bool isSelected{};
	bool isOpened{};

	ImGui::PushID(resource);

	auto icon = GetIconAsImage(ResourceTypeToIconId(typeInfo));
	ImBrowser::Image browserImage{ icon.texId, icon.uv1, icon.uv2, icon.size };

	PropSet props{ hh::fnd::MemoryRouter::GetTempAllocator() };
	GetProps(resource, props);

	csl::ut::MoveArray<ImBrowser::PropertyValue> propVals{ hh::fnd::MemoryRouter::GetTempAllocator() };
	for (auto& p : props.props)
		propVals.push_back({ .key = p.key, .value = p.value });

	ImBrowser::Item(resource, resource->GetName(), selectedResource == resource, &isSelected, &isOpened, propVals.begin(), propVals.size(), browserImage, GetImage(resource));

	if (ImGui::IsItemActive()) {
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("Resource", &resource, sizeof(resource));
			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginPopupContextItem("Resource Context Menu")) {
		if (ImGui::MenuItem("Load from file"))
			ShowLoadResourceDialog(resource);

		ImGui::EndPopup();
	}

	if (isSelected)
		selectedResource = resource;

	if (isOpened) {
		if (typeInfo == Packfile::GetTypeInfo()) {
			selectedResource = nullptr;
			currentPath.push_back(static_cast<Packfile*>(resource));
		}
		else if (typeInfo == ResReflection::GetTypeInfo())
			ResReflectionEditor::Create(Desktop::instance->GetAllocator(), static_cast<ResReflection*>(resource));
#ifdef DEVTOOLS_TARGET_SDK_rangers
		else if (typeInfo == hh::eff::ResEffect::GetTypeInfo())
			ResEffectEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::eff::ResEffect*>(resource));
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
		else if (typeInfo == hh::gfx::ResMaterial::GetTypeInfo())
			ResMaterialEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::gfx::ResMaterial*>(resource));
		else if (typeInfo == hh::pba::ResPhysicalSkeleton::GetTypeInfo())
			ResPhysicalSkeletonEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::pba::ResPhysicalSkeleton*>(resource));
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
		else if (typeInfo == app::ResGismoConfigDesign::GetTypeInfo())
			ResGismoConfigDesignEditor::Create(Desktop::instance->GetAllocator(), static_cast<app::ResGismoConfigDesign*>(resource));
		else if (typeInfo == app::ResGismoConfigPlan::GetTypeInfo())
			ResGismoConfigPlanEditor::Create(Desktop::instance->GetAllocator(), static_cast<app::ResGismoConfigPlan*>(resource));
		else if (typeInfo == hh::gfx::ResMirageLight::GetTypeInfo())
			ResMirageLightEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::gfx::ResMirageLight*>(resource));
		else if (typeInfo == hh::hid::ResVibration::GetTypeInfo())
			ResVibrationEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::hid::ResVibration*>(resource));
		else if (typeInfo == hh::gfx::ResModel::GetTypeInfo())
			ResModelEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::gfx::ResModel*>(resource));
#endif
	}

	ImGui::PopID();
}

ImBrowser::Image ResourceBrowser::GetImage(hh::fnd::ManagedResource* resource)
{
	if (&resource->GetClass() != hh::gfnd::ResTexture::GetTypeInfo())
		return {};

	auto* resTex = static_cast<hh::gfnd::ResTexture*>(resource);
	auto* tex = resTex->GetTexture();

	if (tex == nullptr)
		return {};

	return { GetTextureIDFromResTexture(resTex), { 0.0f, 0.0f }, { 1.0f, 1.0f }, { static_cast<float>(tex->format.width), static_cast<float>(tex->format.height) } };
}

void ResourceBrowser::GetProps(hh::fnd::ManagedResource* resource, PropSet& props)
{
	props.AddProp("Type", "%s", resource->GetClass().pName);
	props.AddProp("Size", "%d kB", resource->GetSize() / 1024);

	if (&resource->GetClass() == hh::gfnd::ResTexture::GetTypeInfo()) {
		if (auto* texture = static_cast<const hh::gfnd::ResTexture*>(resource)->GetTexture()) {
			props.AddProp("Resolution", "%d x %d x %d", texture->format.width, texture->format.height, texture->format.depth);
			props.AddProp("Array size", "%d", texture->format.arraySize);
			props.AddProp("Mip levels", "%d", texture->format.mipLevels);
			props.AddProp("Sample description index", "%d", texture->format.sampleDescIndex);
			props.AddProp("Texture type", "%d", static_cast<unsigned int>(texture->format.type));
			props.AddProp("Format", "%d", static_cast<unsigned int>(texture->format.format));
		}
	}
}

void ResourceBrowser::ShowLoadResourceDialog(hh::fnd::ManagedResource* resource)
{
#ifndef DEVTOOLS_TARGET_SDK_wars
	char extbuf[20];
	const char* ext = GetExtensionByTypeInfo(&resource->GetClass());

	if (ext == nullptr)
		return;

	snprintf(extbuf, 20, ".%s", ext);

	IGFD::FileDialogConfig cfg{};
	cfg.path = GlobalSettings::defaultFileDialogDirectory;
	cfg.flags = ImGuiFileDialogFlags_Modal;
	cfg.userDatas = resource;
	ImGuiFileDialog::Instance()->OpenDialog("ResourceLoadFromFileDialog", "Choose File", extbuf, cfg);
#endif
}

void ResourceBrowser::RenderDialogs()
{
	RenderLoadNewResourceDialog();
}

void ResourceBrowser::RenderGlobalDialogs()
{
	RenderLoadDialog();
	RenderExportDialog();
}

void ResourceBrowser::ShowExportResourceDialog(hh::fnd::ManagedResource* resource)
{
	char extbuf[20];
	const char* ext = GetExtensionByTypeInfo(&resource->GetClass());

	if (ext == nullptr)
		return;

	snprintf(extbuf, 20, ".%s", ext);

	IGFD::FileDialogConfig cfg{};
	cfg.path = GlobalSettings::defaultFileDialogDirectory;
	cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
	cfg.userDatas = resource;
	ImGuiFileDialog::Instance()->OpenDialog("ResourceExportDialog", "Choose File", extbuf, cfg);
}

void ResourceBrowser::RenderLoadNewResourceDialog() {
#ifndef DEVTOOLS_TARGET_SDK_wars
	if (loadNewDialog.Display("ResourceLoadNewDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (loadNewDialog.IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(loadNewDialog.GetUserDatas());
			std::string filePathWithExt = loadNewDialog.GetFilePathName();
			auto dotPos = filePathWithExt.rfind('.');

			if (dotPos != std::string::npos) {
				std::string filePath = filePathWithExt.substr(0, dotPos);
				hh::fnd::InplaceTempUri fileUri{ filePath.c_str(), filePath.size() };
				csl::ut::String filePathFrag{ hh::fnd::MemoryRouter::GetTempAllocator() };
				fileUri.GetPath(filePathFrag);
				fileUri.SetPath(filePathFrag.c_str() + 1);
				std::string ext = filePathWithExt.substr(dotPos + 1);
				hh::fnd::ResourceLoader::Locale locale{};

#ifdef DEVTOOLS_TARGET_SDK_miller
				resourceLoader->LoadResource(fileUri, GetTypeInfoByExtension(ext.c_str()), {}, locale);
#else
				resourceLoader->LoadResource(fileUri, GetTypeInfoByExtension(ext.c_str()), {}, 1, locale);
#endif
			}
		}
		loadNewDialog.Close();
	}
#endif
}

void ResourceBrowser::RenderLoadDialog() {
	if (ImGuiFileDialog::Instance()->Display("ResourceLoadFromFileDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ReloadManager::instance->QueueReload(wFilePath, resource);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void ResourceBrowser::RenderExportDialog() {
	if (ImGuiFileDialog::Instance()->Display("ResourceExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ExportResource(wFilePath.c_str(), resource);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void ResourceBrowser::ExportResource(const wchar_t* filePath, ManagedResource* resource) {
	if (&resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo()) {
		std::ofstream ofs{ filePath, std::ios::trunc | std::ios::binary };
		rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
#ifdef DEVTOOLS_TARGET_SDK_wars
		serializer.serialize<he2sdk::ucsl::GameInterface>(*(ucsl::resources::object_world::v2::ObjectWorldData<he2sdk::ucsl::GameInterface::AllocatorSystem>*)static_cast<hh::game::ResObjectWorld*>(resource)->binaryData);
#else
		serializer.serialize<he2sdk::ucsl::GameInterface>(*(ucsl::resources::object_world::v3::ObjectWorldData<he2sdk::ucsl::GameInterface::AllocatorSystem>*)static_cast<hh::game::ResObjectWorld*>(resource)->binaryData);
#endif
	}
}
