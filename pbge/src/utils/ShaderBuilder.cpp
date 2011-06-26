#include <string>
#include <iostream>
#include "boost/regex.hpp"

#include "pbge/utils/ShaderBuilder.h"

using namespace pbge;

void ShaderBuilder::generateShaderParameters() {
    if(!isMainShader()) return;
    
    boost::smatch parametersMatch;
    boost::regex r("void\\s+"+entry+"\\s*\\((.+)\\)", boost::regex::perl);
    
    if(boost::regex_search(sourceCode, parametersMatch, r)) {
        boost::regex paramRegex("(\\w+)\\s+(\\w+)\\s+(\\w+)", boost::regex::perl);
        std::string params = parametersMatch[1].str();
        for(boost::sregex_iterator m(params.begin(), params.end(), paramRegex); m != boost::sregex_iterator(); m++) {
            std::string in = std::string((*m)[1].first, (*m)[1].second);
            std::string type = std::string((*m)[2].first, (*m)[2].second);
            std::string name = std::string((*m)[3].first, (*m)[3].second);
            parameters.push_back(ShaderStageParameter(in, type, name));
        }
    }
}