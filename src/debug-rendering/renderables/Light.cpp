#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "Light.h"

namespace devtools::debug_rendering::renderables {
	void Light::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::gfx::GOCPointLight::GetClass()) {
					auto* lightGoc = static_cast<hh::gfx::GOCPointLight*>(goc);
					for (auto& light : lightGoc->lights) {
						hh::fnd::WorldPosition worldPos{};
						worldPos.m_Position = light.absolutePosition;
						worldPos.m_Rotation = light.rotation;
						if (auto* hFrame = light.hFrame)
							worldPos.m_Rotation = hFrame->fullTransform.rotation;
						const csl::ut::Color8 lightColor = { 255, 128, 255, 255 };
						if (light.type == ucsl::resources::light::v2::LightType::POINT) {
							ctx.DrawSphere(csl::math::Matrix34AffineTransformation(worldPos.m_Position, {}), light.pointProps.attenuationRadius, lightColor);
						}
						else if (light.type == ucsl::resources::light::v2::LightType::SPOT) {
							RenderSpotlight(ctx, worldPos, light.directionalProps.attenuationRadius, light.directionalProps.innerConeAngle, { 0, 0, 255, 255 });
							RenderSpotlight(ctx, worldPos, light.directionalProps.attenuationRadius, light.directionalProps.outerConeAngle, { 128, 255, 255, 255 });
						}
					}
				}
			}
		}
	}

	void Light::RenderSpotlight(hh::gfnd::DrawContext& ctx, const hh::fnd::WorldPosition& worldPos, const float attenuationRadius, const float coneAngle, const csl::ut::Color8 lightColor)
	{
		csl::ut::Color8 colorArgb{ lightColor.a, lightColor.r, lightColor.g, lightColor.b };
		const uint32_t uLightColor = *reinterpret_cast<const uint32_t*>(&colorArgb);

		constexpr const unsigned int segments = 24;
		hh::gfnd::DrawVertex vertices[segments + 1];
		unsigned short indices[(segments * 2) * 2];

		float coneBaseRadius = attenuationRadius * tanf(coneAngle);

		vertices[0] = { 0, 0, 0, uLightColor };

		for (unsigned int i = 0; i < segments; i++) {
			float theta = (float)i / segments * 2.0f * 3.1417;
			float x = cos(theta) * coneBaseRadius;
			float y = sin(theta) * coneBaseRadius;
			float z = attenuationRadius;

			vertices[i + 1] = { x, y, z, uLightColor };
		}

		for (unsigned int i = 0; i < (segments + 1); i++) {
			auto& vertex = vertices[i];
			csl::math::Matrix34 mat = csl::math::Matrix34AffineTransformation(worldPos.m_Position, worldPos.m_Rotation);
			csl::math::Vector3 position = { vertex.x, vertex.y, vertex.z };
			csl::math::Vector3 finalPosition = mat.translate(position).translation();
			vertices[i].x = finalPosition.x();
			vertices[i].y = finalPosition.y();
			vertices[i].z = finalPosition.z();
		}

		unsigned int idx = 0;
		for (unsigned int i = 0; i < segments; ++i) {
			indices[idx++] = 0;
			indices[idx++] = i + 1;
		}

		for (unsigned int i = 0; i < segments; ++i) {
			indices[idx++] = i + 1;
			indices[idx++] = ((i + 1) % segments) + 1;
		}

		ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, (segments * 2) * 2);
	}
}
#endif
