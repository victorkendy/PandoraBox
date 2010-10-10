//#include "pbge/gfx/Light.h"
//#include "math3d/vector4.h"
//
//
//gfxLight::gfxLight() {
//    isOn = 0;
//    diffuse = zeroVector;
//    specular = zeroVector;
//    specularExponent = 1;
//
//}
//
//
//
//
//void gfxLight::turnOn() {
//    isOn = 1;
//}
//
//void gfxLight::turnOff() {
//    isOn = 0;
//}
//
//int gfxLight::isLightOn () {
//    return (isOn == 1);
//}
//
//void gfxLight::setSpecularColor(const math3d::vector4 & color) {
//    specular = color;
//}
//
//void gfxLight::setSpecularColor(const float & red, const float & green, const float & blue, const float & alpha) {
//    specular.set(red, green, blue, alpha);
//}
//
//void gfxLight::setDiffuseColor(const math3d::vector4 & color) {
//    diffuse = color;
//}
//
//void gfxLight::setDiffuseColor(const float & red, const float & green, const float & blue, const float & alpha) {
//    diffuse.set(red, green, blue, alpha);
//}
//
//void gfxLight::setSpecularSponent(const int & exponent) {
//    if (exponent >= 0)
//        specularExponent = exponent;
//}
//
//void gfxLight::setLinearAttenuation(const float & factor) {
//    linearAttenuation = factor;
//}
//
//void gfxLight::setQuadraticAttenuation(const float & factor) {
//    quadraticAttenuation = factor;
//}
//
//void gfxLight::setConstantAttenuation(const float & factor) {
//    constantAttenuation = factor;
//}
//
//
