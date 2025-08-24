#pragma once
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/CompatibleObject.h>
#include <dieventlib/dvscene.h>

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"SpotlightModel",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"FxColUpdate",
		"VAT",
		"QTEAccel",
		"TheEndCableObject",
		"RifleBeastLighting"
	};

	constexpr unsigned int hhElementCount = 27;
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"TexturePatternAnim",
		"MaterialParam",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"SSAOParam",
		"OcclusionCapsuleParam",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"BulletTime",
		"TimeStop",
		"ObjectTimeStop",
		"ShadowShape",
		"Falloff",
		"Fog",
		"DOF",
		"FxColUpdate"
	};

	constexpr unsigned int hhElementCount = 28;
#endif

	static bool IsUnknownNode(unsigned int nodeId) {
		return nodeId > sizeof(nodeTypeNames) / sizeof(const char*);
	}

	static const char* GetNodeName(unsigned int nodeId) {
		const char* name = "Unknown";

		if (!IsUnknownNode(nodeId))
			name = nodeTypeNames[nodeId];

		return name;
	}

	static bool IsUnknownElement(unsigned int elementId) {
		if (elementId >= 1000)
			elementId = elementId - 1000 + hhElementCount;

		return elementId > sizeof(elementIDStrings) / sizeof(const char*);
	}

	static const char* GetElementName(unsigned int elementId) {
		const char* name = "Unknown";
		unsigned int stringIdx = elementId;

		if (stringIdx >= 1000)
			stringIdx = stringIdx - 1000 + hhElementCount;

		if (!IsUnknownElement(elementId))
			name = elementIDStrings[stringIdx];

		return name;
	}

	struct DvPage;

	class Context : public CompatibleObject {
		void ContainsElement(bool& contains, const unsigned int& elementId, hh::dv::DvNodeBase* node);
		void GetFileNode(dv::DvNode& curNode, hh::dv::DvNodeBase* node, dv::DvNode*& result);
		void GetFilePage(dv::DvPage& curPage, hh::dv::DvPage* page, dv::DvPage*& result);
		void GetRuntimeNode(hh::dv::DvNodeBase* curNode, dv::DvNode* node, hh::dv::DvNodeBase*& result);
		void GetParentNode(dv::DvNode& curNode, dv::DvNode* node, dv::DvNode*& result);
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::dv::DvSceneControl> goDVSC{};
		dv::DvScene* parsedScene;
		app::evt::EventScene* evtScene;
		std::mt19937 mt{ std::random_device{}() };
		csl::ut::MoveArray32<DvPage*> dvPages;
		csl::ut::MoveArray32<hh::dv::DvNodeBase*> addedNodes;

		csl::ut::VariableString cutsceneName;
		csl::ut::VariableString nodeName;
		int nodeType;
		int elementId;

		bool ContainsElement(const unsigned int& elementId);
		static void GetChildren(hh::dv::DvNodeBase* node, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool& includeElements);
		static void GetNodes(hh::dv::DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool includeElements);
		template<typename T>
		static void SetupElement(hh::dv::DvNodeElement* node, hh::dv::DvNodeElement::ElementID elementId);
		template<typename T>
		static hh::dv::DvNodeElement::Description<T>* CreateElementDesc(unsigned int elementId);
		static void DispatchSetupElement(hh::dv::DvNodeElement* node, hh::dv::DvNodeElement::ElementID elementId);
		void SetNodeBasicProps(hh::dv::DvNodeBase* node, const char* nodeName, unsigned int nodeType, hh::dv::DvNodeBase* parent);
		hh::dv::DvNodeBase* CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, hh::dv::DvNodeBase* parent);
		dv::DvNode CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId);
		void ParentNode(hh::dv::DvNodeBase* parent, hh::dv::DvNodeBase* child);
		void ParentNode(dv::DvNode& parent, dv::DvNode& child);
		dv::DvNode* GetFileNode(hh::dv::DvNodeBase* node);
		dv::DvPage* GetFilePage(hh::dv::DvPage* page);
		hh::dv::DvNodeBase* GetRuntimeNode(dv::DvNode* node);
		void GenerateGUID(char* guid);
		dv::DvNode* GetParentNode(dv::DvNode* node);
		dv::Resource* GetResourceByFileNode(dv::DvNode* node);
		dv::DvPage CreatePage(const char* pageName, unsigned int idx);
		hh::dv::DvPage* CreatePage(const char* pageName, unsigned int idx, hh::dv::DvSceneControl* dvsc);
		void CreateWrapperPages();
		void DeallocateNode(hh::dv::DvNodeBase* node);

		Context(csl::fnd::IAllocator* allocator);
	};

	struct DvPage {
	public:
		Context* ctx;
		hh::dv::DvPage* page;
		dv::DvPage* filePage;

		DvPage();
		DvPage(hh::dv::DvPage* page, Context& ctx) : page{ page }, ctx{ &ctx }
		{
			filePage = ctx.GetFilePage(page);
		}

		void UpdateRuntimePage() {
			page->binaryData.start = filePage->frameStart;
			page->binaryData.end = filePage->frameEnd;
			page->binaryData.skipFrame = filePage->skipFrame;
			page->binaryData.pageIndex = filePage->index;
			memcpy(page->binaryData.pageName, filePage->name, 32);
			page->binaryData.transitionCount = filePage->transition.size();
			page->binaryData.dataSize = filePage->unkData.size();
			for (int trans = 0; trans < filePage->transition.size(); trans++) {
				auto* transition = page->transitions[trans];
				auto& fileTransition = filePage->transition[trans];
				transition->binaryData.destinationPageID = fileTransition.destinationPageID;
				transition->binaryData.conditionCount = fileTransition.conditions.size();
				auto& fileCondition = fileTransition.conditions[0];
				if (fileCondition.type == dv::DvCondition::ConditionType::QTE) {
					auto* qteCond = reinterpret_cast<app::dv::DvPageConditionQTE*>(transition->condition);
					memcpy(&qteCond->binaryData, fileCondition.data, fileCondition.dataSize);
				}
			}
		}
	};

	/*template<typename T>
	inline void Context::SetupElement(hh::dv::DvNodeElement* node, unsigned int elementId)
	{
		typename hh::dv::DvNodeElement<T::Description> data = CreateElementDesc<T::Description>(node, elementId);
		node->Setup(data);
	}*/
}
