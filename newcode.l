%{
#include <iostream>
using namespace std;

int vowels = 0;
int identifiers = 0;
int digits = 0;
int whitespaces = 0;
int special_chars = 0;
%}

IDENT      [a-zA-Z_][a-zA-Z0-9_]*
VOWEL      [aeiouAEIOU]
DIGIT      [0-9]

%%

{VOWEL}            { vowels++; }
{DIGIT}            { digits++; }
{IDENT}            { identifiers++; }
[ \t\n]            { whitespaces++; }
.                  { special_chars++; }

%%

int main() {
    cout << "Enter a string: ";
    yylex();
    cout << "\nCounts:\n";
    cout << "Vowels: " << vowels << endl;
    cout << "Identifiers: " << identifiers << endl;
    cout << "Digits: " << digits << endl;
    cout << "Whitespaces: " << whitespaces << endl;
    cout << "Special Characters: " << special_chars << endl;
    return 0;
}

int yywrap() {
    return 1;
}
