#include <utility>
#include <stack>
#include "result.h"

class RegexprParser {
    /*
    Interface for parsing regexpr and counting maximal subword length.
    */
public:
    explicit RegexprParser(std::string regexpr, std::string word)
        : regexpr_(std::move(regexpr)), word_(std::move(word)) {}
    int GetMaxSubwordLength();
    std::string GetWord() const;
    std::string GetRegexpr() const;
    void SetWord(std::string word);
    void SetRegexpr(std::string regexpr);
    std::string GetParsedRegexpr();
    static const int ERROR = -1;
    static const int INF = -2;
private:
    bool CheckAlphabet() const;
    bool ParseCurrentSymbol(std::stack<Result>& stack, char current_symbol);
    void ParseEpsilon(Result& current_result) const;
    void ParseAlphabet(Result& current_result) const;
    void ParseStar(std::stack<Result>& stack, Result& current_result) const;
    void ParsePlus(std::stack<Result>& stack, Result& current_result) const;
    void ParseConcat(std::stack<Result>& stack, Result& current_result) const;
    inline static std::unordered_set<char> const alphabet_ = {'a', 'b', 'c'};
    std::string regexpr_;
    std::string parsed_regexpr_;
    std::string word_;
};

std::string RegexprParser::GetWord() const {
    return word_;
}

std::string RegexprParser::GetRegexpr() const {
    return regexpr_;
}

void RegexprParser::SetWord(std::string word) {
    word_ = std::move(word);
}

void RegexprParser::SetRegexpr(std::string regexpr) {
    regexpr_ = std::move(regexpr);
}

std::string RegexprParser::GetParsedRegexpr() {
    if (parsed_regexpr_.empty() && !regexpr_.empty()) {
        GetMaxSubwordLength();
    }
    return parsed_regexpr_;
}

bool RegexprParser::CheckAlphabet() const {

    // O(n)

    for (const auto& symbol : word_) {
        if (alphabet_.find(symbol) == alphabet_.end()) {
            return false;
        }
    }

    return true;

}

void RegexprParser::ParseEpsilon(Result& current_result) const {

    // O(n)

    int length = word_.length();

    for (int i = 0; i <= length; ++i) {
        // inserting indexes everywhere
        current_result.subword_indexes(i, i) = true;
        current_result.full_indexes(i, i) = true;
        current_result.prefix_indexes(i, i) = true;
        current_result.suffix_indexes(i, i) = true;
    }

}

void RegexprParser::ParseAlphabet(Result& current_result) const {

    // O(n)

    int length = word_.length();

    for (int i = 0; i < length; ++i) {
        if (word_[i] == current_result.expr.front()) {
            // inserting indexes everywhere
            current_result.subword_indexes(i, i + 1) = true;
            current_result.full_indexes(i, i + 1) = true;
            current_result.prefix_indexes(i, i + 1) = true;
            current_result.suffix_indexes(i, i + 1) = true;
        }
    }

}

void RegexprParser::ParseStar(std::stack<Result>& stack, Result& current_result) const {

    // O(n^3 * log(n) + m)

    Result last_result = std::move(stack.top());
    stack.pop();

    current_result.expr = std::move(last_result.expr) + '*';

    int length = word_.length();

    // inserting empty words everywhere
    for (int i = 0; i <= length; ++i) {
        current_result.subword_indexes(i, i) = true;
        current_result.full_indexes(i, i) = true;
        current_result.prefix_indexes(i, i) = true;
        current_result.suffix_indexes(i, i) = true;
    }

    current_result.subword_indexes += last_result.subword_indexes;
    current_result.full_indexes += last_result.full_indexes;
    current_result.prefix_indexes += last_result.prefix_indexes;
    current_result.suffix_indexes += last_result.suffix_indexes;

    int new_subwords = 0;
    do {
        new_subwords = 0;
        for (int i = 0; i <= length; ++i) {
            for (int j = i; j <= length; ++j) {
                for (int k = j; k <= length; ++k) {

                    if (last_result.suffix_indexes(i, j) && current_result.prefix_indexes(j, k)
                    && !current_result.subword_indexes(i, k)) {

                        current_result.subword_indexes(i, k) = true;

                        if (last_result.full_indexes(i, j)) {
                            current_result.prefix_indexes(i, k) = true;
                            if (current_result.full_indexes(j, k)) {
                                current_result.full_indexes(i, k) = true;
                            }
                        }

                        if (current_result.full_indexes(j, k)) {
                            current_result.suffix_indexes(i, k) = true;
                        }

                        ++new_subwords;

                    }

                }
            }
        }
    } while (new_subwords > 0);

}

