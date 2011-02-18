
#include "vbo_setup.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/Model.h"

void createNormalIndexes(std::vector<unsigned short> & ni) {
    unsigned short indexes[] = {3,3,3,3, 5,5,5,5, 0,0,0,0, 2,2,2,2, 1,1,1,1, 4,4,4,4};
    ni = std::vector<unsigned short>(indexes, indexes + 24);
}

void createVertexIndexes(std::vector<unsigned short> & vi) {
    unsigned short indexes[] = {0,1,2,3, 7,4,1,0, 6,5,4,7, 3,2,5,6, 5,2,1,4, 0,3,6,7};
    vi = std::vector<unsigned short>(indexes, indexes + 24);
}

pbge::ModelInstance * createVBOInstance(pbge::OpenGL * ogl) {
    float v = 0.5f;
    std::vector<unsigned short> vIndexes;
    std::vector<unsigned short> nIndexes;
    createNormalIndexes(nIndexes);
    createVertexIndexes(vIndexes);
    pbge::VertexBufferBuilder builder(24);
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    pbge::VertexAttribBuilder normal = builder.addAttrib(3, pbge::VertexAttrib::NORMAL);
    pbge::VertexAttribBuilder color = builder.addAttrib(3, pbge::VertexAttrib::COLOR);

    builder.pushValue(normal,1,0,0).pushValue(0,1,0).pushValue(0,0,1).pushValue(-1,0,0).pushValue(0,-1,0).pushValue(0,0,-1).setAttribIndex(nIndexes);
    
    builder.on(vertex).pushValue(-v,-v,-v).pushValue(-v,v,-v).pushValue(-v,v,v).pushValue(-v,-v,v)
                      .pushValue(v,v,-v).pushValue(v,v,v).pushValue(v,-v,v).pushValue(v,-v,-v).setAttribIndex(vIndexes);

    builder.pushValue(color, 0,0,0).pushValue(0,1,0).pushValue(0,1,1).pushValue(0,0,1);
    builder.pushValue(1,1,0).pushValue(1,1,1).pushValue(1,0,1).pushValue(1,0,0).setAttribIndex(vIndexes);

    pbge::VertexBuffer * vbo = builder.done(GL_STATIC_DRAW, ogl);
    return new pbge::ModelInstance(new pbge::VBOModel(vbo, GL_QUADS));
}