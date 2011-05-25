#ifndef PBGE_CORE_DEFINITIONS_H_
#define PBGE_CORE_DEFINITIONS_H_

namespace pbge {
    // MISC
    const unsigned KB = 1024;

    // Numeric constants
    const float pi = 3.1415927f;
    const float inverse_pi = 0.3183099f;

    inline const float degToRad(const float & angle) {
        return (pbge::pi * (angle))/180.0f;
    }

    inline const float radToDeg(const float & angle) {
        return (angle * 180.0f) / pbge::pi;
    }
}
#endif