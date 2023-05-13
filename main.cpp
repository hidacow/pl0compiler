#include<iostream>
#include "lexer.cpp"
#include "exprparse.cpp"
using namespace std;
int main(int argc,char *argv[]){
    if(argc>2){
        cout<<"Usage: main [filename]"<<endl;
        return 0;
    }
    string fn = "tests/5";
    if(argc==2){
        fn = argv[1];
    }
    cout<< "File: " << fn << endl;
    ifstream fin(fn);
    if (!fin)
    {
        cerr << "Error opening file." << endl;
        return 0;
    }
    auto tokens = lexer(fin,fn);
//    /*
//    LAB 1
//    */
//    unordered_map<string, int> ident_cnt;
//    for (auto &item : tokens)
//        if (item.type == ident)
//            ident_cnt[item.value]++;
//    for (auto &item : ident_cnt)
//        if (!wordlist_map.count(item.first))
//            cout << "(" << item.first << ": " << item.second << ")" << endl;
//    cout<<endl<<endl;
//    /*
//    LAB 2
//    */
//    for (auto &item : tokens)
//        cout << item << endl;
//    save_token_list(tokens, fn+".token");
    //auto tokens = parse_token_file(fn);
/*
LAB 3
*/
    auto expression = exprparse(tokens);
//    expression.showraw();
//    cout<<endl<<endl;
//    try{
//        cout<<"Result: "<<expression.parse_expression()<<endl;
//        expression.end();
//    }catch (const std::exception& e){
//        cout << "Error: " << e.what() << endl;
//    }
/*
LAB 4
*/
    int result;
    try{
        result = expression.parse_expression();
        expression.showtac();
        expression.end();
        cout<<"Passed"<<endl;
    }catch (const std::exception& e){
        cout << "Error: " << e.what() << endl;
    }
    //cout << "Result: " << result << endl;
    return 0;
}