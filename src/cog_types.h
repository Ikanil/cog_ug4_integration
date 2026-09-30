#pragma once
#include <vector>

// Weltkoordinaten von UG4
struct  WorldPointType
{
    double x;
    double y;
};


// Punkt als Rasterposition
struct PixelPointType
{
    double col;
    double row;
};


// BoundingBox in Weltkoordinaten
struct WorldBoundingBoxType
{
    double minX;
    double minY;
    double maxX;
    double maxY;
};


// die Pixelkoordinaten Version der boundingBox
struct PixelWindowType
{
    int xOffset;
    int yOffset;
    int width;
    int height;
};


// GeoTransform
struct GeoTransformType
{
    double originX;
    double pixelWidth;
    double rotationX;

    double originY;
    double rotationY;
    double pixelHeight;
};


// Zwischenspeicher zwischen prepare und eval. 
struct PreparedWindowType
{
    PixelWindowType window;
    int width;
    int height;
    std::vector<double> values;
};

struct RasterInfoType
{
    int width;
    int height;
    int bandCount;
};