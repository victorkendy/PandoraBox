#ifndef gfx_light_h
#define gfx_light_h

#include "math3d/math3d.h"

#include "pbge/gfx/Node.h"
#include "pbge/gfx/Shader.h"
//
//
//class PBGE_EXPORT Light: public Attachable {
//public:
//    gfxLight();
////    gfxLight(const vector4 & diffuseColor, const vector4 & specularColor, const int & specularSponent, const int & isOn);
//
//    void turnOn();
//    void turnOff();
//
//    int  isLightOn();
//
//    void setSpecularColor(const math3d::vector4 & color);
//    void setSpecularColor(const float & red, const float & green, const float & blue, const float & alpha);
//    void setDiffuseColor(const math3d::vector4 & color);
//    void setDiffuseColor(const float & red, const float & green, const float & blue, const float & alpha);
//
//    void setSpecularSponent(const int & exponent);
//
//    void setLinearAttenuation(const float & factor);
//    void setQuadraticAttenuation(const float & factor);
//    void setConstantAttenuation(const float & factor);
//    
//    virtual const gfxGPUProgram * getLightShader()=0;
//protected:
//    int isOn;
//    math3d::vector4 diffuse, specular;
//    float   constantAttenuation, linearAttenuation, quadraticAttenuation;
//    int     specularExponent;
//};


/*
class gfxSpotLight : public gfxLight {
public:
    gfxSpotLight();
    gfxSpotLight(const vector4 & diffuseColor, const vector4 & specularColor, const int & specularSponent, const int & isOn);
private:
    math3d::vector4 direction;
    gfxGPUProgram lightShader;
}*/

#endif
