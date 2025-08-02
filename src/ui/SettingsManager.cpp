#include "SettingsManager.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/Context.h>
#include <ui/Desktop.h>
#include <ui/input/Input.h>
#include <debug-rendering/DebugRenderingSystem.h>
#include <ui/operation-modes/behaviors/ObjectLocationVisual3D.h>
#include <ui/GlobalSettings.h>
#include <modules/PhotoMode.h>

ImGuiSettingsHandler SettingsManager::settingsHandler{};
SettingsManager::Settings SettingsManager::settings{};
bool SettingsManager::showConfigDialog{ false };

bool SettingsManager::Settings::operator==(const SettingsManager::Settings& other) const {
	bool equal = theme == other.theme
		&& language == other.language
		&& fontSize == other.fontSize
		&& gizmoLineScale == other.gizmoLineScale
		&& gizmoHandleScale == other.gizmoHandleScale
		&& configFlags == other.configFlags
		&& debugCameraMouseSensitivityX == other.debugCameraMouseSensitivityX
		&& debugCameraMouseSensitivityY == other.debugCameraMouseSensitivityY
		&& rflDefaultFloatStep == other.rflDefaultFloatStep
		&& rflMinFloatStep == other.rflMinFloatStep
		&& rflSliderCutOffRange == other.rflSliderCutOffRange
		&& enableViewports == other.enableViewports
		&& debugRenderingRenderGOCVisualDebugDraw == other.debugRenderingRenderGOCVisualDebugDraw
		&& debugRenderingRenderColliders == other.debugRenderingRenderColliders
		&& debugRenderingRenderOcclusionCapsules == other.debugRenderingRenderOcclusionCapsules
		&& debugRenderingRenderBones == other.debugRenderingRenderBones
		&& debugRenderingRenderPaths == other.debugRenderingRenderPaths
		&& debugRenderingRenderPathNormals == other.debugRenderingRenderPathNormals
		&& debugRenderingRenderPathTangents == other.debugRenderingRenderPathTangents
		&& debugRenderingGOCVisualDebugDrawOpacity == other.debugRenderingGOCVisualDebugDrawOpacity
		&& debugRenderingLevelEditorDebugBoxScale == other.debugRenderingLevelEditorDebugBoxScale
		&& debugRenderingLevelEditorDebugBoxRenderLimit == other.debugRenderingLevelEditorDebugBoxRenderLimit
		&& debugRenderingLevelEditorDebugBoxRenderDistance == other.debugRenderingLevelEditorDebugBoxRenderDistance
		&& debugRenderingLevelEditorDebugBoxRenderPrimaryTags == other.debugRenderingLevelEditorDebugBoxRenderPrimaryTags
		&& debugRenderingLevelEditorDebugBoxRenderSecondaryTags == other.debugRenderingLevelEditorDebugBoxRenderSecondaryTags
		&& enablePhotoMode == other.enablePhotoMode
		&& enableApi == other.enableApi
		&& !strcmp(apiHost, other.apiHost)
		&& apiPort == other.apiPort
		&& !strcmp(defaultFileDialogDir, other.defaultFileDialogDir)
		;

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			equal = equal && debugRenderingColliderFilters[i][j] == other.debugRenderingColliderFilters[i][j];

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			equal = equal && selectionColliderFilters[i][j] == other.selectionColliderFilters[i][j];

	for (size_t i = 0; i < shortcutCount; i++)
		equal = equal && shortcutBindings[i] == other.shortcutBindings[i];

	return equal;
}

bool SettingsManager::Settings::operator!=(const SettingsManager::Settings& other) const {
	return !operator==(other);
}

void SettingsManager::Init() {
	settingsHandler.TypeName = "DevTools";
	settingsHandler.TypeHash = ImHashStr(settingsHandler.TypeName);
	settingsHandler.ClearAllFn = ClearAllFn;
	settingsHandler.ReadInitFn = ReadInitFn;
	settingsHandler.ReadOpenFn = ReadOpenFn;
	settingsHandler.ReadLineFn = ReadLineFn;
	settingsHandler.ApplyAllFn = ApplyAllFn;
	settingsHandler.WriteAllFn = WriteAllFn;
	settingsHandler.UserData = &settings;
	ImGui::AddSettingsHandler(&settingsHandler);

	// We apply the defaults on startup in case the user doesn't have a config yet.
	ApplySettings();
}

