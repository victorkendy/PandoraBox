
#ifndef TESTS_MOCKS_MOCKVISITORS_H_
#define TESTS_MOCKS_MOCKVISITORS_H_

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

class MockRenderVisitor : public pbge::RenderVisitor {
public:
    MOCK_METHOD2(visit, void(pbge::Node * node, pbge::GraphicAPI * ogl));
};

#endif