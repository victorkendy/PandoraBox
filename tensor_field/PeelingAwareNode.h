#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include "pbge/pbge.h"

class PeelingAwareNode : public pbge::ModelCollection {
public:
    PeelingAwareNode(pbge::Model * _model, pbge::GPUProgram * _peelingProgram) : pbge::ModelCollection(_model), peelingProgram(_peelingProgram) {}
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->peelingProgram);
        gfx->updateState();

        pbge::VBOModel * vboModel = dynamic_cast<pbge::VBOModel *>(getModel());
        gfx->getDrawController()->drawVBOModel(vboModel, getNumberOfInstances());
    }
    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }
private:
    pbge::GPUProgram * peelingProgram;
};

#endif