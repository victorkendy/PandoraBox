#ifndef tensormodel_h_
#define tensormodel_h_

#include "pbge/gfx/Model.h"

namespace pbge {

    // Only symmetric 2D and 3D matrices and diagonal matrices of any order (others are not implemented yet)
    class PBGE_EXPORT TensorModel : public Model {
    public:
        TensorModel(float ** tensor, int order, int slices = 16);
        ~TensorModel();
        float getEigenvalue(int index);
        float * getEigenvector(int index);
        
        void render(ModelInstance * instance, GraphicAPI * ogl);
        void renderDepth(ModelInstance * instance, GraphicAPI * ogl);
    private:
        float ** tensor;
        int order;
        float * eigenvalues;
        float ** eigenvectors;
        bool calculated;
        int slices;
        Model * model;

        // Formulas based on Kingsley, Peter B. (2005). Introduction to Diffusion Tensor Imaging Mathematics: Part I. Tensors, Rotations, and Eigenvectors. Concepts in Magnetic Resonance Part A (Bridging Education and Research): 117-119
        void calculateEigenValuesAndVectors();
        void calculateEigenValuesAndVectors2d();
        void calculateEigenValues2dSymmetric();
        void calculateEigenVectors2dSymmetric();
        void calculateEigenValuesAndVectors3d();
        void calculateEigenValuesAndVectorsDiagonal();
        void calculateEigenValues3dSymmetric();
        void calculateEigenVectors3dSymmetric();
        
        void render2d(ModelInstance * instance, GraphicAPI * ogl);
        void render3d(ModelInstance * instance, GraphicAPI * ogl);

        bool tensorIsDiagonal();
        bool tensorIsSymmetric();
    };

}
#endif