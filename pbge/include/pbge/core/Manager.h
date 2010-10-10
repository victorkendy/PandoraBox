#ifndef pbge_manager
#define pbge_manager 1

#include "pbge/core/Log.h"
#include "pbge/core/core.h"
#include "pbge/core/OpenGLParameters.h"
#include <string>
#include <vector>

namespace pbge {
    class PBGE_EXPORT Manager{
    public:
        /* Write a status or warning message */
        void writeLog(std::string message) { 
            pbgeLog->write(message); 
        }
        
        /* Write internal error messages for the user */
        void writeErrorLog(std::string message) {
            pbgeLog->writeError(message); 
        }
        
        /* Add a new shader source directory to the search path */
        void addShaderDirectory(const std::string & newDir);

        /* Get all the directories in the shader source search path */
        const std::vector<std::string> getShaderDirs() const;

        /* sets a user defined logger */
        void setLog(Log * newLog) {
            pbgeLog = newLog;
        }
        
        /* Initialize the OpenGL Context */
        bool initializeOpenGL(OpenGLParameters parameters);
        void cleanUp();

        /* Implements the singleton pattern */
        static Manager * getInstance();
    private:
        Manager();
        ~Manager();
        static Manager * instance;
        Log * pbgeLog;
        std::vector<std::string> shaderDirectories;
    };
}

#endif //pbge_manager
