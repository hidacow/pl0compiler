#ifndef _PL0C_EXPR
#define _PL0C_EXPR

#include <utility>

#include "tokens.cpp"
#include "error.cpp"
#include "TAC.cpp"

class exprparse{
    vector<token> tokens;
    vector<TAC> tac;
    int pos;
    int result;
    int localvar;
    string lastident;
public:
    exprparse(vector<token> tokens): tokens(std::move(tokens)), pos(0), localvar(0){}

    token nexttoken(){
        if(pos++>=tokens.size()-1)
            return {endsym,"",0,0,0};
        //throw std::runtime_error(parserErrors[0]);
        return tokens[pos];
    }
    token peektoken(){
        if(pos==tokens.size())
            return {endsym,"",0,0,0};
        return tokens[pos];
    }
    token prevtoken(){
        return tokens[--pos];
    }
    void showtokens(){
        for(auto& token: tokens){
            cout<<token<<endl;
        }
    }
    void showraw(){
        for(auto& token: tokens){
            if(token.type == number){
                cout << token.num;
            }else if (token.type == ident){
                cout << token.value;
            }else{
                cout << wordlist_str[token.type];
            }
        }
    }


    bool is_end(){
        return pos>=tokens.size();
    }

    // expression 表达式的产生式
    // <表达式> → [+|-]<项>{<加减运算符><项>}
    int parse_expression(){
        int result = 0;
        bool is_negative = false;
        if(peektoken().type == plussym || peektoken().type == minussym){
            if(peektoken().type == minussym)
                is_negative = true;
            nexttoken();
        }
        result = is_negative? -parse_term():parse_term();
        int firstfacvar = localvar-1;
        string firstfacident = lastident;
//        lastident = "";
        if(is_negative)
        {
            tac.emplace_back("=", string(), std::to_string(0), "T" + std::to_string(localvar++));
            if(!firstfacident.empty()) tac.emplace_back("-",  "T" +std::to_string(localvar-1), firstfacident, "T" + std::to_string(localvar));
            else tac.emplace_back("-",  "T" +std::to_string(localvar-1), "T" + std::to_string(firstfacvar), "T" + std::to_string(localvar));
            localvar++; lastident = "";
        }

        while(peektoken().type == plussym || peektoken().type == minussym){
            int lastfacvar = localvar-1;
            string lastfacident = lastident;
            if(peektoken().type == plussym){
                nexttoken();
                int plusor = parse_term();
                if(!lastfacident.empty())
                    if(!lastident.empty()) tac.emplace_back("+", lastfacident, lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("+", lastfacident, "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                else
                    if(!lastident.empty()) tac.emplace_back("+",  "T" +std::to_string(lastfacvar), lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("+",  "T" +std::to_string(lastfacvar), "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                result += plusor;
            }else{
                nexttoken();
                int minor = parse_term();
                if(!lastfacident.empty())
                    if(!lastident.empty()) tac.emplace_back("-", lastfacident, lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("-", lastfacident, "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                else
                if(!lastident.empty()) tac.emplace_back("-",  "T" +std::to_string(lastfacvar), lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("-",  "T" +std::to_string(lastfacvar), "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                result -= minor;
            }
            localvar++; lastident = "";
        }
        return result;
    }

    // term 项的产生式
    // <项> → <因子>{<乘除运算符><因子>}
    int parse_term(){
        int result = parse_factor();
        int lastfacvar = localvar-1;
        string lastfacident = lastident;
        while(peektoken().type == times || peektoken().type == slash){
            if(peektoken().type == times){
                nexttoken();
                int timer = parse_factor();
                if(!lastfacident.empty())
                    if(!lastident.empty()) tac.emplace_back("*", lastfacident, lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("*", lastfacident, "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                else
                    if(!lastident.empty()) tac.emplace_back("*", "T" + std::to_string(lastfacvar), lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("*", "T" + std::to_string(lastfacvar), "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                localvar++; lastident = "";
                result *= timer;
            }else{
                nexttoken();
                int divisor = parse_factor();
                if(divisor == 0){
                    throw std::runtime_error("Division by zero"); // Division by zero error
                    //return 0;
                }
                if(!lastfacident.empty())
                    if(!lastident.empty()) tac.emplace_back("/", lastfacident, lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("/", lastfacident, "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                else
                    if(!lastident.empty())tac.emplace_back("/", "T" + std::to_string(lastfacvar), lastident, "T" + std::to_string(localvar));
                    else tac.emplace_back("/", "T" + std::to_string(lastfacvar), "T" + std::to_string(localvar-1), "T" + std::to_string(localvar));
                localvar++; lastident = "";
                result /= divisor;
            }
        }

        return result;
    }

    // factor 因子的产生式
    // <因子> → <标识符>|<无符号整数>|'('<表达式>')'
    int parse_factor(){
        int result = 0;
        auto p = peektoken();
        if(peektoken().type == ident){
            lastident = peektoken().value;
            nexttoken();
        }else if(peektoken().type == number){
            tac.emplace_back("=", string(), std::to_string(peektoken().num), "T" + std::to_string(localvar++));
            lastident = "";
            result = peektoken().num;
            nexttoken();
        }else if(peektoken().type == lparen){
            nexttoken();
            result = parse_expression();
            if(peektoken().type == rparen){
                nexttoken();
            }else{
                throw std::runtime_error(parserErrors[22]);
            }
        }else{
            throw std::runtime_error(parserErrors[23]);
        }
        return result;
    }

    void end(){
        if(pos<tokens.size())
            throw std::runtime_error(parserErrors[8]+" expression already ended before token "+std::to_string(pos) + " [ " + tokens[pos].show() + " ]");
    }

    void showtac()
    {
        for(auto &it: tac)
            std::cout << "(" << it.op << ", "
                      << it.y << ", " << it.z << ", " << it.x << ")" << std::endl;
    }


};

#endif