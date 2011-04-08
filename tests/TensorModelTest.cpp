#include <gtest/gtest.h>

#include "pbge/tensor/TensorModel.h"

float** alloc2DFloatMatrix() {
    float ** matrix = (float**)malloc(2*sizeof(float*));
    matrix[0] = (float*)malloc(2*sizeof(float));
    matrix[1] = (float*)malloc(2*sizeof(float));
    return matrix;
}

void setMatrixValues(float ** matrix, float val00, float val01, float val10, float val11) {
    matrix[0][0] = val00;
    matrix[0][1] = val01;
    matrix[1][0] = val10;
    matrix[1][1] = val11;
}

float ** multiply2DFloatMatrices(float ** matrix1, float ** matrix2) {
    float ** result = alloc2DFloatMatrix();
    result[0][0] = matrix1[0][0] * matrix2[0][0] + matrix1[0][1] * matrix2[1][0];
    result[0][1] = matrix1[0][0] * matrix2[0][1] + matrix1[0][1] * matrix2[1][1];
    result[1][0] = matrix1[1][0] * matrix2[0][0] + matrix1[1][1] * matrix2[1][0];
    result[1][1] = matrix1[1][0] * matrix2[0][1] + matrix1[1][1] * matrix2[1][1];

    return result;
}

class TensorModelTest : public testing::Test {
public:
    pbge::TensorModel *tensorModel;
    float **tensor;

    void SetUp() {
        initTensor2D();
        tensorModel = new pbge::TensorModel(tensor, 2);
    }

    ~TensorModelTest() {
        delete tensorModel;
        free(tensor[0]);
        free(tensor[1]);
        free(tensor);
    }
private:
    void initTensor2D() {
        tensor = alloc2DFloatMatrix();
        setMatrixValues(tensor, 6.0, 2.0, 2.0, 3.0);
    }
};

void assert_2d_matrices_equal(float ** matrix1, float ** matrix2) {
    ASSERT_FLOAT_EQ(matrix1[0][0], matrix2[0][0]);
    ASSERT_FLOAT_EQ(matrix1[0][1], matrix2[0][1]);
    ASSERT_FLOAT_EQ(matrix1[1][0], matrix2[1][0]);
    ASSERT_FLOAT_EQ(matrix1[1][1], matrix2[1][1]);
}

void assert_eigenvector_definition(float ** matrix, float * eigenvector1, float * eigenvector2, float eigenvalue1, float eigenvalue2) {
    float ** eigenvalues = alloc2DFloatMatrix();
    setMatrixValues(eigenvalues, eigenvalue1, 0, 0, eigenvalue2);
    float ** eigenvectors = alloc2DFloatMatrix();
    setMatrixValues(eigenvectors, eigenvector1[0], eigenvector2[0], eigenvector1[1], eigenvector2[1]);
    float ** tensor_vectors_product = multiply2DFloatMatrices(matrix, eigenvectors);
    float ** values_vectors_product = multiply2DFloatMatrices(eigenvectors, eigenvalues);
    assert_2d_matrices_equal(tensor_vectors_product, values_vectors_product);
}

TEST_F(TensorModelTest, givenA2DSymmetricMatrixCalculatesEigenvalues) {
    float eigenvalue1 = tensorModel->getEigenvalue(0);
    float eigenvalue2 = tensorModel->getEigenvalue(1);
    ASSERT_FLOAT_EQ(7.0, eigenvalue1);
    ASSERT_FLOAT_EQ(2.0, eigenvalue2);
}

TEST_F(TensorModelTest, givenA2DSymmetricMatrixCalculatesNormalizedEigenvectors) {
    float* eigenvector1 = tensorModel->getEigenvector(0);
    float* eigenvector2 = tensorModel->getEigenvector(1);
    
    // assert eigenvectors are normalized
    ASSERT_FLOAT_EQ(1.0, eigenvector1[0]*eigenvector1[0] + eigenvector1[1]*eigenvector1[1]);
    ASSERT_FLOAT_EQ(1.0, eigenvector2[0]*eigenvector2[0] + eigenvector2[1]*eigenvector2[1]);
    
    // assert eigenvectors are really eigenvectors
    float eigenvalue1 = tensorModel->getEigenvalue(0);
    float eigenvalue2 = tensorModel->getEigenvalue(1);
    assert_eigenvector_definition(tensor, eigenvector1, eigenvector2, eigenvalue1, eigenvalue2);
}

