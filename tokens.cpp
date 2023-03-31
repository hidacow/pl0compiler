#include<unordered_set>
#include<unordered_map>
#include <utility>
#include<vector>
#include<string>
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::vector;
using std::ostream;

//PL-0 wordlist
enum wordlist{
    beginsym, callsym, constsym, dosym, endsym, ifsym,
    oddsym, procsym, readsym, thensym, varsym, whilesym,
    writesym, plussym, minussym, times, slash,
    eql, neq, lss, leq, gtr, geq, becomes, lparen, rparen,
    comma, semicolon, period, ident, number
};
vector<string> wordlist_sym{
    "beginsym", "callsym", "constsym", "dosym", "endsym", "ifsym",
    "oddsym", "procsym", "readsym", "thensym", "varsym", "whilesym",
    "writesym", "plussym", "minussym", "times", "slash",
    "eql", "neq", "lss", "leq", "gtr", "geq", "becomes", "lparen", "rparen",
    "comma", "semicolon", "period", "ident", "number"
};
vector<string> wordlist_str = {"begin", "call", "const", "do", "end", "if",
                               "odd", "procedure", "read", "then", "var", "while",
                               "write", "+", "-", "*", "/", "=", "#", "<", "<=", 
                               ">", ">=", ":=", "(", ")", ",", ";", "."};

unordered_map<string,wordlist> wordlist_map = {{"begin", beginsym}, {"call", callsym}, {"const", constsym}, {"do", dosym}, {"end", endsym}, {"if", ifsym},
                                          {"odd", oddsym}, {"procedure", procsym}, {"read", readsym}, {"then", thensym}, {"var", varsym}, {"while", whilesym},
                                          {"write", writesym}, {"+", plussym}, {"-", minussym}, {"*", times}, {"/", slash},
                                          {"=", eql}, {"#", neq}, {"<", lss}, {"<=", leq}, {">", gtr}, {">=", geq}, {":=", becomes}, {"(", lparen}, {")", rparen},
                                          {",", comma}, {";", semicolon}, {".", period}};



unordered_set<char> symbol_set = {'+', '-', '*', '/', '=', '#', '<', '>', ':', '(', ')', ',', ';', '.'};

struct token{
    wordlist type;
    string value;
    long long num;
    int line;
    token(wordlist type, string value, long long num, int line):type(type), value(std::move(value)), num(num), line(line){}
    friend ostream& operator<<(std::ostream& os, const token& t){
        os<<"(";
        if(t.type == ident)
            os << wordlist_sym[t.type] << ", " << t.value;
        else if(t.type == number)
            os << wordlist_sym[t.type] << ", " << t.num;
        else
            os << wordlist_sym[t.type] << ", " << wordlist_str[t.type];
        os << ")";
        return os;
    }
};

bool isOperator(const token& t){
    return t.type>=13 && t.type<=23;
}

