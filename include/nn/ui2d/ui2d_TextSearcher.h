#pragma once

#include <nn/types.h>

namespace nn {
namespace ui2d {

class TextSearcher {
public:
    struct TextInfo {
        const uint16_t* pText;
        int32_t textLength;
        int32_t bufferLength;
        int32_t forceAssignedTextLength;

        void SetDefault();
    };

    struct TextInfoUtf8 {
        const char* pText;
        int32_t textLength;
        int32_t bufferLength;
        int32_t forceAssignedTextLength;

        void SetDefault();
    };
};

}  // namespace ui2d
}  // namespace nn
