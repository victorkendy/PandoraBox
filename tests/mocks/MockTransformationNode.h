#include <gmock/gmock.h>

#include "pbge/gfx/Node.h"


class MockTransformationNode : public pbge::TransformationNode {
public:
    MOCK_METHOD1(update, math3d::matrix44*(math3d::matrix44* m));
    MOCK_METHOD0(getTransformationMatrix, math3d::matrix44*());
    MOCK_METHOD1(accept, void(pbge::NodeVisitor * visitor));
};