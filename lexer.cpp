#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <cmath>
#include <cassert>
#include "util.cpp"
#include "tokens.cpp"
using std::string;
using std::ifstream;
using std::ofstream;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;

enum state      //状态机状态枚举类
{
    start,      //初始状态
    inident,    //标识符
    innum,      //数字
    incomment,  //注释(不作要求)
    inassign,   //赋值(:)
    assign,     //赋值(:=)
    inlt,       //less than (<)
    ingt,       //greater than (>)
    inle,       //less than or equal (<=)
    inge,       //greater than or equal (>=)
    ineq,       //equal (=)
    inneq       //not equal (#)
};
constexpr size_t MAX_TOKEN_SIZE = 25;
constexpr size_t MAX_NUM_SIZE = 10;

vector<token> lexer(ifstream &fin)
{
    assert(fin);
    int current_state = start;
    long long current_number = 0;
    string current_ident;
    int errors = 0, warnings = 0;
    vector<token> token_list;
    int pos = 0;
    bool is_eof = false;
    while (!is_eof)
    {
        char ch;
        fin.get(ch);
        pos++;
        if (fin.eof())
        {
            is_eof = true;
            ch = ' ';
        }
        switch (current_state)
        {
        case start:
            if (is_blank(ch))
            {
                current_state = start;
            }
            else if (isdigit(ch))
            {
                current_state = innum;
                current_number = ch - '0';
            }
            else if (isalpha(ch))
            {
                current_state = inident;
                current_ident = ch;
            }
            else if (ch == '{')
            {
                current_state = incomment;
            }
            else if (ch == ':')
            {
                current_state = inassign;
            }
            else if (ch == '<')
            {
                current_state = inlt;
            }
            else if (ch == '>')
            {
                current_state = ingt;
            }
            else if (ch == '=')
            {
                current_state = ineq;
            }
            else if (ch == '#')
            {
                current_state = inneq;
            }
            else if (symbol_set.count(ch))
            {
                token_list.emplace_back(wordlist_map[string(1,ch)],string(1,ch),0,pos);
                current_state = start;
            }
            else
            {
                cerr << "Invalid character: " << ch << "(" << (int)ch << ") on column " << pos << endl;
                errors++;
            }
            break;
        case inident:
            if (isalnum(ch))
            {
                current_ident += ch;
                if (current_ident.size() > MAX_TOKEN_SIZE)
                {
                    cerr << "Identifier too long:" << current_ident << endl;
                    errors++;
                    current_ident = "";
                    current_state = start;
                }
            }
            else
            {
                if(wordlist_map.count(to_lower(current_ident)))
                    token_list.emplace_back(wordlist_map[to_lower(current_ident)],"",0,pos);
                else
                    token_list.emplace_back(ident,to_lower(current_ident),0,pos);
                current_ident = "";
                current_state = start;
                fin.putback(ch);
                pos--;
            }
            break;
        case innum:
            if (isdigit(ch))
            {
                if(current_number > pow(10, MAX_NUM_SIZE-1))
                {
//                    throw std::runtime_error("Number too large");
                    std::cerr << "Number too large: " << current_number << "... on column " << pos << endl;
                    errors++;
                    current_number = 0;
                    current_state = start;
                }
                current_number = current_number * 10 + (ch - '0');
            }
            else
            {
                token_list.emplace_back(number,"",current_number,pos);
//                if(isalpha(ch)) {
//                    cerr << "Invalid ident starting with " << current_number << " on column " << current_line << endl;
//                    errors++;
//                }
                current_number = 0;
                current_state = start;
                fin.putback(ch);
                pos--;
            }
            break;
        case incomment:
            if (ch == '}')
            {
                current_state = start;
            }
            break;
        case inassign:      // :
            if (ch == '=')  // :=
            {
                current_state = assign;
            }
            else
            {
                cerr << "Invalid character '" << ch << "' after ':' on column " << pos << endl;
                errors++;
                current_state = start;
            }
            break;
        case inlt:          // <
            if (ch == '=')  // <=
            {
                current_state = inle;
            }
            else
            {
                current_state = start;
                token_list.emplace_back(lss,wordlist_str[lss],0,pos);
                fin.putback(ch);
            }
            break;
        case ingt:          // >
            if (ch == '=')  // >=
            {
                current_state = inge;
            }
            else
            {
                current_state = start;
                token_list.emplace_back(gtr,wordlist_str[gtr],0,pos);
                fin.putback(ch);
            }
            break;
        case inle:
            current_state = start;
            token_list.emplace_back(leq,wordlist_str[leq],0,pos);
            fin.putback(ch);
            break;
        case inge:
            current_state = start;
            token_list.emplace_back(geq,wordlist_str[geq],0,pos);
            fin.putback(ch);
            break;
        case ineq:
            current_state = start;
            token_list.emplace_back(eql,wordlist_str[eql],0,pos);
            fin.putback(ch);
            break;
        case inneq:
            current_state = start;
            token_list.emplace_back(neq,wordlist_str[neq],0,pos);
            fin.putback(ch);
            break;
        case assign:
            current_state = start;
            token_list.emplace_back(becomes,wordlist_str[becomes],0,pos);
            fin.putback(ch);
            break;
        default:
            break;
        }
    }
    token lasttoken = token_list.front();
    bool isFirst = true;
    // check for errors in symbols and identifiers
    for(auto &t : token_list){
        if(isFirst){
            isFirst = false;
            continue;
        }
        if(t.type == ident && lasttoken.type == number){
            cerr << "Invalid ident: " << lasttoken.num << t.value << " on column " << lasttoken.pos << endl;
            errors++;
        }
        if(t.type == ident && lasttoken.type == ident){
            cerr << "Invalid ident: " << lasttoken.value << t.value << " on column " << lasttoken.pos << endl;
            errors++;
        }
        if(t.type == number && lasttoken.type == ident){
            cerr << "Invalid ident: " << lasttoken.value << t.num << " on column " << lasttoken.pos << endl;
            errors++;
        }
        if(t.type == number && lasttoken.type == number){
            cerr << "Invalid ident: " << lasttoken.num << t.num << " on column " << lasttoken.pos << endl;
            errors++;
        }
        if(isOperator(t) && isOperator(lasttoken)){
            cerr << "Invalid operator: " << lasttoken.value << t.value << " on column " << lasttoken.pos << endl;
            errors++;
        }
        lasttoken = t;
    }



    if(errors > 0 || warnings > 0) {
        cerr << endl << "Errors: " << errors << ", Warnings: " << warnings << endl;
        //exit(1);
    }else{
        cout << endl << "Lexical analysis completed successfully" << endl;
    }
    return token_list;
    
}

void save_token_list(vector<token> &token_list, const string& filename)
{
    ofstream fout(filename);
    for (auto &t : token_list)
    {
        fout << t << endl;
    }
    fout.close();
}

vector<token> parse_token_file(const string& filename){
    ifstream fin(filename);
    vector<token> token_list;
    string name,value;
    while(fin >> name >> value){
        name.pop_back();
        value.pop_back();
        name = name.substr(1);
        if(name == "number"){
            token_list.emplace_back(number,value,stoi(value),0);
        }
        else if(name == "ident"){
            token_list.emplace_back(ident,value,0,0);
        }else if(wordlist_map.count(value)){
            token_list.emplace_back(wordlist_map[value],"",0,0);
        }
        else{
            cout << "Invalid token name: " << name << endl;
            throw std::runtime_error("Invalid token name");
        }
        
    }
    fin.close();
    return token_list;
}
