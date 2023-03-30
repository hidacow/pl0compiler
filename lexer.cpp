#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <cmath>
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

enum state
{
    start,
    inident,
    innum,
    incomment,
    inassign,
    assign,
    inlt,
    ingt,
    inle,
    inge,
    ineq,
    inneq
};
constexpr size_t MAX_TOKEN_SIZE = 25;
constexpr size_t MAX_NUM_SIZE = 10;

vector<token> lexer(ifstream &fin)
{

    int current_state = start;
    long long current_number = 0;
    string current_ident = "";

    vector<token> token_list;
    int current_line = 1;
    bool is_eof = false;
    while (!is_eof)
    {
        char ch;
        fin.get(ch);
        current_line++;
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
                token_list.emplace_back(wordlist_map[string(1,ch)],"",0,current_line);
                current_state = start;
            }
            else
            {
                cout << "Invalid character: " << ch << "(" << (int)ch << ")" << endl;
                throw std::runtime_error("Invalid character");
            }
            break;
        case inident:
            if (isalnum(ch))
            {
                current_ident += ch;
                if (current_ident.size() > MAX_TOKEN_SIZE)
                {
                    throw std::runtime_error("Identifier too long");
                }
            }
            else
            {
                if(wordlist_map.count(to_lower(current_ident)))
                {
                    token_list.emplace_back(wordlist_map[to_lower(current_ident)],"",0,current_line);
                }
                else
                {
                    token_list.emplace_back(ident,to_lower(current_ident),0,current_line);
                }
                current_ident = "";
                current_state = start;
                fin.putback(ch);
                current_line--;
            }
            break;
        case innum:
            if (isdigit(ch))
            {
                if(current_number > pow(10, MAX_NUM_SIZE-1))
                {
                    throw std::runtime_error("Number too large");
                }
                current_number = current_number * 10 + (ch - '0');
            }
            else
            {
                token_list.emplace_back(number,"",current_number,current_line);
                current_number = 0;
                current_state = start;
                fin.putback(ch);
                current_line--;
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
                token_list.emplace_back(lss,"",0,current_line);
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
                token_list.emplace_back(gtr,"",0,current_line);
                fin.putback(ch);
            }
            break;
        case inle:
            current_state = start;
            token_list.emplace_back(leq,"",0,current_line);
            fin.putback(ch);
            break;
        case inge:
            current_state = start;
            token_list.emplace_back(geq,"",0,current_line);
            fin.putback(ch);
            break;
        case ineq:
            current_state = start;
            token_list.emplace_back(eql,"",0,current_line);
            fin.putback(ch);
            break;
        case inneq:
            current_state = start;
            token_list.emplace_back(neq,"",0,current_line);
            fin.putback(ch);
            break;
        case assign:
            current_state = start;
            token_list.emplace_back(becomes,"",0,current_line);
            fin.putback(ch);
            break;
        default:
            break;
        }
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
