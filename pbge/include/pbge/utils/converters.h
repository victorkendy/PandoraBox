#ifndef PBGE_UTILS_CONVERTERS_H_
#define PBGE_UTILS_CONVERTERS_H_

#include <sstream>
#include <string>

namespace pbge {
    template<typename Number>
    const Number stringToNumber(const std::string & str) {
        std::istringstream is(str);
        Number value;
        is >> value;
        return value;
    }
}

#endif
