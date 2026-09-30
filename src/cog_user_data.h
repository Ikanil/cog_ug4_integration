#ifndef COG_USER_DATA_H_
#define COG_USER_DATA_H_
#include "element_user_data.hpp"
#include "cog_config.h"
#include "cog_reader.h"
#include "cog_types.h"
#include "sampler.h"
#include <string>
#include <stdexcept>
#include <iostream>


namespace ug{

template <int dim>
struct COGStrategy : public ElemenEvalStrategy<COGStrategy<dim>, dim>
{
public:
    COGStrategy(){}
    COGStrategy(const std::string& url, int band)
    {
        p_config.url = url;
        p_config.band = band;
        p_reader.open(p_config.url);
    }


    void prepare_element_impl(const GridObject* elem, const MathVector<dim> vCornerCoords[]){

        if (dim <2)
        {
            throw std::runtime_error("COGStrategy benötigt dim >=2");
        }
 
        BoundingBox<dim> box(elem, vCornerCoords);
        WorldBoundingBoxType worldBox;
        worldBox.minX = box.ll[0];
        worldBox.minY = box.ll[1];
        worldBox.maxX = box.ur[0];
        worldBox.maxY = box.ur[1];
      

        p_currentWindow = p_reader.worldBoundsToPixelWindow(worldBox);
        p_currentBlock =p_reader.readWindow(p_currentWindow,p_config.band);
        p_hasPreparedBlock = true;
    }

    
    // vGlobIP ist Punkt innerhalb des elements
    void eval_on_element_impl(double& value, const MathVector<dim>& vGlobIP, number time, int si) const{
        if (!p_hasPreparedBlock)
        {
            throw std::runtime_error("COGStrategy: no prepared raster block");
        }
        if (dim <2)
        {
            throw std::runtime_error("COGStrategy braucht dim >=2");
        }


        WorldPointType point;
        point.x = vGlobIP[0];
        point.y = vGlobIP[1];


        PixelPointType pixel = p_reader.worldToPixel(point);
        value = p_sampler.nearest(p_currentBlock,pixel);
        // sampler liest aus currentblock denn nearest neightbour wert und schreibt ihn in value

    }
protected:
    CogConfig p_config;
    CogReader p_reader;
    Sampler p_sampler;
    PixelWindowType p_currentWindow{};
    PreparedWindowType p_currentBlock{};
    bool p_hasPreparedBlock = false;
};

template <typename TData, int dim>
class COGUserData : public ElementUserData<TData, COGStrategy<dim>, dim>
{
public: 
    COGUserData() : strategy(), ElementUserData<TData, COGStrategy<dim>, dim>(strategy){}
    COGUserData(const std::string& url, int band) : strategy(url, band), ElementUserData<TData, COGStrategy<dim>, dim>(strategy) {} 
protected:
    COGStrategy<dim> strategy;
};

}
#endif