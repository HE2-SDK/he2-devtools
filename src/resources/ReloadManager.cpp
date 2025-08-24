#include "ReloadManager.h"
#include <ui/Desktop.h>
//#include <ui/operation-modes/LevelEditor/LevelEditor.h>
#include <utilities/ResourceTypes.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <future>
#include "ManagedMemoryRegistry.h"

using namespace hh::fnd;
using namespace hh::game;

ReloadManager* ReloadManager::instance{};

ReloadManager::ReloadManager(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
	InitializeCriticalSection(&reloadRequestMutex);
    GameManager::GetInstance()->AddGameStepListener(this);
}

ReloadManager::~ReloadManager() {
	if (fileWatcher != nullptr) {
		fileWatcher->~FileWatch();
		GetAllocator()->Free(fileWatcher);
	}

    if (auto* gameManager = GameManager::GetInstance())
        gameManager->RemoveGameStepListener(this);

	EnterCriticalSection(&reloadRequestMutex);
	for (auto* ptr : reloadRequestsInFlight)
		delete ptr;

	reloadRequestsInFlight.clear();
	LeaveCriticalSection(&reloadRequestMutex);

	DeleteCriticalSection(&reloadRequestMutex);
}

void ReloadManager::QueueReload(const std::wstring& path, ManagedResource* resource)
{
	std::async([path, resource, this]() {
		HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
			return;

		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		DWORD fileSize = GetFileSize(file, NULL);
		void* buffer = allocator.Alloc(fileSize, 64);
		bool result = ReadFile(file, buffer, fileSize, NULL, NULL);
		CloseHandle(file);

		if (!result) {
			allocator.Free(buffer);
			return;
		}

		QueueReload(buffer, fileSize, resource);
	});
}

void ReloadManager::QueueReload(void* ptr, size_t size, ManagedResource* resource)
{
	EnterCriticalSection(&reloadRequestMutex);
	for (auto* request : reloadRequestsInFlight) {
		if (request->ptr == ptr) {
			LeaveCriticalSection(&reloadRequestMutex);
			return;
		}
	}
	reloadRequestsInFlight.push_back(new (GetAllocator()) ReloadRequest{ GetAllocator(), ptr, size, resource });
	LeaveCriticalSection(&reloadRequestMutex);
}

void ReloadManager::QueueReload(hh::fnd::ManagedResource* resource)
{
	QueueReload(nullptr, 0, resource);
}

void ReloadManager::ReloadSync(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	PerformReload(buffer, fileSize, resource);
}

void ReloadManager::ReloadSync(hh::fnd::ManagedResource* resource)
{
	PerformReload(nullptr, 0, resource);
}

void ReloadManager::UpdateCallback(GameManager* gameManager, const hh::game::GameStepInfo& gameStepInfo)
{
	if (reloadRequestsInFlight.empty())
		return;

	csl::ut::MoveArray<ReloadRequest*> requestsToHandle{ GetTempAllocator() };

	EnterCriticalSection(&reloadRequestMutex);
	for (auto* request : reloadRequestsInFlight)
		requestsToHandle.push_back(request);
	reloadRequestsInFlight.clear();
	LeaveCriticalSection(&reloadRequestMutex);

	for (auto* request : requestsToHandle) {
		PerformReload(request->ptr, request->size, request->resource);
		delete request;
    }
}

void ReloadManager::PerformReload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	auto* gameManager = hh::game::GameManager::GetInstance();

	gameManager->PreResourceReloadCallback(resource);
#ifdef DEVTOOLS_TARGET_SDK_rangers
	if (auto* trrMgr = gameManager->GetService<app::trr::TerrainManager>())
		trrMgr->reloaderListener->PreResourceReloadCallback(resource);
#endif

	if (&resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo())
#ifndef DEVTOOLS_TARGET_SDK_miller
		ReloadObjectWorld(buffer, fileSize, static_cast<ResObjectWorld*>(&*resource));
#else
		;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
	else if (&resource->GetClass() == hh::gfx::ResTerrainModel::GetTypeInfo())
		ReloadByLoad(buffer, fileSize, resource);
#endif
	else if (buffer == nullptr)
		ReloadSelf(resource);
	else
		Reload(buffer, fileSize, resource);

	gameManager->PostResourceReloadCallback(resource);
#ifdef DEVTOOLS_TARGET_SDK_rangers
	if (auto* trrMgr = gameManager->GetService<app::trr::TerrainManager>())
		trrMgr->reloaderListener->PostResourceReloadCallback(resource);

	if (&resource->GetClass() == hh::gfx::ResTerrainModel::GetTypeInfo()
		|| &resource->GetClass() == app::gfx::ResPointcloudModel::GetTypeInfo()
		|| &resource->GetClass() == hh::gfx::ResMaterial::GetTypeInfo()
		|| &resource->GetClass() == app::gfx::ResPointcloudLight::GetTypeInfo()) {
		if (auto* resMan = gameManager->GetService<app::game::GameModeResourceManager>()) {
			for (auto& module : resMan->modules) {
				if (module->GetNameHash() == 0x74DA1FC3) {
					module->UnkFunc7();
					module->Load();
				}
			}
		}
	}

	/*if (&resource->GetClass() == app::gfx::ResPointcloudLight::GetTypeInfo()) {
		if (auto* trrMgr = gameManager->GetService<app::trr::TerrainManager>()) {
			for (auto& trrWrld : trrMgr->terrains) {
				if (auto* trrGrnd = static_cast<app::trr::TerrainGround*>(*trrWrld->objects[0])) {
					for (auto& sector : trrGrnd->sectors)
						sector.lightInstances.clear();
					trrGrnd->gocPointLight->ClearLights();
					char buffer[32];
					snprintf(buffer, sizeof(buffer), "%s_trr_s00", trrWrld->terrainId);
					trrGrnd->LoadLight(buffer);
				}
			}
		}
	}*/
