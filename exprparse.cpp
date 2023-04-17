#ifndef _PL0C_EXPR
#define _PL0C_EXPR

#include <utility>

#include "tokens.cpp"
#include "error.cpp"

class exprparse{
    vector<token> tokens;
    int pos;
public:
    exprparse(vector<token> tokens): tokens(std::move(tokens)), pos(0){}

    token nexttoken(){
        if(pos>=tokens.size()-1)
            throw std::runtime_error(parserErrors[0]);
        return tokens[++pos];
    }
    token peektoken(){
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
                cout << wordlist_str[token.type] << " ";
            }
        }
    }


    bool is_end(){
        return pos>=tokens.size();
    }

    // expression 表达式的产生式
    // <表达式> → [+|-]<项>{<加减运算符><项>}
    void parse_expression(){
        if(peektoken().type == plussym || peektoken().type == minussym){
            nexttoken();
            parse_term();
        }else{
            nexttoken();
            parse_term();
        }
        while(peektoken().type == plussym || peektoken().type == minussym){
            nexttoken();
            parse_term();
        }
    }

    // term 项的产生式
    // <项> → <因子>{<乘除运算符><因子>}
    void parse_term(){
        peektoken();
        parse_factor();
        while(peektoken().type == times || peektoken().type == slash){
            nexttoken();
            parse_factor();
        }
    }

    // factor 因子的产生式
    // <因子> → <标识符>|<无符号整数>|'('<表达式>')'
    void parse_factor(){
        if(peektoken().type == ident){
            nexttoken();
        }else if(peektoken().type == number){
            nexttoken();
        }else if(peektoken().type == lparen){
            parse_expression();
            if(peektoken().type == rparen){
                cout << "next rparen: " << nexttoken() << endl;
            }else{
                throw std::runtime_error(parserErrors[22]);
            }
        }

    }



};

#endif
