#ifndef TENSOR_FIELD_DEPTHPEELINGVISITOR_H_
#define TENSOR_FIELD_DEPTHPEELINGVISITOR_H_

#include <vector>

#include "pbge/pbge.h"
#include "PeelingAwareNode.h"

class DepthPeelingVisitor : public pbge::RenderVisitor {
public:
    void visitAction(pbge::Node * node, pbge::GraphicAPI * gfx) {
        PeelingAwareNode * peelingNode = dynamic_cast<PeelingAwareNode *>(node);
        if(peelingNode != NULL)
            peelingNode->renderPeeling(gfx);
    }
    void postVisitAction(pbge::Node * node, pbge::GraphicAPI * gfx) {
        PeelingAwareNode * peelingNode = dynamic_cast<PeelingAwareNode *>(node);
        if(peelingNode != NULL)
            peelingNode->postRenderPeeling(gfx);
    }
};

#endif