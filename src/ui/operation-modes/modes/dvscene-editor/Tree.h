#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::dvscene_editor {
    struct Category {
        const char* name;
        std::pair<unsigned int, bool> items[32];
    };

#ifdef DEVTOOLS_TARGET_SDK_rangers
    inline constexpr Category categorizedNodes[] = {
        Category{"Camera", {
            {3u, false}, {4u, false}, {1u, true}, {6u, true},
            {7u, true}, {17u, true}, {20u, true}, {21u, true}
        }},
        Category{"Character/Model", {
            {5u, false}, {6u, false}, {8u, false}, {10u, false},
            {11u, false}, {3u, true}, {11u, true}, {13u, true},
            {14u, true}, {15u, true}, {16u, true}, {18u, true},
            {1012u, true}, {1019u, true}, {1022u, true}, {1025u, true}, 
            {1030u, true}
        }},
        Category{"Positioning", {
            {1u, false}, {5u, true}, {10u, true}
        }},
        Category{"Post Processing", {
            {12u, true}, {1000u, true}, {1001u, true}, {1002u, true},
            {1003u, true}, {1004u, true}, {1005u, true}, {1008u, true},
            {1009u, true}, {1017u, true}, {1018u, true}, {1020u, true},
            {1023u, true}, {1027u, true}, {1029u, true}, {1031u, true},
            {1034u, true}, {1035u, true}, {1038u, true}, {1039u, true},
            {1043u, true}
        }},
        Category{"Lighting", {
            {9u, true}, {24u, true}, {26u, true}, {1036u, true}
        }},
        Category{"Screen Effects", {
            {1010u, true}, {1011u, true}, {1014u, true}, {1015u, true},
            {1032u, true}, {1033u, true}, {1037u, true}
        }},
        Category{"Miscellaneous", {
            {8u, true}, {25u, true}, { 1021u, true }, {1024u, true}, 
            {1026u, true}, {1028u, true}, {1041u, true}, {1042u, true}, 
            {1016u, true}
        }}
    };
#elif DEVTOOLS_TARGET_SDK_miller
    inline constexpr Category categorizedNodes[] = {
        Category{"Camera", {
            {3u, false}, {4u, false}, {1u, true}, {6u, true},
            {7u, true}, {17u, true}, {20u, true}, {21u, true}
        }},
        Category{"Character/Model", {
            {5u, false}, {6u, false}, {8u, false}, {10u, false},
            {11u, false}, {3u, true}, {11u, true}, {13u, true},
            {14u, true}, {15u, true}, {16u, true}, {18u, true},
            {23u, true}, {26u, true}, {27u, true}, {1014u, true},
            {1021u, true}, {1024u, true}, {1027u, true}, {1028u, true}, 
            {1032u, true}, {1044u, true}, {1045u, true}
        }},
        Category{"Positioning", {
            {1u, false}, {5u, true}, {10u, true}
        }},
        Category{"Post Processing", {
            {12u, true}, {1000u, true}, {1001u, true}, {1002u, true},
            {1003u, true}, {1004u, true}, {1005u, true}, {1006u, true},
            {1007u, true}, {1010u, true}, {1001u, true}, {1019u, true},
            {1020u, true}, {1022u, true}, {1025u, true}, {1029u, true},
            {1031u, true}, {1033u, true}, {1036u, true}, {1037u, true},
            {1040u, true}, {1041u, true}, {1042u, true}, {1046u, true},
            {1047u, true}, {1048u, true},
        }},
        Category{"Lighting", {
            {9u, true}, {22u, true}, {24u, true}
        }},
        Category{"Screen Effects", {
            {1012u, true}, {1013u, true}, {1016u, true}, {1017u, true},
            {1034u, true}, {1035u, true}
        }},
        Category{"Miscellaneous", {
            {8u, true}, {1023u, true}, {1026u, true}, {1028u, true},
            {1030u, true}, {1043u, true}, {1018u, true}
        }}
    };
#endif

    class Tree : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;

        void RenderNode(dv::DvNode* node);
    };
}
