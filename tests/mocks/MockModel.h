
#include <gmock/gmock.h>
#include "pbge/gfx/Model.h"

class MockModel : public pbge::Model {
    MOCK_METHOD2(render, void(pbge::ModelInstance * instance, pbge::GraphicAPI * ogl));

    MOCK_METHOD2(renderDepth, void(pbge::ModelInstance * instance, pbge::GraphicAPI * ogl));
};

class MockModelInstance : public pbge::ModelInstance {
public:
    MockModelInstance() : pbge::ModelInstance(NULL) {}
    MockModelInstance(pbge::Model * model) : pbge::ModelInstance(model) {}

    MOCK_METHOD1(render, void(pbge::GraphicAPI * ogl));
};