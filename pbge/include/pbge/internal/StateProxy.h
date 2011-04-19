

#ifndef PBGE_INTERNAL_STATEPROXY_H_
#define PBGE_INTERNAL_STATEPROXY_H_

#include "pbge/gfx/GraphicAPI.h"

namespace pbge {

    class StateProxy {
    public:
        virtual void apply(GraphicAPI * ogl) = 0;

        virtual void unApply(GraphicAPI * ogl) = 0;

    };

    class EnableMode : public StateProxy{
    public:
        EnableMode(GraphicAPI::Mode mode);

        void apply(GraphicAPI * ogl);

        void unApply(GraphicAPI * ogl);
    private:
        GraphicAPI::Mode mode;
    };

    class DisableMode : public StateProxy{
    public:
        DisableMode(GraphicAPI::Mode mode);

        void apply(GraphicAPI * ogl);

        void unApply(GraphicAPI * ogl);
    private:
        GraphicAPI::Mode mode;
    };
}



#endif //PBGE_INTERNAL_STATEPROXY_H_