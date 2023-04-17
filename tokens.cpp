#ifndef _PL0C_TOKEN
#define _PL0C_TOKEN

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


// 所有出现的符号字符，如果出现了不在这个集合中的符号，就是非法符号
unordered_set<char> symbol_set = {'+', '-', '*', '/', '=', '#', '<', '>', ':', '(', ')', ',', ';', '.'};

struct token{
    wordlist type;
    string value;
    long long num;
    int line;
    int pos;
    token(wordlist type, string value, long long num, int line,int pos):type(type), value(std::move(value)), num(num), line(line), pos(pos){}
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
    string show() const{
        if(this->type == ident)
            return this->value;
        else if(this->type == number)
            return std::to_string(this->num);
        else
            return wordlist_str[this->type];
    }

    string showpos() const{
        return std::to_string(line)+":"+std::to_string(pos);
    }
};

bool isOperator(const token& t){
    return t.type>=13 && t.type<=23;
}

#endif