#endif
}

void ReloadManager::Reload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

	hh::fnd::ResourceManagerResolver resolver{};
	resource->Reload(buffer, fileSize);
	resource->Resolve(resolver);

	allocator.Free(buffer);
}

void ReloadManager::ReloadByLoad(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

	hh::fnd::ResourceManagerResolver resolver{};
	resource->Unload();

	if (resource->resourceTypeInfo->isInBinaContainer) {
		hh::ut::BinaryFile bfile{ buffer };

		if (!bfile.IsValid()) {
			allocator.Free(buffer);
			return;
		}

		if (auto* prevData = reloadedData.GetValueOrFallback(resource, nullptr))
			allocator.Free(prevData);

		reloadedData.Insert(resource, buffer);

#ifndef DEVTOOLS_TARGET_SDK_miller
		resource->originalBinaryData = buffer;
#endif
		resource->unpackedBinaryData = bfile.GetDataAddress(-1);
		resource->size = fileSize;
	}
	else {
		if (auto* prevData = reloadedData.GetValueOrFallback(resource, nullptr))
			allocator.Free(prevData);

		reloadedData.Insert(resource, buffer);

#ifndef DEVTOOLS_TARGET_SDK_miller
		resource->originalBinaryData = nullptr;
#endif
		resource->unpackedBinaryData = buffer;
		resource->size = fileSize;
	}

	resource->Load(resource->unpackedBinaryData, resource->size);
	resource->Resolve(resolver);
}

void ReloadManager::ReloadSelf(hh::fnd::ManagedResource* resource)
{
	hh::fnd::ResourceManagerResolver resolver{};
	resource->Unload();
	resource->Load(resource->unpackedBinaryData, resource->size);
	resource->Resolve(resolver);
}

#ifndef DEVTOOLS_TARGET_SDK_miller
void ReloadManager::ReloadObjectWorld(void* buffer, size_t fileSize, hh::game::ResObjectWorld* resource)
{
	bool success{ false };

	if (auto* gameManager = hh::game::GameManager::GetInstance()) {
		if (auto* objWorld = gameManager->GetService<hh::game::ObjectWorld>()) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				if (auto* layer = chunk->GetLayerByName(resource->GetName())) {
					hh::ut::BinaryFile bfile{ buffer };

					if (bfile.IsValid()) {
						auto* allocator = layer->GetAllocator();
						bool wasEnabled = layer->IsEnable();

						//if (auto* levelEditor = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
						//	levelEditor->Deselect();

						{
							Reference<hh::game::ObjectWorldChunkLayer> l{ layer };
							chunk->RemoveLayer(layer);
							chunk->ShutdownPendingObjects();
						}

						resource->GetResourceAllocator()->Free(resource->originalBinaryData);
						resource->originalBinaryData = buffer;
						resource->unpackedBinaryData = bfile.GetDataAddress(-1);
						resource->size = fileSize;
						resource->Load(resource->unpackedBinaryData, resource->size);

						for (auto* objData : resource->GetObjects()) {
							if (auto* objInfoName = static_cast<const char*>(hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(objData->gameObjectClass)->GetAttributeValue("objinfo"))) {
								auto* objInfoContainer = GameManager::GetInstance()->GetService<ObjInfoContainer>();
								auto* objInfoClass = RESOLVE_STATIC_VARIABLE(ObjInfoRegistry::instance)->objInfosByName.GetValueOrFallback(objInfoName, nullptr);
								auto* objInfo = objInfoClass->instantiator(GetAllocator());

								objInfoContainer->Register(objInfo->GetInfoName(), objInfo);
							}
						}

						chunk->AddLayer(hh::game::ObjectWorldChunkLayer::Create(allocator, resource));
						chunk->SetLayerEnabled(resource->GetName(), wasEnabled);

						//if (auto* levelEditor = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
						//	levelEditor->ReloadObjectWorldData();

						success = true;
						break;
					}
				}
			}
		}
	}

	if (!success)
		resource->GetResourceAllocator()->Free(buffer);
}
#endif

void ReloadManager::WatchDirectory(const std::string& path) {
	if (fileWatcher != nullptr) {
		fileWatcher->~FileWatch();
		GetAllocator()->Free(fileWatcher);
	}

	fileWatcher = new (GetAllocator()) filewatch::FileWatch<std::string>(path, [this,path](const std::string& subpath, const filewatch::Event changeType) {
		switch (changeType) {
		case filewatch::Event::added:
		case filewatch::Event::modified:
			{
				std::filesystem::path fspath{ path };
				fspath /= subpath;

				if (!std::filesystem::exists(fspath))
					return;

				std::wstring wfilename = fspath.filename();
				std::wstring wext = fspath.extension();

				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
				std::string filename = converter.to_bytes(wfilename);
				size_t dotloc = filename.find('.');

				if (dotloc == std::string::npos)
					return;

				std::string fullExt = filename.substr(dotloc + 1);
				auto* resourceType = GetTypeInfoByExtension(fullExt.c_str());

				if (resourceType == nullptr)
					return;

				auto* resource = ResourceManager::GetInstance()->GetResource(filename.c_str(), resourceType);

				if (resource == nullptr)
					return;

				QueueReload(fspath, resource);
			}
			break;
		}
	});
}

ReloadManager::ReloadRequest::ReloadRequest(csl::fnd::IAllocator* allocator, void* ptr, size_t size, hh::fnd::ManagedResource* resource)
	: CompatibleObject{ allocator }, ptr{ ptr }, size{ size }, resource{ resource }
{
}
