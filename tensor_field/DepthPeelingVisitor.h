#ifndef TENSOR_FIELD_DEPTHPEELINGVISITOR_H_
#define TENSOR_FIELD_DEPTHPEELINGVISITOR_H_

#include <vector>

#include "pbge/pbge.h"
#include "PeelingAwareNode.h"

class DepthPeelingVisitor {
public:
    void visit(pbge::Node * node, pbge::GraphicAPI * gfx) {
        PeelingAwareNode * peelingNode = dynamic_cast<PeelingAwareNode *>(node);
        if(peelingNode != NULL) {
            peelingNode->renderPeeling(gfx);
        }
        for(std::vector<pbge::Node *>::iterator it = node->getChildren().begin(); it != node->getChildren().end(); ++it) {
            visit(*it, gfx);
        }
        if(peelingNode != NULL) {
            peelingNode->postRenderPeeling(gfx);
        }
    }
};

#endif