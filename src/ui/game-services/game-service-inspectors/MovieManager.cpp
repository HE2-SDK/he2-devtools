#include "EventPlayer.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void FrameStrEditor(const char* label, hh::fmv::MoviePlayerCri::FrameInfo* str) {
    if (ImGui::TreeNode(label)) {
        Editor("framesNum", str->framesNum);
        Editor("framesNumPerFile", str->framesNumPerFile);
        Editor("width", str->width);
        Editor("height", str->height);
        Editor("displayWidth", str->displayWidth);
        Editor("displayHeight", str->displayHeight);
        Editor("framerate", str->framerate);
        Editor("framerateN", str->framerateN);
        Editor("framerateD", str->framerateD);
        Editor("totalFrames", str->totalFrames);
        Editor("time", str->time);
        Editor("timePerFile", str->timePerFile);
        Editor("tunit", str->tunit);
        Editor("numConcatenatedMovie", str->numConcatenatedMovie);
        Editor("numImages", str->numImages);
        Editor("flag", str->flag);
        Editor("alphaType", str->alphaType);
        Editor("refResult", str->refResult);
        Editor("colorConversionType", str->colorConversionType);
        Editor("cntSkippedFrames", str->cntSkippedFrames);
        ImGui::TreePop();
    }
}

void RenderGameServiceInspector(hh::fmv::MovieManager& service)
{
    if(ImGui::TreeNode("Movies")){
        for (auto& x : service.movies) {
            char buffer[512];
            snprintf(buffer, sizeof(buffer), "%s##%zx", x->moviePlayer->usmFilePath, (size_t)*x);
            if (ImGui::TreeNode(buffer)) {
                auto* movPly = x->moviePlayer;
                Viewer("Filepath", movPly->usmFilePath);
                Viewer("Progress", movPly->secondsRunning);
                Viewer("Frames Running", movPly->framesRunning);
                Editor("renderPass", movPly->renderPass);
                Editor("renderPriority", movPly->renderPriority);
                Editor("flags", movPly->flags);
                Editor("setupDataUnk3", movPly->setupDataUnk3);
                Editor("setupDataUnk4", movPly->setupDataUnk4);
                Editor("criManaPlayerStatus", movPly->criManaPlayerStatus);
                if (ImGui::TreeNode("Texture Infos")) {
                    for (auto& y : movPly->textureInfos) {
                        char xbuffer[128];
                        snprintf(xbuffer, sizeof(xbuffer), "0x%zx", (size_t)&y);
                        if (ImGui::TreeNode(xbuffer)) {
                            Editor("width", y.width);
                            Editor("height", y.height);
                            Editor("unk4", y.unk4);
                            Editor("linePitch", y.linePitch);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                FrameStrEditor("Frame Info", movPly->frameInfo);
                FrameStrEditor("Temp Frame Info 0", movPly->tempFrameInfo0);
                FrameStrEditor("Temp Frame Info 1", movPly->tempFrameInfo1);
                Viewer("loadingFrame", movPly->loadingFrame);
                Viewer("unk10", movPly->unk10);
                Viewer("unk11", movPly->unk11);
                Viewer("unk12", movPly->unk12);
                Viewer("unk13", movPly->unk13);
                Viewer("unk14", movPly->unk14);
                Viewer("unk17", movPly->unk17);
                Viewer("unk18", movPly->unk18);
                Viewer("unk19", movPly->unk19);
                Viewer("unk20", movPly->unk20);
                Viewer("unk21", movPly->unk21);
                Viewer("unk22", movPly->unk22);
                Viewer("unk23", movPly->unk23);
                Viewer("hasAudio", movPly->hasAudio);
                Viewer("isCriManaLoaded", movPly->isCriManaLoaded);
                Viewer("unk26", movPly->unk26);
                Viewer("unk27", movPly->unk27);
                Viewer("unk28", movPly->unk28);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    Editor("paused", service.paused);
    //Viewer("shader name", service.shaderName);
}
