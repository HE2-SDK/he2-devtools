#include "DvNode.h"
#include <utilities/math/MathUtils.h>
#include <ranges>

namespace ui::operation_modes::modes::dvscene_editor {
	
	constexpr std::pair<size_t, size_t> NodeDataInfo[] = {
		{},
		{sizeof(hh::dv::DvNodePath::Description), offsetof(hh::dv::DvNodePath, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeCamera::Description), offsetof(hh::dv::DvNodeCamera, binaryData)},
		{sizeof(hh::dv::DvNodeCameraMotion::Description), offsetof(hh::dv::DvNodeCameraMotion, binaryData)},
		{sizeof(hh::dv::DvNodeCharacter::Description), offsetof(hh::dv::DvNodeCharacter, binaryData)},
		{sizeof(hh::dv::DvNodeCharacterMotion::Description), offsetof(hh::dv::DvNodeCharacterMotion, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModel::Description), offsetof(hh::dv::DvNodeModel, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModelMotion::Description), offsetof(hh::dv::DvNodeModelMotion, binaryData)},
		{sizeof(hh::dv::DvNodeModelNode::Description), offsetof(hh::dv::DvNodeModelNode, binaryData)},
		{sizeof(hh::dv::DvNodeElement::DescriptionBase), offsetof(hh::dv::DvNodeElement, binaryData)}
	};
	constexpr std::pair<size_t, size_t> ElementDataInfo[] = {
		{},
		{sizeof(hh::dv::DvElementCameraParams::Description), offsetof(hh::dv::DvElementCameraParams, binaryData)},
		{},
		{sizeof(hh::dv::DvElementDrawOff::Description), offsetof(hh::dv::DvElementDrawOff, binaryData)},
		{},
		{sizeof(hh::dv::DvElementPathOffset::Description), offsetof(hh::dv::DvElementPathOffset, binaryData)},
		{sizeof(hh::dv::DvElementCameraShake::Description), offsetof(hh::dv::DvElementCameraShake, binaryData)},
		{sizeof(hh::dv::DvElementCameraShakeLoop::Description), offsetof(hh::dv::DvElementCameraShakeLoop, binaryData)},
		{/*sizeof(hh::dv::DvElementEffect::Description)*/676, offsetof(hh::dv::DvElementEffect, binaryData)}, //due to aligning im hardcoding the size for now
		{sizeof(hh::dv::DvElementDirectionalLight::Description), offsetof(hh::dv::DvElementDirectionalLight, binaryData)},
		{sizeof(hh::dv::DvElementPathInterpolation::Description), offsetof(hh::dv::DvElementPathInterpolation, binaryData)},
		{0, 0},
		{sizeof(hh::dv::DvElementCameraNearFar::Description), offsetof(hh::dv::DvElementCameraNearFar, binaryData)},
		{sizeof(hh::dv::DvElementUVAnim::Description), offsetof(hh::dv::DvElementUVAnim, binaryData)},
		{sizeof(hh::dv::DvElementVisibilityAnim::Description), offsetof(hh::dv::DvElementVisibilityAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialAnim::Description), offsetof(hh::dv::DvElementMaterialAnim, binaryData)},
		{sizeof(hh::dv::DvElementMultipleAnim::Description), offsetof(hh::dv::DvElementMultipleAnim, binaryData)},
		{sizeof(hh::dv::DvElementCameraOffset::Description), offsetof(hh::dv::DvElementCameraOffset, binaryData)},
		{sizeof(hh::dv::DvElementModelFade::Description), offsetof(hh::dv::DvElementModelFade, binaryData)},
		{sizeof(hh::dv::DvElementDebugMotion::Description), offsetof(hh::dv::DvElementDebugMotion, binaryData)},
		{sizeof(hh::dv::DvElementCameraHedgehog::Description), offsetof(hh::dv::DvElementCameraHedgehog, binaryData)},
		{sizeof(hh::dv::DvElementCameraInGame::Description), offsetof(hh::dv::DvElementCameraInGame, binaryData)},
		{sizeof(hh::dv::DvElementPointLight::Description), offsetof(hh::dv::DvElementPointLight, binaryData)},
		{sizeof(hh::dv::DvElementVertexAnimationTexture::Description), offsetof(hh::dv::DvElementVertexAnimationTexture, binaryData)},
		{sizeof(hh::dv::DvElementSpotlight::Description), offsetof(hh::dv::DvElementSpotlight, binaryData)},
		{sizeof(hh::dv::DvElementControllerVibration::Description), offsetof(hh::dv::DvElementControllerVibration, binaryData)},
#ifdef DEVTOOLS_TARGET_SDK_rangers
		{sizeof(hh::dv::DvElementSpotlightModel::Description), offsetof(hh::dv::DvElementSpotlightModel, binaryData)}
#elif DEVTOOLS_TARGET_SDK_miller
		{sizeof(hh::dv::DvElementTexturePatternAnim::Description), offsetof(hh::dv::DvElementTexturePatternAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialParam::Description), offsetof(hh::dv::DvElementMaterialParam, binaryData)},
#endif
	};

