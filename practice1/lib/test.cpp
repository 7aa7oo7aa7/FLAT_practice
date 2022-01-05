/*
16. Даны \alpha и слово u \in {a,b,c}*.
Найти длину самого длинного подслова u, являющегося также подсловом некоторого слова в L.
*/

#include <iostream>
#include "regexpr_parser.h"

void PrintTestResult(const std::string& test_name, bool result) {
    std::cout << test_name << (result ? " passed." : " failed.") << "\n";
}

void TestNormal() {

    bool result = true;
    RegexprParser parser("", "");

    parser.SetRegexpr("abacaba......");
    parser.SetWord("abacaba");
    result = (parser.GetMaxSubwordLength() == 7);

    if (result) {
        parser.SetRegexpr("a*b*.");
        parser.SetWord("aaabbb");
        result = (parser.GetMaxSubwordLength() == 6);
    }

    if (result) {
        parser.SetRegexpr("a*cb*..");
        parser.SetWord("bbaaacbbbcca");
        result = (parser.GetMaxSubwordLength() == 7);
    }

    if (result) {
        parser.SetRegexpr("ab+c.aba.*.bac.+.+*");
        parser.SetWord("babc");
        result = (parser.GetMaxSubwordLength() == 3);
    }

    if (result) {
        parser.SetRegexpr("acb..bab.c.*.ab.ba.+.+*a.");
        parser.SetWord("abbaa");
        result = (parser.GetMaxSubwordLength() == 5);
    }

    if (result) {
        parser.SetRegexpr("a*a.");
        parser.SetWord("cbcbcbc");
        result = (parser.GetMaxSubwordLength() == 0);
    }

    if (result) {
        parser.SetRegexpr("ab.ba..");
        parser.SetWord("bb");
        result = (parser.GetMaxSubwordLength() == 2);
    }

    if (result) {
        parser.SetRegexpr("a");
        parser.SetWord("cbcbcbc");
        result = (parser.GetMaxSubwordLength() == 0);
    }

    if (result) {
        parser.SetRegexpr("ab.");
        parser.SetWord("cccccccc");
        result = (parser.GetMaxSubwordLength() == 0);
    }

    if (result) {
        parser.SetRegexpr("ab+");
        parser.SetWord("cccccccc");
        result = (parser.GetMaxSubwordLength() == 0);
    }

    PrintTestResult("TestNormal", result);

}

void TestEmptyWord() {

    bool result = true;
    RegexprParser parser("", "");

    parser.SetRegexpr("a");
    result = (parser.GetMaxSubwordLength() == 0);

    if (result) {
        parser.SetRegexpr("1a+");
        result = (parser.GetMaxSubwordLength() == 0);
    }

    PrintTestResult("TestEmptyWord", result);

}

void TestEmptyExpr() {

    bool result = true;
    RegexprParser parser("", "");

    result = (parser.GetMaxSubwordLength() == RegexprParser::ERROR);

    PrintTestResult("TestEmptyExpr", result);

}

void TestAlphabet() {

    bool result = true;
    RegexprParser parser("", "");

    parser.SetRegexpr("1");
    parser.SetWord("F");
    result = (parser.GetMaxSubwordLength() == RegexprParser::ERROR);

    PrintTestResult("TestAlphabet", result);

}

void TestError() {

    bool result = true;
    RegexprParser parser("", "");
    parser.SetWord("a");

    parser.SetRegexpr("*");
    result = (parser.GetMaxSubwordLength() == RegexprParser::ERROR);

    if (result) {
        parser.SetRegexpr("a+");
        result = (parser.GetMaxSubwordLength() == RegexprParser::ERROR);
    }

    if (result) {
        parser.SetRegexpr("a.");
        result = (parser.GetMaxSubwordLength() == RegexprParser::ERROR);
    }

    PrintTestResult("TestError", result);

}

void LaunchAllTests() {
    TestNormal();
    TestEmptyWord();
    TestEmptyExpr();
    TestAlphabet();
    TestError();
}

int main() {
    LaunchAllTests();
    return 0;
}
