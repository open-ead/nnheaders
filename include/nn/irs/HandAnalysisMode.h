#pragma once

namespace nn::irsensor {
enum HandAnalysisMode {
    HandAnalysisMode_None,
    HandAnalysisMode_Silhouette,
    HandAnalysisMode_Image,
    HandAnalysisMode_SilhouetteAndImage,
    HandAnalysisMode_SilhouetteOnly,  // [4.0.0+]
};
}  // namespace nn::irsensor
