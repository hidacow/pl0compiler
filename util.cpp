#include<string>
using std::string;

string to_lower(const string& str){
    string res(str);
    for(auto& ch: res){
        ch = tolower(ch);
    }
    return res;
}

bool is_blank(char ch){
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || isblank(ch);
}