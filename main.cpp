#include<iostream>
#include "lexer.cpp"

using namespace std;

int main(int argc,char *argv[]){
    if(argc>2){
        cout<<"Usage: main [filename]"<<endl;
        return 0;
    }
    string fn = "tests/1";
    if(argc==2){
        fn = argv[1];
    }
    cout<< "File: " << fn << endl;
    ifstream fin(fn);
    auto tokens = lexer(fin);
    unordered_map<string, int> ident_cnt;
    for (auto &item : tokens)
    {
        if (item.type == ident)
        {
            ident_cnt[item.value]++;
        }
    }
    for (auto &item : ident_cnt)
    {
        if (!wordlist_map.count(item.first))
        {
            cout << "(" << item.first << ": " << item.second << ")" << endl;
        }
    }
    cout<<endl<<endl;

    for (auto &item : tokens)
    {
        cout << item << endl;
    }
    save_token_list(tokens, fn+".token");
    // auto testdic = parse_token_file(fn+".token");
    // for (auto &item : testdic)
    // {
    //     cout << item << endl;
    // }

}