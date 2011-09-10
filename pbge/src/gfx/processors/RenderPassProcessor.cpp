#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/SceneGraph.h"

#include "pbge/gfx/processors/RenderPassProcessor.h"

using namespace pbge;

void RenderPassProcessor::process(GraphicAPI * gfx, Renderer * renderer) {
    Node * node = renderer->getScene()->getSceneGraphRoot();
    visitor->visit(node, gfx);
}