void RegexprParser::ParsePlus(std::stack<Result>& stack, Result& current_result) const {

    // O(n^2 + m)

    Result rhs = std::move(stack.top());
    stack.pop();
    Result lhs = std::move(stack.top());
    stack.pop();

    current_result.expr = '(' + std::move(lhs.expr) + '+' + std::move(rhs.expr) + ')';

    // just copying everything from lhs and rhs

    current_result.subword_indexes += lhs.subword_indexes;
    current_result.subword_indexes += rhs.subword_indexes;
    current_result.full_indexes += lhs.full_indexes;
    current_result.full_indexes += rhs.full_indexes;
    current_result.prefix_indexes += lhs.prefix_indexes;
    current_result.prefix_indexes += rhs.prefix_indexes;
    current_result.suffix_indexes += lhs.suffix_indexes;
    current_result.suffix_indexes += rhs.suffix_indexes;

}

void RegexprParser::ParseConcat(std::stack<Result>& stack, Result& current_result) const {

    // O(n^3 + m)

    Result rhs = std::move(stack.top());
    stack.pop();
    Result lhs = std::move(stack.top());
    stack.pop();

    current_result.expr = '(' + std::move(lhs.expr) + std::move(rhs.expr) + ')';

    current_result.subword_indexes += lhs.subword_indexes;
    current_result.subword_indexes += rhs.subword_indexes;
    current_result.prefix_indexes += lhs.prefix_indexes;
    current_result.suffix_indexes += rhs.suffix_indexes;

    int length = word_.length();

    // concat lhs suffixes with rhs prefixes
    for (int i = 0; i <= length; ++i) {
        for (int j = i; j <= length; ++j) {
            for (int k = j; k <= length; ++k) {

                if (lhs.suffix_indexes(i, j) && rhs.prefix_indexes(j, k)) {

                    current_result.subword_indexes(i, k) = true;

                    if (lhs.full_indexes(i, j)) {
                        current_result.prefix_indexes(i, k) = true;
                        if (rhs.full_indexes(j, k)) {
                            current_result.full_indexes(i, k) = true;
                        }
                    }

                    if (rhs.full_indexes(j, k)) {
                        current_result.suffix_indexes(i, k) = true;
                    }

                }

            }
        }
    }

}

bool RegexprParser::ParseCurrentSymbol(std::stack<Result>& stack, char current_symbol) {

    // O(n^3 * log(n) + m)

    Result current_result(current_symbol, word_.length());

    if (current_symbol == '1') {

        ParseEpsilon(current_result);

    } else if (alphabet_.find(current_symbol) != alphabet_.end()) {

        ParseAlphabet(current_result);

    } else if (current_symbol == '*') {

        if (stack.empty()) {
            // requires 1 argument
            return false;
        }
        ParseStar(stack, current_result);

    } else if (current_symbol == '+') {

        if (stack.size() < 2) {
            // requires 2 arguments
            return false;
        }
        ParsePlus(stack, current_result);

    } else if (current_symbol == '.') {

        if (stack.size() < 2) {
            // requires 2 arguments
            return false;
        }
        ParseConcat(stack, current_result);

    } else {
        // regexpr is incorrect
        return false;
    }

    // parsing is correct, pushing result to stack
    stack.emplace(current_result);
    return true;

}

int RegexprParser::GetMaxSubwordLength() {

    // O(m * n^3 * log(n) + m^2)

    if (!CheckAlphabet()) {
        return ERROR;
    }

    std::stack<Result> stack;
    int regexpr_length = regexpr_.length();

    // parsing each regexpr symbol
    for (int i = 0; i < regexpr_length; ++i) {
        if (!ParseCurrentSymbol(stack, regexpr_[i])) {
            return ERROR;
        }
    }

    if (regexpr_length == 0) {
        // cannot parse a word with empty regexpr
        return ERROR;
    }

    Result result = std::move(stack.top());
    parsed_regexpr_ = result.expr;

    if (stack.size() > 1 || stack.empty()) {
        // some regexpr parts haven't been combined => incorrect regexpr
        return ERROR;
    }

    int length = word_.length();
    int max_subword_length = 0;
    for (int i = 0; i <= length; ++i) {
        for (int j = i; j <= length; ++j) {
            if (result.subword_indexes(i, j) && (j - i > max_subword_length)) {
                max_subword_length = j - i;
            }
        }
    }

    return max_subword_length;

}
