/*
16. Даны \alpha и слово u \in {a,b,c}*.
Найти длину самого длинного подслова u, являющегося также подсловом некоторого слова в L.
*/

/*
n = word.length();
m = regexpr.length();
Asymptotic:
 * iterating over the word - O(n)
 * iterating over subword indexes - O(n^2)
 * modifying regexpr - O(m) (is needed only for debug and for getting parsed regexpr, can be removed)
 * iterating over regexpr - O(m)
*/

#include <iostream>
#include "regexpr_parser.h"

int main() {

    std::string regexpr;
    std::string word;
    std::cin >> regexpr >> word;

    int max_subword_length = RegexprParser(regexpr, word).GetMaxSubwordLength();

    if (max_subword_length == RegexprParser::ERROR) {
        std::cout << "ERROR\n";
    } else if (max_subword_length == RegexprParser::INF) {
        std::cout << "INF\n";
    } else {
        std::cout << max_subword_length << '\n';
    }

    return 0;
}
