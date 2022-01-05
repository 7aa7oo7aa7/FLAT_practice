#include <vector>

class Matrix {
public:
    explicit Matrix(int size): size_(size + 1), capacity_((size + 1) * (size + 1)),
        matrix_((size + 1) * (size + 1), false) {}
    auto operator()(int first, int second);
    Matrix& operator+=(const Matrix& other);
    Matrix operator+(const Matrix& other) const;
private:
    std::vector<bool> matrix_;
    int size_;
    int capacity_;
};

auto Matrix::operator()(int first, int second) {
    return matrix_[first * size_ + second];
}

Matrix& Matrix::operator+=(const Matrix& other) {
    // O(n^2)
    for (int i = 0; i < capacity_; ++i) {
        matrix_[i] = matrix_[i] || other.matrix_[i];
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    // O(n^2)
    Matrix result = other;
    result += *this;
    return result;
}
