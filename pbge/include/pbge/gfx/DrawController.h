#ifndef PBGE_GFX_DRAWCONTROLLER_H_
#define PBGE_GFX_DRAWCONTROLLER_H_

namespace pbge {
    class Model;
    class VBOModel;

    class DrawController {
    public:
        virtual void draw(Model * model) = 0;
        
        // Implementation of the pseudo instancing
        virtual void draw(Model * model, int times) = 0;

        virtual void drawVBOModel(VBOModel * model) = 0;

        virtual void drawVBOModel(VBOModel * model, int times) = 0;
    };
}

#endif