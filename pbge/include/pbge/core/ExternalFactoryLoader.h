
#ifndef PBGE_CORE_EXTERNALFACTORYLOADER_H_
#define PBGE_CORE_EXTERNALFACTORYLOADER_H_

#include <windows.h>
#include <atlstr.h>

namespace pbge {

    typedef void * (__cdecl * MYPROC)();

    /** A helper class that loads a dynamic link library.
        This class searches for exported function pointers that are defined within extern "C" blocks.

        @todo Generalize this class for other SOs. Currently only windows will compile.
    */
    template <typename T>
    class ExternalFactoryLoader {
    public:
        /** Constructor for ExternalFactoryLoader.
            
            @param libraryPath the path to the dynamic link library
            @param functionName the name of the C style factory function
        */
        ExternalFactoryLoader(const std::string & libraryPath, const std::string & functionName) {
            instantiateF = NULL;
            lib = LoadLibrary(CString(libraryPath.c_str()));
            if(lib != NULL) {
                std::cout << "creating library" << std::endl;
                instantiateF = (MYPROC)GetProcAddress(lib, functionName.c_str());
                if(instantiateF != NULL) {
                    std::cout << "Library Loaded successfully" << std::endl;
                } else {
                    std::cout << GetLastError() << std::endl;
                }
            }
        }

        /** Releases the loaded dynamic link library.
        */
        ~ExternalFactoryLoader() {
            std::cout << "Freeing Library" << std::endl;
            FreeLibrary(lib);
        }

        /** Create a instance defined by the type T.

            @return A instance of the type of T
        */
        T * instantiate() {
            return (T*) instantiateF();
        }
    private:
        HINSTANCE lib;

        MYPROC instantiateF;
    };
}

#endif