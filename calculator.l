%{
#include <iostream>
#include <cstdlib>
using namespace std;

// Define YYSTYPE if you're integrating with Bison/Yacc
int result = 0;
%}

DIGIT       [0-9]
NUMBER      {DIGIT}+

%%

{NUMBER}    {
                result = atoi(yytext);
                cout << "Number: " << result << endl;
            }

"+"         {
                cout << "Operator: +" << endl;
            }

"-"         {
                cout << "Operator: -" << endl;
            }

"*"         {
                cout << "Operator: *" << endl;
            }

"/"         {
                cout << "Operator: /" << endl;
            }

\n          {
                cout << "End of expression.\n";
            }

[ \t]       ;  // Ignore whitespaces

.           {
                cout << "Unknown character: " << yytext << endl;
            }

%%

int main(int argc, char** argv) {
    cout << "Enter an expression (e.g. 2 + 3 * 4):\n";
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
