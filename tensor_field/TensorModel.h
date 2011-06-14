#ifndef tensormodel_h_
#define tensormodel_h_

#include "pbge/pbge.h"

class TensorModel : public pbge::Model {
public:
    TensorModel(float ** tensor, int order, int slices = 16);
    ~TensorModel();
    float getEigenvalue(int index);
    float * getEigenvector(int index);
    
    void beforeRender(pbge::GraphicAPI * gfx){}
    void afterRender(pbge::GraphicAPI * gfx){}
    void render(pbge::GraphicAPI * gfx);
    void renderDepth(pbge::GraphicAPI * gfx);
private:
    float ** tensor;
    int order;
    float * eigenvalues;
    float ** eigenvectors;
    bool calculated;
    int slices;
    pbge::Model * model;

    // Formulas based on Kingsley, Peter B. (2005). Introduction to Diffusion Tensor Imaging Mathematics: Part I. Tensors, Rotations, and Eigenvectors. Concepts in Magnetic Resonance Part A (Bridging Education and Research): 117-119
    void calculateEigenValuesAndVectors();
    void calculateEigenValuesAndVectors2d();
    void calculateEigenValues2dSymmetric();
    void calculateEigenVectors2dSymmetric();
    void calculateEigenValuesAndVectors3d();
    void calculateEigenValuesAndVectorsDiagonal();
    void calculateEigenValues3dSymmetric();
    void calculateEigenVectors3dSymmetric();
    
    void render2d(pbge::GraphicAPI * ogl);
    void render3d(pbge::GraphicAPI * ogl);

    bool tensorIsDiagonal();
    bool tensorIsSymmetric();
};

#endif