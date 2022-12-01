#include <iostream>
#include "expression_eval.cpp"
using namespace std;

int main() {
    char test_expr[100];
    //Example: "cbrt(tan(sqrt(log10(10^2+(5+4)*100)^2)) + (sin(7)^2 + cos(7)^2)/2)-0.5"; 
    //cbrt(tan(sqrt(log10(10**2+(5+4)*100)**2)) + (sin(7)**2 + cos(7)**2)/2)-0.5
    gets(test_expr);

    cout << "=" << expression_eval(test_expr);
    

    return 0;
}
 
