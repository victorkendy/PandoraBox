#ifndef tensormodel_h_
#define tensormodel_h_

#include "pbge/gfx/Model.h"

namespace pbge {

    // Only symmetric 2D matrices (others are not implemented yet)
    class PBGE_EXPORT TensorModel : public Model {
    public:
        TensorModel(float ** tensor, int order);
        ~TensorModel();
        float getEigenvalue(int index);
        float * getEigenvector(int index);

        void render(ModelInstance * instance, OpenGL * ogl);
        void renderDepth(ModelInstance * instance, OpenGL * ogl);
    private:
        float ** tensor;
        int order;
        float * eigenvalues;
        float ** eigenvectors;
        bool calculated;

        // Formulas based on Kingsley, Peter B. (2005). Introduction to Diffusion Tensor Imaging Mathematics: Part I. Tensors, Rotations, and Eigenvectors. Concepts in Magnetic Resonance Part A (Bridging Education and Research): 117-119
        void calculateEigenValuesAndVectors();
        void calculateEigenValuesAndVectors2d();
        void calculateEigenValues2dSymmetric();
        void calculateEigenVectors2dSymmetric();
        bool tensorIsSymmetric();
    };

}
#endif