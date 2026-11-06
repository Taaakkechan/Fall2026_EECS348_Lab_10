#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <utility>

// Simple N x N matrix of ints backed by a dynamically allocated array.
class Matrix {
private:
    int size;
    int* data;

    int& at(int row, int col) { return data[row * size + col]; }
    int at(int row, int col) const { return data[row * size + col]; }

public:
    explicit Matrix(int n = 0) : size(n), data(n > 0 ? new int[n * n]() : nullptr) {}

    // Copy constructor (needed so the Matrix can be passed by value safely)
    Matrix(const Matrix& other) : size(other.size), data(other.size > 0 ? new int[other.size * other.size] : nullptr) {
        for (int i = 0; i < size * size; i++) {
            data[i] = other.data[i];
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            Matrix temp(other);
            std::swap(size, temp.size);
            std::swap(data, temp.data);
        }
        return *this;
    }

    ~Matrix() { delete[] data; }

    int getSize() const { return size; }

    int get(int row, int col) const { return at(row, col); }

    void set(int row, int col, int value) { at(row, col) = value; }

    bool isValidIndex(int index) const { return index >= 0 && index < size; }

    // Reads size*size values from the stream into this matrix.
    bool readFrom(std::istream& in) {
        for (int i = 0; i < size * size; i++) {
            if (!(in >> data[i])) {
                return false;
            }
        }
        return true;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                std::cout << std::setw(4) << at(i, j);
            }
            std::cout << std::endl;
        }
    }

    Matrix operator+(const Matrix& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes do not match for addition.");
        }
        Matrix result(size);
        for (int i = 0; i < size * size; i++) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes do not match for multiplication.");
        }
        Matrix result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += at(i, k) * other.at(k, j);
                }
                result.at(i, j) = sum;
            }
        }
        return result;
    }

    int mainDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += at(i, i);
        }
        return sum;
    }

    int secondaryDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += at(i, size - 1 - i);
        }
        return sum;
    }

    void swapRows(int r1, int r2) {
        for (int j = 0; j < size; j++) {
            std::swap(at(r1, j), at(r2, j));
        }
    }

    void swapCols(int c1, int c2) {
        for (int i = 0; i < size; i++) {
            std::swap(at(i, c1), at(i, c2));
        }
    }
};

// Reads N followed by two N x N matrices from the given file.
bool readMatricesFromFile(const std::string& filename, Matrix& a, Matrix& b) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error: could not open file " << filename << std::endl;
        return false;
    }

    int n;
    if (!(file >> n) || n <= 0) {
        std::cout << "Error: invalid matrix size in file." << std::endl;
        return false;
    }

    a = Matrix(n);
    b = Matrix(n);
    if (!a.readFrom(file) || !b.readFrom(file)) {
        std::cout << "Error: not enough matrix data in file." << std::endl;
        return false;
    }
    return true;
}

// Problem 4
void printDiagonalSums(const Matrix& m) {
    std::cout << "Main diagonal sum: " << m.mainDiagonalSum() << std::endl;
    std::cout << "Secondary diagonal sum: " << m.secondaryDiagonalSum() << std::endl;
}

// Problem 5: matrix is passed by value so the original is not modified.
void swapRows(Matrix m, int r1 = 0, int r2 = 1) {
    if (!m.isValidIndex(r1) || !m.isValidIndex(r2)) {
        std::cout << "Error: invalid row indices." << std::endl;
        return;
    }
    m.swapRows(r1, r2);
    std::cout << "Problem 5 - Rows " << r1 << " and " << r2 << " swapped:" << std::endl;
    m.print();
}

// Problem 6
void swapCols(Matrix m, int c1 = 0, int c2 = 1) {
    if (!m.isValidIndex(c1) || !m.isValidIndex(c2)) {
        std::cout << "Error: invalid column indices." << std::endl;
        return;
    }
    m.swapCols(c1, c2);
    std::cout << "Problem 6 - Columns " << c1 << " and " << c2 << " swapped:" << std::endl;
    m.print();
}

// Problem 7
void updateElement(Matrix m, int row = 0, int col = 0, int value = 100) {
    if (!m.isValidIndex(row) || !m.isValidIndex(col)) {
        std::cout << "Error: invalid row or column index." << std::endl;
        return;
    }
    m.set(row, col, value);
    std::cout << "Problem 7 - Updated matrix:" << std::endl;
    m.print();
}

int main() {
    std::string filename;
    std::cout << "Enter input filename: ";
    std::cin >> filename;
    std::cout << std::endl;

    Matrix a, b;
    if (!readMatricesFromFile(filename, a, b)) {
        return 1;
    }

    // Problem 1
    std::cout << "Matrix A:" << std::endl;
    a.print();
    std::cout << std::endl;

    std::cout << "Matrix B:" << std::endl;
    b.print();
    std::cout << std::endl;

    // Problem 2
    Matrix sum = a + b;
    std::cout << "A + B:" << std::endl;
    sum.print();
    std::cout << std::endl;

    // Problem 3
    Matrix product = a * b;
    std::cout << "A * B:" << std::endl;
    product.print();
    std::cout << std::endl;

    // Problem 4
    std::cout << "Diagonal sums for Matrix A:" << std::endl;
    printDiagonalSums(a);
    std::cout << std::endl;

    // Problem 5
    swapRows(a, 0, 2);
    std::cout << std::endl;

    // Problem 6
    swapCols(a, 0, 2);
    std::cout << std::endl;

    // Problem 7
    updateElement(a, 1, 2, 99);

    return 0;
}
