#ifndef pbge_manager
#define pbge_manager 1

#include "pbge/core/Log.h"
#include "pbge/core/core.h"

#include <string>
#include <vector>

namespace pbge {
    class OpenGL;
    class Window;
    class SceneGraph;

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
        
        /* Implements the singleton pattern */
        static Manager * getInstance();

        static void init(bool test=false);

        OpenGL * getOpenGL() {
            return ogl;
        }

        void _setOpenGL(OpenGL * _ogl) {
            this->ogl = _ogl;
        }
    
    // public window initialization functions
    public:
        void setWindowDimensions(const unsigned & w, const unsigned & h);

        void setFullscreen(const bool & fullscreen);

        void setWindowTitle(const std::string title);

        void setMainSceneGraph(SceneGraph * sceneGraph);

        void displayGraphics();

    private:
        Manager(bool test = false);
        ~Manager();
        Window * window;
        Log * pbgeLog;
        OpenGL * ogl;
        std::vector<std::string> shaderDirectories;
        bool testConfiguration;

        // window management functions
    };
}

#endif //pbge_manager