	DvNode::DvNode() {}
	DvNode::DvNode(dv::DvNode* node, Context& ctx) : fileNode{ node }, ctx{ &ctx }
	{
		this->node = ctx.GetRuntimeNode(node);
		if(fileNode)
			fileResource = ctx.GetResourceByFileNode(fileNode);
	}

	bool DvNode::operator==(const DvNode& other) const
	{
		return memcmp(&fileNode->guid, &other.fileNode->guid, 16) == 0 && strcmp(fileNode->name, other.fileNode->name) == 0;
	}

	const char* DvNode::GetName() const
	{
		return fileNode->name;
	}

	bool DvNode::CanTransform() const 
	{
		constexpr unsigned int transformableElements[] = {
			5, 8, 17, 22, 24, 26, 1036
		};
		if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT)
			return std::ranges::find(transformableElements, static_cast<int>(reinterpret_cast<hh::dv::DvNodeElement*>(node)->binaryData.elementId)) != std::end(transformableElements);
		else
			return node->nodeType == hh::dv::DvNodeBase::NodeType::PATH;
	}

	//TODO: Fix parent transforms
	Eigen::Affine3f DvNode::GetTransform() const 
	{
		if(CanTransform())
		{
			Eigen::Affine3f transform = Eigen::Affine3f::Identity();
			switch (node->nodeType) {
			case hh::dv::DvNodeBase::NodeType::PATH:
				transform = Eigen::Affine3f{ reinterpret_cast<hh::dv::DvNodePath::Description*>(fileNode->data)->matrix.matrix() };
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = &fileNode->data[sizeof(hh::dv::DvNodeElement::Description<hh::dv::DvElementBase::Description>)];
				switch (reinterpret_cast<hh::dv::DvNodeElement::Description<hh::dv::DvElementBase::Description>*>(fileNode->data)->elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					transform = Eigen::Affine3f{ reinterpret_cast<hh::dv::DvElementPathOffset::Description*>(element)->offsetMatrix.matrix() };
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					transform = Eigen::Affine3f{ reinterpret_cast<hh::dv::DvElementEffect::Description*>(element)->effectMatrix.matrix() };
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					transform.translation() = reinterpret_cast<hh::dv::DvElementCameraOffset::Description*>(element)->offsetPosition;
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementPointLight::Description*>(element)->position;
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlight::Description*>(element)->position;
					break;
#ifdef DEVTOOLS_TARGET_SDK_rangers
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlightModel::Description*>(element)->position;
					break;
#endif
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					transform.translation() = reinterpret_cast<app::dv::DvElementVariablePointLight::Description*>(element)->position;
					break;
				}
				break;
			}
			}
			if(auto* x = node->parent)
				transform = TransformToAffine3f(x->transform) * transform;
			return transform;
		}
		else
			return Eigen::Affine3f{};
	}
	
    void DvNode::SetTransform(const Eigen::Affine3f &transform)
    {
		if(CanTransform())
		{
			auto trans = transform;
			if(auto* x = node->parent)
				trans = TransformToAffine3f(x->transform).inverse() * transform;
			switch (node->nodeType) {
			case hh::dv::DvNodeBase::NodeType::PATH:
				reinterpret_cast<hh::dv::DvNodePath::Description*>(fileNode->data)->matrix = Eigen::Projective3f{ trans.matrix() };
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = &fileNode->data[sizeof(hh::dv::DvNodeElement::Description<hh::dv::DvElementBase::Description>)];
				switch (reinterpret_cast<hh::dv::DvNodeElement::Description<hh::dv::DvElementBase::Description>*>(fileNode->data)->elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					reinterpret_cast<hh::dv::DvElementPathOffset::Description*>(element)->offsetMatrix = Eigen::Projective3f{ trans.matrix() };
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					reinterpret_cast<hh::dv::DvElementEffect::Description*>(element)->effectMatrix = Eigen::Projective3f{ trans.matrix() };
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					reinterpret_cast<hh::dv::DvElementCameraOffset::Description*>(element)->offsetPosition = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					reinterpret_cast<hh::dv::DvElementPointLight::Description*>(element)->position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					reinterpret_cast<hh::dv::DvElementSpotlight::Description*>(element)->position = trans.translation();
					break;
#ifdef DEVTOOLS_TARGET_SDK_rangers
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					reinterpret_cast<hh::dv::DvElementSpotlightModel::Description*>(element)->position = trans.translation();
					break;
#endif
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					reinterpret_cast<app::dv::DvElementVariablePointLight::Description*>(element)->position = trans.translation();
					break;
				}
				break;
			}
			}
			UpdateRuntimeNode();
		}
    }

	DvNode DvNode::GetParent() {
		return LoopThroughParents(node);
	}

	void DvNode::DeleteNode() {
		Delete(node);
		Delete(*fileNode);
	}

	void DvNode::UpdateRuntimeNode() {
		memcpy(node->guid, &fileNode->guid, 16);
		node->nodeName.clear();
		for (char x = 0; x < strlen(fileNode->name) + 1; x++)
			node->nodeName.push_back(fileNode->name[x]);
		memcpy(&reinterpret_cast<char*>(node)[NodeDataInfo[fileNode->category].second], fileNode->data, NodeDataInfo[fileNode->category].first);
		switch (node->nodeType) {
		case hh::dv::DvNodeBase::NodeType::PATH:
			reinterpret_cast<hh::dv::DvNodePath*>(node)->transform = Affine3fToTransform(Eigen::Affine3f(reinterpret_cast<hh::dv::DvNodePath::Description*>(fileNode->data)->matrix.matrix()));
			break;
		/*case hh::dv::DvNodeBase::NodeType::MODEL:
		case hh::dv::DvNodeBase::NodeType::CHARACTER:
			node->Setup(fileNode->data);
			break;*/
		case hh::dv::DvNodeBase::NodeType::ELEMENT: {
			auto* nodeElement = reinterpret_cast<hh::dv::DvNodeElement*>(node);
			auto* nodeElementData = reinterpret_cast<hh::dv::DvNodeElement::Description<hh::dv::DvElementBase::Description>*>(fileNode->data);
			nodeElement->start = nodeElementData->start * 100;
			nodeElement->end = nodeElementData->end * 100;
			char* elementData;
			size_t elementSize = 0;
			if (static_cast<unsigned int>(nodeElement->binaryData.elementId) >= 1000) {
				auto& appElement = *reinterpret_cast<hh::fnd::Reference<app::dv::AppDvElementBase>*>(&nodeElement->element);
				elementData = static_cast<char*>(appElement->elementBinaryData);
				elementSize = appElement->elementBinaryDataSize;
#ifdef DEVTOOLS_TARGET_SDK_miller
				if (static_cast<unsigned int>(nodeElement->binaryData.elementId) == 1000)
					elementSize = 172; // i dunno why, but in the create function it's wrong -_-
#endif
			}
			else {
				auto* element = &*nodeElement->element;
				elementData = &reinterpret_cast<char*>(element)[ElementDataInfo[static_cast<unsigned int>(nodeElement->binaryData.elementId)].second];
				elementSize = ElementDataInfo[static_cast<unsigned int>(nodeElement->binaryData.elementId)].first;
			}
			memcpy(elementData, &fileNode->data[NodeDataInfo[fileNode->category].first], elementSize);
			switch (nodeElement->binaryData.elementId) {
			case hh::dv::DvNodeElement::ElementID::EFFECT:
				nodeElement->element->DeleteData();
				break;
			case hh::dv::DvNodeElement::ElementID::QTE: {
				nodeElement->element->AddCallback(ctx->goDVSC->timeline->preCurrentFrame, nodeElement->transform);
				break;
			}
			}
			break;
		}
		}
	}
}
