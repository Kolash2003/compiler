%{
#include <iostream>
using namespace std;
%}

%option noyywrap
%option c++

ID     [a-zA-Z_][a-zA-Z0-9_]*

%%

{ID}    { cout << "Valid string: " << yytext << endl; }
.|\n    { cout << "Invalid string: " << yytext << endl; }

%%

int main() {
    cout << "Enter strings to validate (Ctrl+D to end):\n";
    yylex();
    return 0;
}
