#pragma once
#include <cstddef>
#include <exception>
#include "Matrix.h"

class MatrixIterator {
    friend class Matrix;

private:
    const Matrix& matrix;
    int current_row;
    int current_column;
    Matrix::MatrixNode** elems_stack;
    int stack_size;
    Matrix::MatrixNode* current_node;

    MatrixIterator(const Matrix& matrix);

public:
    void next();
    bool valid() const;
    TElem getCurrent() const;
};
