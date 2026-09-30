#pragma once
#include <string>


enum class InterpolationModeConfig
{
    Nearest
};


struct CogConfig
{
    std::string url;
    int band = 1;
    InterpolationModeConfig interpolation = InterpolationModeConfig::Nearest;
};