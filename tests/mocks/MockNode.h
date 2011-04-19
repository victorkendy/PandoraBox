#include <gmock/gmock.h>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Node.h"


class MockNode : public pbge::Node {
public:
    MOCK_METHOD2(updatePass, void(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * ogl));
    MOCK_METHOD2(postUpdatePass, void(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * ogl));
    MOCK_METHOD2(renderPass, void(pbge::RenderVisitor * visitor, pbge::GraphicAPI * ogl));
    MOCK_METHOD2(postRenderPass, void(pbge::RenderVisitor * visitor, pbge::GraphicAPI * ogl));
    MOCK_METHOD2(depthPass, void(pbge::RenderVisitor * visitor, pbge::GraphicAPI * ogl));
    MOCK_METHOD2(postDepthPass, void(pbge::RenderVisitor * visitor, pbge::GraphicAPI * ogl));

    pbge::Node::node_list & getChilds() {
        return childs;
    }

    void addChild(pbge::Node * node) {
        childs.push_back(node);
    }

    pbge::Node::node_list childs;
};
