
#include <gmock/gmock.h>
#include "pbge/gfx/Model.h"

class MockModel : public pbge::Model {
    MOCK_METHOD2(render, void(pbge::ModelInstance * instance, pbge::GraphicAPI * ogl));

    MOCK_METHOD2(renderDepth, void(pbge::ModelInstance * instance, pbge::GraphicAPI * ogl));
};