void SettingsManager::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(1400, 1000), ImGuiCond_Once);
	if (showConfigDialog) {
		if (ImGui::Begin("Settings", &showConfigDialog, windowFlags)) {
			Settings tempSettings{ settings };

			if (ImGui::BeginTabBar("SettingsTabs")) {
				if (ImGui::BeginTabItem("UI")) {
					if (ImGui::BeginTabBar("UITabs")) {
						if (ImGui::BeginTabItem("General")) {
							ImGui::SeparatorText("General");
							const char* themeComboStr = Theme::themes[tempSettings.theme].name;
							if (ImGui::BeginCombo("Theme", themeComboStr)) {
								for (unsigned int i = 0; i < Theme::themeCount; i++) {
									bool is_selected = tempSettings.theme == i;

									if (ImGui::Selectable(Theme::themes[i].name, is_selected)) {
										tempSettings.theme = i;
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}


#ifndef DEVTOOLS_TARGET_SDK_miller
							ImGui::Checkbox("Enable multiple viewports", &tempSettings.enableViewports);
							ImGui::SetItemTooltip("Enables ImGui's multiple viewports feature, which lets you drag DevTools windows out of the game window when running in windowed mode.");
#endif
							ImGui::SeparatorText("Navigation");
							ImGui::CheckboxFlags("Allow keyboard navigation", &tempSettings.configFlags, ImGuiConfigFlags_NavEnableKeyboard);
							ImGui::CheckboxFlags("Allow gamepad navigation", &tempSettings.configFlags, ImGuiConfigFlags_NavEnableGamepad);

							ImGui::SeparatorText("File dialogs");
							InputDirectory("Default directory", tempSettings.defaultFileDialogDir);
							ImGui::SetItemTooltip("Default location to open in file export dialogs.");

							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Reflection editor")) {
							const char* languageComboStr = Translations::GetLanguageName(Translations::GetCurrentLanguage());
							bool languageComboOpen = ImGui::BeginCombo("Description language", languageComboStr);
							ImGui::SetItemTooltip("Which language to use in reflection editor property descriptions.");
							if (languageComboOpen) {
								for (unsigned int i = 0; i < 3; i++) {
									Translations::Language language = static_cast<Translations::Language>(i);
									bool is_selected = tempSettings.language == language;

									if (ImGui::Selectable(Translations::GetLanguageName(language), is_selected)) {
										tempSettings.language = language;
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
							ImGui::DragFloat("Default step size for float values", &tempSettings.rflDefaultFloatStep, 0.001f);
							ImGui::SetItemTooltip("Default precision for float values if the game does not specify a precision.");
							ImGui::DragFloat("Minimum step size for float values", &tempSettings.rflMinFloatStep, 0.01f);
							ImGui::SetItemTooltip("The drag control's precision for float values will be capped to this value if the game specifies a smaller precision. Intended to correct for the game often specifying the excessively precise value 0.001.");
							ImGui::DragScalar("Slider cut-off range", ImGuiDataType_U32, &tempSettings.rflSliderCutOffRange, 1.0f);
							ImGui::SetItemTooltip("Range between minimum and maximum value at which the editor will choose to use drag controls instead of sliders.");
							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Accessibility")) {
							ImGui::SeparatorText("Text");
							ImGui::DragFloat("Font size", &tempSettings.fontSize, 1.0f, 6.0f, 64.0f);
							ImGui::SetItemTooltip("Which font size to use. The UI will scale to fit your chosen size.");

							ImGui::SeparatorText("Gizmos");
							ImGui::DragFloat("Line scale", &tempSettings.gizmoLineScale, 0.01f, 0.01f, 64.0f);
							ImGui::SetItemTooltip("Scaling to apply to 3D gizmo lines.");
							ImGui::DragFloat("Handle scale", &tempSettings.gizmoHandleScale, 0.01f, 0.01f, 64.0f);
							ImGui::SetItemTooltip("Scaling to apply to 3D gizmo handles (the circles and arrows).");
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Camera")) {
					ImGui::DragFloat("Debug camera mouse horizontal sensitivity", &tempSettings.debugCameraMouseSensitivityX, 0.001f);
					ImGui::DragFloat("Debug camera mouse vertical sensitivity", &tempSettings.debugCameraMouseSensitivityY, 0.001f);
					ImGui::Checkbox("Enable photo mode (L3)", &tempSettings.enablePhotoMode);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Selection")) {
					if (ImGui::BeginTabBar("Selection tabs")) {
						if (ImGui::BeginTabItem("Selection collider filters")) {
							if (ImGui::BeginTable("Filters", 33))
							{
								ImGui::TableSetupColumn("Filter categories", ImGuiTableColumnFlags_WidthStretch);
								for (int j = 0; j < 32; j++) {
									char header[10];
									snprintf(header, 10, "%d", j);
									ImGui::TableSetupColumn(header, ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f);
								}

								ImGui::TableHeadersRow();

								for (int i = 0; i < 32; i++)
								{
									ImGui::PushID(i);
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Layer %d", i);

									for (int j = 0; j < 32; j++) {
										ImGui::PushID(j);
										ImGui::TableNextColumn();
										ImGui::Checkbox("", &tempSettings.selectionColliderFilters[i][j]);
										ImGui::PopID();
									}

									ImGui::PopID();
								}
								ImGui::EndTable();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Debug rendering")) {
					if (ImGui::BeginTabBar("Debug rendering tabs")) {
						if (ImGui::BeginTabItem("General options")) {
							constexpr uint8_t minAlpha{ 0 };
							constexpr uint8_t maxAlpha{ 255 };
							ImGui::Checkbox("Render GOCVisualDebugDraw visuals", &tempSettings.debugRenderingRenderGOCVisualDebugDraw);
							ImGui::Checkbox("Render colliders", &tempSettings.debugRenderingRenderColliders);
							ImGui::Checkbox("Render occlusion capsules", &tempSettings.debugRenderingRenderOcclusionCapsules);
							ImGui::Checkbox("Render bones", &tempSettings.debugRenderingRenderBones);
							ImGui::Checkbox("Render paths", &tempSettings.debugRenderingRenderPaths);
							ImGui::Indent();
							ImGui::Checkbox("Render path normals", &tempSettings.debugRenderingRenderPathNormals);
							ImGui::Checkbox("Render path tangents", &tempSettings.debugRenderingRenderPathTangents);
							ImGui::Unindent();
							ImGui::SliderScalar("GOCVisualDebugDraw opacity (requires stage restart)", ImGuiDataType_U8, &tempSettings.debugRenderingGOCVisualDebugDrawOpacity, &minAlpha, &maxAlpha);
							ImGui::SeparatorText("Debug boxes");
							ImGui::SetItemTooltip("Options for debug boxes (the purple boxes showing the position of invisible objects).");
							ImGui::DragFloat("Level editor debug box scale", &tempSettings.debugRenderingLevelEditorDebugBoxScale, 0.05f);
							ImGui::SetItemTooltip("Size of the debug boxes.");
							ImGui::DragScalar("Level editor debug box render limit", ImGuiDataType_U32, &tempSettings.debugRenderingLevelEditorDebugBoxRenderLimit);
							ImGui::SetItemTooltip("Max amount of debug boxes that can be rendered.");
							ImGui::DragFloat("Level editor debug box render distance", &tempSettings.debugRenderingLevelEditorDebugBoxRenderDistance, 0.05f);
							ImGui::SetItemTooltip("Culls any debug boxes beyond this distance.");
							ImGui::Checkbox("Render primary tags", &tempSettings.debugRenderingLevelEditorDebugBoxRenderPrimaryTags);
							ImGui::SetItemTooltip("Whether the primary text tag should be rendered (usually object name).");
							ImGui::Checkbox("Render secondary tags", &tempSettings.debugRenderingLevelEditorDebugBoxRenderSecondaryTags);
							ImGui::SetItemTooltip("Whether the secondary text tag should be rendered (usually object type).");
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Collider rendering filters")) {
							if (ImGui::BeginTable("Filters", 33))
							{
								ImGui::TableSetupColumn("Filter categories", ImGuiTableColumnFlags_WidthStretch);
								for (int j = 0; j < 32; j++) {
									char header[10];
									snprintf(header, 10, "%d", j);
									ImGui::TableSetupColumn(header, ImGuiTableColumnFlags_WidthFixed, 24.0f);
								}

								ImGui::TableHeadersRow();

								for (int i = 0; i < 32; i++)
								{
									ImGui::PushID(i);
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Layer %d", i);

									for (int j = 0; j < 32; j++) {
										ImGui::PushID(j);
										ImGui::TableNextColumn();
										ImGui::Checkbox("", &tempSettings.debugRenderingColliderFilters[i][j]);
										ImGui::PopID();
									}

									ImGui::PopID();
								}
								ImGui::EndTable();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("API")) {
					ImGui::Checkbox("Enable DevTools API", &tempSettings.enableApi);
					ImGui::SetItemTooltip("Runs a HTTP REST API that other tools can connect to to control DevTools or read out game information.");
					ImGui::InputText("Host", tempSettings.apiHost, sizeof(tempSettings.apiHost));
					ImGui::SetItemTooltip("Which host the API will listen on.");
					ImGui::InputScalar("Port", ImGuiDataType_U16, &tempSettings.apiPort);
					ImGui::SetItemTooltip("Which port the API will listen on.");
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Shortcuts")) {
					if (ImGui::BeginTable("Filters", 2))
					{
						ImGui::TableSetupColumn("Shortcut", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 250.0f);
						ImGui::TableHeadersRow();

						ForEachShortcut([&](ShortcutId shortcutId) {
							auto& description = GetShortcutDescription(shortcutId);

							ImGui::PushID(static_cast<int>(shortcutId));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text(description.name);
							ImGui::SetItemTooltip(description.description);
							ImGui::TableNextColumn();

							const char* chordName = ImGui::GetKeyChordName(tempSettings.shortcutBindings[static_cast<size_t>(shortcutId)]);

							ImGui::InputText("##Key", const_cast<char*>(chordName), 100, ImGuiInputTextFlags_ReadOnly);
							if (ImGui::IsItemActive() && !ImGui::IsItemActivated()) {
								ImGuiKeyChord mods{};

								if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt))
									mods |= ImGuiMod_Alt;

								if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
									mods |= ImGuiMod_Ctrl;

								if (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift))
									mods |= ImGuiMod_Shift;

								for (int i = 0; i < ImGuiKey_COUNT; i++) {
									auto key = static_cast<ImGuiKey>(i);

									if ((ImGui::IsKeyboardKey(key) || ImGui::IsMouseKey(key) || ImGui::IsGamepadKey(key)) && !ImGui::IsModKey(key) && ImGui::IsKeyPressed(key)) {
										tempSettings.shortcutBindings[static_cast<size_t>(shortcutId)] = key | mods;
										ImGui::ClearActiveID();
									}
								}
							}
							ImGui::PopID();
						});

						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			if (tempSettings != settings) {
				settings = tempSettings;
				ApplySettings();
				ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);
			}
		}
		ImGui::End();
	}
}

void SettingsManager::OpenConfigDialog() {
	showConfigDialog = true;
}

void SettingsManager::ApplySettings() {
	auto& io = ImGui::GetIO();

	Theme::themes[settings.theme].Load();
	Translations::SetCurrentLanguage(settings.language);
	io.ConfigFlags = (settings.configFlags & settingsConfigFlagsMask) | (io.ConfigFlags & ~settingsConfigFlagsMask);
	defaultFloatStep = settings.rflDefaultFloatStep;
	rflMinFloatStep = settings.rflMinFloatStep;
	rflSliderCutOff = settings.rflSliderCutOffRange;
#ifdef DEVTOOLS_TARGET_SDK_miller
	Context::set_enable_viewports(false);
#else
	Context::set_enable_viewports(settings.enableViewports);
#endif
	Context::set_font_size(settings.fontSize);
	Context::set_api_config(settings.enableApi, settings.apiHost, settings.apiPort);
	devtools::debug_rendering::DebugRenderingSystem::instance->gocVisualDebugDrawsRenderable.enabled = settings.debugRenderingRenderGOCVisualDebugDraw;
	devtools::debug_rendering::DebugRenderingSystem::instance->gocVisualDebugDrawsRenderable.opacity = settings.debugRenderingGOCVisualDebugDrawOpacity;
	devtools::debug_rendering::DebugRenderingSystem::instance->collidersRenderable.enabled = settings.debugRenderingRenderColliders;
	devtools::debug_rendering::DebugRenderingSystem::instance->occlusionCapsulesRenderable.enabled = settings.debugRenderingRenderOcclusionCapsules;
	devtools::debug_rendering::DebugRenderingSystem::instance->bonesRenderable.enabled = settings.debugRenderingRenderBones;
	devtools::debug_rendering::DebugRenderingSystem::instance->pathsRenderable.enabled = settings.debugRenderingRenderPaths;
	devtools::debug_rendering::DebugRenderingSystem::instance->pathsRenderable.normalsEnabled = settings.debugRenderingRenderPathNormals;
	devtools::debug_rendering::DebugRenderingSystem::instance->pathsRenderable.tangentsEnabled = settings.debugRenderingRenderPathTangents;
	ObjectLocationVisual3DBehaviorBase::ApplySettings(
		settings.debugRenderingLevelEditorDebugBoxScale,
		settings.debugRenderingLevelEditorDebugBoxRenderLimit,
		settings.debugRenderingLevelEditorDebugBoxRenderDistance,
		settings.debugRenderingLevelEditorDebugBoxRenderPrimaryTags,
		settings.debugRenderingLevelEditorDebugBoxRenderSecondaryTags
	);
	PhotoMode::enabled = settings.enablePhotoMode;
	strcpy_s(GlobalSettings::defaultFileDialogDirectory, settings.defaultFileDialogDir);

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			Desktop::selectionColliderFilters[i][j] = settings.selectionColliderFilters[i][j];
	
	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			devtools::debug_rendering::DebugRenderingSystem::instance->collidersRenderable.colliderFilters[i][j] = settings.debugRenderingColliderFilters[i][j];

	for (size_t i = 0; i < shortcutCount; i++)
		SetShortcutBinding(static_cast<ShortcutId>(i), settings.shortcutBindings[i]);

	ApplyGizmoScale(settings.gizmoLineScale, settings.gizmoHandleScale);

	//ReloadInputSettings();
}

void SettingsManager::ApplyGizmoScale(float gizmoLineScale, float gizmoHandleScale)
{
	auto& style = ImGuizmo::GetStyle();
	style.TranslationLineThickness = gizmoLineScale * 3.0f;
	style.TranslationLineArrowSize = gizmoHandleScale * 6.0f;
	style.RotationLineThickness = gizmoLineScale * 2.0f;
	style.RotationOuterLineThickness = gizmoLineScale * 3.0f;
	style.ScaleLineThickness = gizmoLineScale * 3.0f;
	style.ScaleLineCircleSize = gizmoHandleScale * 6.0f;
	style.HatchedAxisLineThickness = gizmoLineScale * 6.0f;
	style.CenterCircleSize = gizmoHandleScale * 6.0f;
}

void SettingsManager::ClearAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	settings = SettingsManager::Settings{};
}

void SettingsManager::ReadInitFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
}

void* SettingsManager::ReadOpenFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name)
{
	return &settings;
}

void SettingsManager::ReadLineFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line)
{
	unsigned int u;
	float f;
	char s[512];
	if (sscanf_s(line, "Theme=%u", &u) == 1) { settings.theme = u; return; }
	if (sscanf_s(line, "Translations=%u", &u) == 1) { settings.language = static_cast<Translations::Language>(u); return; }
	if (sscanf_s(line, "FontSize=%f", &f) == 1) { settings.fontSize = f; return; }
	if (sscanf_s(line, "GizmoLineScale=%f", &f) == 1) { settings.gizmoLineScale = f; return; }
	if (sscanf_s(line, "GizmoHandleScale=%f", &f) == 1) { settings.gizmoHandleScale = f; return; }
	if (sscanf_s(line, "ConfigFlags=%u", &u) == 1) { settings.configFlags = u; return; }
	if (sscanf_s(line, "DebugCameraMouseSensitivityX=%f", &f) == 1) { settings.debugCameraMouseSensitivityX = f; return; }
	if (sscanf_s(line, "DebugCameraMouseSensitivityY=%f", &f) == 1) { settings.debugCameraMouseSensitivityY = f; return; }
	if (sscanf_s(line, "DefaultFloatStep=%f", &f) == 1) { settings.rflDefaultFloatStep = f; return; }
	if (sscanf_s(line, "MinFloatStep=%f", &f) == 1) { settings.rflMinFloatStep = f; return; }
	if (sscanf_s(line, "SliderCutOffRange=%u", &u) == 1) { settings.rflSliderCutOffRange = u; return; }
	if (sscanf_s(line, "EnableViewports=%u", &u) == 1) { settings.enableViewports = static_cast<bool>(u); return; }
	if (sscanf_s(line, "DebugRenderingRenderGOCVisualDebugDraw=%u", &u) == 1) { settings.debugRenderingRenderGOCVisualDebugDraw = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderColliders=%u", &u) == 1) { settings.debugRenderingRenderColliders = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderOcclusionCapsules=%u", &u) == 1) { settings.debugRenderingRenderOcclusionCapsules = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderBones=%u", &u) == 1) { settings.debugRenderingRenderBones = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderPaths=%u", &u) == 1) { settings.debugRenderingRenderPaths = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderPathNormals=%u", &u) == 1) { settings.debugRenderingRenderPathNormals = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderPathTangents=%u", &u) == 1) { settings.debugRenderingRenderPathTangents = u; return; }
	if (sscanf_s(line, "DebugRenderingGOCVisualDebugDrawOpacity=%u", &u) == 1) { settings.debugRenderingGOCVisualDebugDrawOpacity = static_cast<uint8_t>(u); return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxScale=%f", &f) == 1) { settings.debugRenderingLevelEditorDebugBoxScale = f; return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxRenderLimit=%u", &u) == 1) { settings.debugRenderingLevelEditorDebugBoxRenderLimit = u; return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxRenderDistance=%f", &f) == 1) { settings.debugRenderingLevelEditorDebugBoxRenderDistance = f; return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxRenderPrimaryTags=%u", &u) == 1) { settings.debugRenderingLevelEditorDebugBoxRenderPrimaryTags = static_cast<bool>(u); return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxRenderSecondaryTags=%u", &u) == 1) { settings.debugRenderingLevelEditorDebugBoxRenderSecondaryTags = static_cast<bool>(u); return; }
	if (sscanf_s(line, "EnableAPI=%u", &u) == 1) { settings.enableApi = static_cast<bool>(u); return; }
	if (sscanf_s(line, "APIHost=%127[^\r\n]", s, 128) == 1) { strcpy_s(settings.apiHost, s); return; }
	if (sscanf_s(line, "APIPort=%u", &u) == 1) { settings.apiPort = static_cast<unsigned short>(u); return; }
	if (sscanf_s(line, "EnablePhotoMode=%u", &u) == 1) { settings.enablePhotoMode = static_cast<bool>(u); return; }
	if (sscanf_s(line, "DefaultFileDialogDirectory=%511[^\r\n]", s, 512) == 1) { strcpy_s(settings.defaultFileDialogDir, s); return; }
	if (sscanf_s(line, "SelectionColliderFilters=%256s", s, 512) == 1 && strlen(s) == 256) {
		for (size_t i = 0; i < 32; i++) {
			sscanf_s(s + i * 8, "%8x", &u);
			for (size_t j = 0; j < 32; j++) {
				settings.selectionColliderFilters[i][j] = u & (1 << j);
			}
		}
		return;
	}
	if (sscanf_s(line, "DebugRenderingColliderFilters=%256s", s, 512) == 1 && strlen(s) == 256) {
		for (size_t i = 0; i < 32; i++) {
			sscanf_s(s + i * 8, "%8x", &u);
			for (size_t j = 0; j < 32; j++) {
				settings.debugRenderingColliderFilters[i][j] = u & (1 << j);
			}
		}
		return;
	}
	if (sscanf_s(line, "Shortcut[%128[^]]]=%u", s, 512, &u) == 2) {
		for (size_t i = 0; i < shortcutCount; i++) {
			if (!strcmp(s, GetShortcutDescription(static_cast<ShortcutId>(i)).name)) {
				settings.shortcutBindings[i] = u;
				return;
			}
		}
	}
}

void SettingsManager::ApplyAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	ApplySettings();
}

void SettingsManager::WriteAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf)
{
	//buf->reserve(buf->size() + sizeof(Settings) * 6);
	out_buf->appendf("[DevTools][]\n");
	out_buf->appendf("Theme=%u\n", settings.theme);
	out_buf->appendf("Translations=%u\n", settings.language);
	out_buf->appendf("FontSize=%f\n", settings.fontSize);
	out_buf->appendf("GizmoLineScale=%f\n", settings.gizmoLineScale);
	out_buf->appendf("GizmoHandleScale=%f\n", settings.gizmoHandleScale);
	out_buf->appendf("ConfigFlags=%u\n", settings.configFlags);
	out_buf->appendf("DebugCameraMouseSensitivityX=%f\n", settings.debugCameraMouseSensitivityX);
	out_buf->appendf("DebugCameraMouseSensitivityY=%f\n", settings.debugCameraMouseSensitivityY);
	out_buf->appendf("DefaultFloatStep=%f\n", settings.rflDefaultFloatStep);
	out_buf->appendf("MinFloatStep=%f\n", settings.rflMinFloatStep);
	out_buf->appendf("SliderCutOffRange=%u\n", settings.rflSliderCutOffRange);
	out_buf->appendf("EnableViewports=%u\n", settings.enableViewports);
	out_buf->appendf("DebugRenderingRenderGOCVisualDebugDraw=%u\n", settings.debugRenderingRenderGOCVisualDebugDraw);
	out_buf->appendf("DebugRenderingRenderColliders=%u\n", settings.debugRenderingRenderColliders);
	out_buf->appendf("DebugRenderingRenderOcclusionCapsules=%u\n", settings.debugRenderingRenderOcclusionCapsules);
	out_buf->appendf("DebugRenderingRenderBones=%u\n", settings.debugRenderingRenderBones);
	out_buf->appendf("DebugRenderingRenderPaths=%u\n", settings.debugRenderingRenderPaths);
	out_buf->appendf("DebugRenderingRenderPathNormals=%u\n", settings.debugRenderingRenderPathNormals);
	out_buf->appendf("DebugRenderingRenderPathTangents=%u\n", settings.debugRenderingRenderPathTangents);
	out_buf->appendf("DebugRenderingGOCVisualDebugDrawOpacity=%u\n", settings.debugRenderingGOCVisualDebugDrawOpacity);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxScale=%f\n", settings.debugRenderingLevelEditorDebugBoxScale);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxRenderLimit=%u\n", settings.debugRenderingLevelEditorDebugBoxRenderLimit);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxRenderDistance=%f\n", settings.debugRenderingLevelEditorDebugBoxRenderDistance);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxRenderPrimaryTags=%f\n", settings.debugRenderingLevelEditorDebugBoxRenderPrimaryTags);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxRenderSecondaryTags=%f\n", settings.debugRenderingLevelEditorDebugBoxRenderSecondaryTags);
	out_buf->appendf("EnableAPI=%u\n", settings.enableApi);
	out_buf->appendf("APIHost=%s\n", settings.apiHost);
	out_buf->appendf("APIPort=%u\n", settings.apiPort);
	out_buf->appendf("EnablePhotoMode=%u\n", settings.enablePhotoMode);
	out_buf->appendf("DefaultFileDialogDirectory=%s\n", settings.defaultFileDialogDir);

	out_buf->appendf("SelectionColliderFilters=");
	for (size_t i = 0; i < 32; i++) {
		unsigned int u{ 0 };

		for (size_t j = 0; j < 32; j++)
			if (settings.selectionColliderFilters[i][j])
				u |= 1 << j;

		out_buf->appendf("%08x", u);
	}
	out_buf->appendf("\n");

	out_buf->appendf("DebugRenderingColliderFilters=");
	for (size_t i = 0; i < 32; i++) {
		unsigned int u{ 0 };

		for (size_t j = 0; j < 32; j++)
			if (settings.debugRenderingColliderFilters[i][j])
				u |= 1 << j;

		out_buf->appendf("%08x", u);
	}
	out_buf->appendf("\n");

	for(size_t i = 0; i < shortcutCount; i++)
		out_buf->appendf("Shortcut[%s]=%u\n", GetShortcutDescription(static_cast<ShortcutId>(i)).name, settings.shortcutBindings[i]);
}
