#ifndef _PL0C_EXPR
#define _PL0C_EXPR

#include "tokens.cpp"
#include "error.cpp"

class exprparse{
    vector<token> tokens;
    int pos;
public:
    exprparse(vector<token> tokens): tokens(tokens), pos(0){}

    token nexttoken(){
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

    bool is_end(){
        return pos>=tokens.size();
    }

    // expression 表达式的产生式
    // <表达式> → [+|-]<项>{<加减运算符><项>}
    void parse_expression(){
        if(peektoken().type == plussym || peektoken().type == minussym){
            cout << "next term: " << nexttoken() << endl;
            parse_term();
        }else{
            cout << "next term: " << nexttoken() << endl;
            parse_term();
        }
        while(peektoken().type == plussym || peektoken().type == minussym){
            cout << "next term: " << nexttoken() << endl;
            parse_term();
        }
    }

    // term 项的产生式
    // <项> → <因子>{<乘除运算符><因子>}
    void parse_term(){
        cout << "next factor: " << peektoken() << endl;
        parse_factor();
        while(peektoken().type == times || peektoken().type == slash){
            cout << "next factor: " << nexttoken() << endl;
            parse_factor();
        }
    }

    // factor 因子的产生式
    // <因子> → <标识符>|<无符号整数>|'('<表达式>')'
    void parse_factor(){
        if(peektoken().type == ident){
            cout << "next ident: " << nexttoken() << endl;
        }else if(peektoken().type == number){
            cout << "next number: " << nexttoken() << endl;
        }else if(peektoken().type == lparen){
            parse_expression();
            if(peektoken().type == rparen){
                cout << "next rparen: " << nexttoken() << endl;
            }else{
                throw std::runtime_error("22");
            }
        }

    }

};

#endif
