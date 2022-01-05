#include <string>
#include "matrix.h"

struct Result {
    /*
    Element of parsing stack.
    It contains current regexpr part,
    indexes of subwords that fit this part
    and maximal subword length for this part.
    */
    explicit Result(std::string expr, int size)
        : expr(std::move(expr)),
        subword_indexes(size), full_indexes(size), prefix_indexes(size), suffix_indexes(size) {}
    explicit Result(char symbol, int size):
        subword_indexes(size), full_indexes(size),
        prefix_indexes(size), suffix_indexes(size) { expr.push_back(symbol); }
    std::string expr;
    Matrix subword_indexes;  // subwords detected by regexpr subword
    Matrix full_indexes;  // subwords detected by full regexpr
    Matrix prefix_indexes;  // subwords detected by regexpr prefix
    Matrix suffix_indexes;  // subwords detected by regexpr suffix
};
