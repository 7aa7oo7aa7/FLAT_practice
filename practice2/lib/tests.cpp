#include "Algo.cpp"

void TestScan() {

    GrammarRule BS_rule_1('S', "S(S)");
    GrammarRule BS_rule_2('S', "(S)S");
    Earley::ConfigTable D(2);
    D[0].emplace(BS_rule_1, 0, 0);  // (S -> .S(S), 0)
    D[0].emplace(BS_rule_2, 0, 0);  // (S -> .(S)S, 0)
    Earley::Scan(D, 0, "(((((");  // adds (S -> (.S)S, 0)
    if (D[1].size() == 1 && *(D[1].begin()) == Configuration(BS_rule_2, 1, 0)) {
        std::cout << "Scan test passed.\n";
    } else {
        std::cout << "Scan test failed.\n";
    }

}

void TestPredict() {

    std::vector<GrammarRule> BS_rules = {
        GrammarRule('S', "T(T)"),
        GrammarRule('S', "(T)T"),
        GrammarRule('T', "(T)")
    };
    Grammar BS(BS_rules, 'S');
    Earley::ConfigTable D(1);
    D[0].emplace(BS_rules[0], 0, 0);  // (S -> .T(T), 0)
    D[0].emplace(BS_rules[1], 0, 0);  // (S -> .(T)T, 0)
    int changes_num = Earley::Predict(D, 0, BS);  // adds (T -> .(T), 0), returns 1
    if (changes_num == 1 && D[0].size() == 3
    && D[0].find(Configuration(BS_rules[2], 0, 0)) != D[0].end()) {
        std::cout << "Predict test passed.\n";
    } else {
        std::cout << "Predict test failed.\n";
    }

}

void TestComplete() {

    std::vector<GrammarRule> BS_rules = {
        GrammarRule('S', "(T)T"),
        GrammarRule('T', "(T)")
    };
    Earley::ConfigTable D(2);
    D[0].emplace(BS_rules[0], 1, 1);  // (S -> (.T)T, 1)
    D[1].emplace(BS_rules[1], 3, 2);  // (T -> (T)., 2)
    int changes_num = Earley::Complete(D, 1);  // adds (S -> (T.)T, 2), returns 1
    if (changes_num == 1 && D[0].size() == 1 && D[1].size() == 2
    && D[1].find(Configuration(BS_rules[0], 2, 2)) != D[1].end()) {
        std::cout << "Complete test passed.\n";
    } else {
        std::cout << "Complete test failed.\n";
    }

}

void TestAlgo() {

    bool flag = true;
    bool global_flag = true;

    GrammarRule CBS_rule_1('S', "(S)S");
    GrammarRule CBS_rule_2('S', "");
    Grammar CBS({CBS_rule_1, CBS_rule_2}, 'S');
    Algo CBS_parser(CBS);
    flag = CBS_parser.IsDeducible("((((()()()()()()))(()))((()()())))");
    if (!flag) {
        std::cout << "CBS parser failed in deducing CBS.\n";
        global_flag = false;
    }
    flag = !CBS_parser.IsDeducible("))))))))))))())))))))))))))))))))))))");
    if (!flag) {
        std::cout << "CBS parser failed in deducing non-CBS.\n";
        global_flag = false;
    }

    // grammar from 2nd control work
    // { w: 2 * |w|(a) - |w|(b) = -2 }
    std::vector<GrammarRule> G_rules = {
        GrammarRule('S', "TbTbT"),
        GrammarRule('T', "aTbTbT"),
        GrammarRule('T', "bTbTaT"),
        GrammarRule('T', "bTaTbT"),
        GrammarRule('T', "")
    };
    Grammar G(G_rules, 'S');
    Algo G_parser(G);
    flag = G_parser.IsDeducible("abbbbabbbabababbbbab");
    if (!flag) {
        std::cout << "G parser failed in deducing G.\n";
        global_flag = false;
    }
    flag = !G_parser.IsDeducible("ababababababab");
    if (!flag) {
        std::cout << "G parser failed in deducing non-G.\n";
        global_flag = false;
    }

    if (global_flag) {
        std::cout << "Algo test passed.\n";
    } else {
        std::cout << "Algo test failed.\n";
    }

}

int main() {
    TestScan();
    TestPredict();
    TestComplete();
    TestAlgo();
    return 0;
}
