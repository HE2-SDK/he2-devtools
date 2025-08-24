#pragma once

void Viewer(const char* label, hh::needle::Texture& texture);
bool Editor(const char* label, hh::needle::intrusive_ptr<hh::needle::Texture>& texture);
bool Editor(const char* label, hh::needle::Texture*& texture);
bool Editor(const char* label, hh::needle::RsFlagMask& mask);
void Viewer(const char* label, const hh::needle::FxCamera& camera);
void Viewer(const char* label, const hh::gfx::RenderManager::Impl::LocalFxCamera& camera);
bool Editor(const char* label, hh::needle::SupportFX::FxViewport& viewport);
bool Editor(const char* label, hh::needle::SupportFX::FxRenderParam& renderParam);
bool Editor(const char* label, hh::needle::PipelineInfo& pipelineInfo);
bool Editor(const char* label, hh::needle::RenderingPipeline& pipeline);
bool Editor(const char* label, hh::needle::WorldRenderingPipeline& pipeline);
#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::needle::RenderingPipelineMiller& pipeline);
#elif DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::needle::RenderingPipelineRangers& pipeline);
#endif
bool Editor(const char* label, hh::needle::RenderUnit& renderUnit);
bool Editor(const char* label, hh::needle::RenderTextureHandle& renderTexture);
bool Editor(const char* label, hh::needle::MeshResource& meshResource);
bool Editor(const char* label, hh::needle::InstanceParameterContainerData& ipcd);
bool Editor(const char* label, hh::needle::ParameterValueObjectContainer& pvo);
bool Editor(const char* label, hh::needle::ShaderObject& shaderObj);
