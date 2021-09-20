#include "MatrixIterator.h"

MatrixIterator::MatrixIterator(const Matrix& matrix) : matrix(matrix) {
    this->current_row = 0;
    this->current_column = 0;
    this->stack_size = 0;
    this->elems_stack = new Matrix::MatrixNode*[this->matrix.nr_elems];
    Matrix::MatrixNode* aux = this->matrix.root;
    while (aux != NULL) {
        this->elems_stack[this->stack_size] = aux;
        this->stack_size += 1;
        aux = aux->left;
    }
    if (this->stack_size > 0) {
        this->current_node = this->elems_stack[this->stack_size - 1];
    }
    else {
        this->current_node = NULL;
    }
}

bool MatrixIterator::valid() const {
    if (this->current_node == NULL || this->current_row >= this->matrix.nr_columns || this->current_column >= this->matrix.nr_lines) {
        return false;
    }
    return true;
}

TElem MatrixIterator::getCurrent() const {
    if (this->current_node == NULL || this->current_row >= this->matrix.nr_columns || this->current_column >= this->matrix.nr_lines) {
        throw std::exception();
    }
    if (this->current_row == this->current_node->row && this->current_column == this->current_node->column) {
        return this->current_node->value;
    }
    else {
        return NULL_TELEM;
    }
}

void MatrixIterator::next() {
    if (this->current_node == NULL || this->current_row >= this->matrix.nr_columns || this->current_column >= this->matrix.nr_lines) {
        throw std::exception();
    }
    if (this->current_row == this->current_node->row && this->current_column == this->current_node->column) {
        Matrix::MatrixNode* node = this->elems_stack[this->stack_size - 1];
        this->stack_size -= 1;
        if (node->right != NULL) {
            node = node->right;
            while (node != NULL) {
                this->elems_stack[this->stack_size] = node;
                this->stack_size += 1;
                node = node->left;
            }
        }
        if (this->stack_size > 0) {
            this->current_node = this->elems_stack[this->stack_size - 1];
        }
        else {
            this->current_node = NULL;
        }
    }
    else {

    }
    this->current_column += 1;
    if (this->current_column == this->matrix.nr_columns) {
        this->current_column = 0;
        this->current_row += 1;
    }
}