#ifndef tensormodel_h_
#define tensormodel_h_

#include "pbge/gfx/Model.h"

namespace pbge {

    class PBGE_EXPORT TensorModel : public Model {
    public:
        TensorModel(float * tensor, int order);
        ~TensorModel();
        float getEigenvalue(int index);
        float * getEigenvector(int index);
    private:
        float * tensor;
        int order;
        float * eigenvalues;
        float ** eigenvectors;
        boolean calculated;

        void calculateEigenValuesAndVectors();
        void calculateEigenValuesAndVectors2d();
        boolean tensorIsSymmetric();
    };

}
#endif