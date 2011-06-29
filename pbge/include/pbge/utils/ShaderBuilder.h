#ifndef PBGE_UTILS_SHADERBUILDER_H_
#define PBGE_UTILS_SHADERBUILDER_H_

#include <string>
#include <vector>
#include <map>

#include "pbge/core/PairCompare.h"
#include "pbge/core/core.h"
#include "pbge/gfx/Shader.h"

namespace pbge {
    
    class ShaderStageParameter {
    public:
        ShaderStageParameter(const std::string & in, const std::string & parameterType, const std::string & parameterName){
            name = parameterName; type = parameterType;
            input = (in == "in");
        }
        const std::string getName() const { return name; }
        const std::string getType() const { return type; }
        const bool isInput() const { return input; }
        bool operator < (const ShaderStageParameter & other) const {
            return PairCompare<std::string,std::string>::less(getType(),other.getType(), getName(),other.getName());
        }
    private:
        bool input;
        std::string name;
        std::string type;
    };



    class PBGE_EXPORT ShaderBuilder {
    public:
        ShaderBuilder(const std::string & source, const std::string & entryPoint, const Shader::ShaderType & shaderType) 
            :sourceCode(source), entry(entryPoint), type(shaderType) {
            generateShaderParameters();
        }

        bool isMainShader() {
            return entry != "";
        }

        const std::vector<ShaderStageParameter> & getParameters() {
            return parameters;
        }
    private:
        void generateShaderParameters();

        std::string sourceCode;
        std::string entry;
        std::vector<ShaderStageParameter> parameters;
        Shader::ShaderType type;
    };


    class ShaderCallParameter {
        
    };
/*
    class PBGE_EXPORT ParameterInjector {
    public:
        ShaderCallParameter * getParameter(const ShaderStageParameter & param){}
        void registerCallParameter(ShaderStageParameter paramSpec, ShaderCallParameter * param){
            params[paramSpec] = param;
        }
    private:
        std::map<ShaderStageParameter, ShaderCallParameter*> params;
    };
*/

    class ShaderProgramBuilder {
    public:
        void useShader(const std::string & source, const std::string & entryPoint, const Shader::ShaderType & type){}
    };
}
#endif
