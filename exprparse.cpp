#ifndef _PL0C_EXPR
#define _PL0C_EXPR

#include <utility>

#include "tokens.cpp"
#include "error.cpp"

class exprparse{
    vector<token> tokens;
    int pos;
    int result;
public:
    exprparse(vector<token> tokens): tokens(std::move(tokens)), pos(0){}

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
        result = parse_term();

        while(peektoken().type == plussym || peektoken().type == minussym){
            if(peektoken().type == plussym){
                nexttoken();
                result += parse_term();
            }else{
                nexttoken();
                result -= parse_term();
            }
            
        }
        return is_negative?-result:result;
    }

    // term 项的产生式
    // <项> → <因子>{<乘除运算符><因子>}
    int parse_term(){
        int result = parse_factor();
        while(peektoken().type == times || peektoken().type == slash){
            if(peektoken().type == times){
                nexttoken();
                result *= parse_factor();
            }else{
                nexttoken();
                int divisor = parse_factor();
                if(divisor == 0){
                    throw std::runtime_error("0"); // Division by zero error
                    return 0;
                }
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
            nexttoken();
        }else if(peektoken().type == number){
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




};

#endif
