#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <unordered_set>

struct GrammarRule {
    // CF grammar rule (A -> alpha), where A is nonterminal char, alpha is string of alphabet and nonterminal symbols

    explicit GrammarRule(char from, std::string to)
        : from(from), to(std::move(to)) {}

    bool operator==(const GrammarRule& other) const {
        return (from == other.from && to == other.to);
    }
    bool operator!=(const GrammarRule& other) const {
        return !(*this == other);
    }
    static bool IsNonterminal(char symbol) {
        return symbol <= 0 || 'A' <= symbol && symbol <= 'Z';
    }

    char from;
    std::string to;

};

struct RuleHash {
    size_t operator()(const GrammarRule& rule) const {
        return std::hash<std::string>()(rule.to) + std::hash<char>()(rule.from);
    }
};

struct Grammar {
    // CF grammar

    explicit Grammar(std::vector<GrammarRule> rules, char start_nonterminal)
        : rules(std::move(rules)), start_nonterminal(start_nonterminal) {}

    std::vector<GrammarRule> rules;
    char start_nonterminal;

};

struct Configuration {
    // Configuration for Earley algorithm: grammar rule, index, symbols_read

    explicit Configuration(GrammarRule rule, int index, int symbols_read)
        : rule(std::move(rule)), index(index), symbols_read(symbols_read) {}

    bool operator==(const Configuration& other) const {
        return (rule == other.rule && index == other.index && symbols_read == other.symbols_read);
    }
    bool operator!=(const Configuration& other) const {
        return !(*this == other);
    }

    GrammarRule rule;
    int index;
    int symbols_read;

};

struct ConfigurationHash {
    size_t operator()(const Configuration& config) const {
        return RuleHash()(config.rule) + std::hash<int>()(config.index) + std::hash<int>()(config.symbols_read);
    }
};

namespace Earley {
    typedef std::vector< std::unordered_set<Configuration, ConfigurationHash> > ConfigTable;
    void Scan(ConfigTable& D, int j, const std::string& word);
    int Predict(ConfigTable& D, int j, const Grammar& grammar);
    int Complete(ConfigTable& D, int j);
}

class Algo {
    // Earley algorithm parser
public:

    explicit Algo(Grammar grammar) : grammar_(std::move(grammar)) {}

    bool IsDeducible(const std::string& word);  // check if word can be determined by grammar

private:

    const Grammar grammar_;

};

bool Algo::IsDeducible(const std::string& word) {

    const Configuration begin_config(
        GrammarRule(0, std::string(1, grammar_.start_nonterminal)),
        0, 0);  // (S' -> .S, 0) config
    const Configuration end_config(
        GrammarRule(0, std::string(1, grammar_.start_nonterminal)),
        1, 0);  // (S' -> S., 0) config

    size_t length = word.length();
    Earley::ConfigTable D(length + 1);
    D[0].emplace(begin_config);

    for (int i = 0; i <= length; ++i) {
        Earley::Scan(D, i - 1, word);
        int changes_num = -1;
        while (changes_num != 0) {
            changes_num = Earley::Predict(D, i, grammar_);
            changes_num += Earley::Complete(D, i);
        }
    }

    return (D[length].find(end_config) != D[length].end());

}

void Earley::Scan(Earley::ConfigTable& D, int j, const std::string& word) {

    if (j < 0) {
        return;
    }

    for (const auto& config : D[j]) {
        // if config index is before word current symbol
        // then move index and add new config
        if (config.index < config.rule.to.length()
            && !GrammarRule::IsNonterminal(config.rule.to[config.index])
            && config.rule.to[config.index] == word[j]) {

            Configuration new_config = config;
            ++new_config.index;
            D[j + 1].emplace(new_config);

        }
    }

}

int Earley::Predict(Earley::ConfigTable& D, int j, const Grammar& grammar) {

    std::unordered_set<Configuration, ConfigurationHash> new_configurations;

    for (const auto& config : D[j]) {
        if (config.index < config.rule.to.length()
        && GrammarRule::IsNonterminal(config.rule.to[config.index])) {
            // if config index is before nonterminal
            // then 'enter' it and add new config for all rules from it
            for (const auto& rule : grammar.rules) {
                if (rule.from == config.rule.to[config.index]
                && D[j].find(Configuration(rule, 0, j)) == D[j].end()) {
                    new_configurations.emplace(rule, 0, j);
                }
            }

        }
    }

    int old_size = D[j].size();
    D[j].merge(new_configurations);
    int new_size = D[j].size();
    return new_size - old_size;

}

int Earley::Complete(Earley::ConfigTable& D, int j) {

    std::unordered_set<Configuration, ConfigurationHash> new_configurations;

    for (int k = 0; k <= j; ++k) {
        for (const auto& config_k : D[k]) {
            for (const auto& config_j : D[j]) {

                Configuration new_config = config_k;
                ++new_config.index;

                // if config index is in the end of the rule
                // then find rule from which we 'entered' current rule and move index there
                if (D[j].find(new_config) == D[j].end()
                    && config_j != config_k && config_j.symbols_read == k
                    && config_k.index < config_k.rule.to.length() && config_j.index == config_j.rule.to.length()
                    && GrammarRule::IsNonterminal(config_k.rule.to[config_k.index])
                    && config_j.rule.from == config_k.rule.to[config_k.index]) {
                    new_configurations.emplace(new_config);
                }

            }
        }
    }

    int old_size = D[j].size();
    D[j].merge(new_configurations);
    int new_size = D[j].size();
    return new_size - old_size;

}