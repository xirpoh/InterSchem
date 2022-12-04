#include <iostream>
#include "expression_eval.cpp"
using namespace std;

int main() {
    char test_expr[100] = "6*7==42 && 4*6==24";
    //Example: "cbrt(tan(sqrt(log10(10^2+(5+4)*100)^2)) + (sin(7)^2 + cos(7)^2)/2)-0.5"; 
    //cbrt(tan(sqrt(log10(10**2+(5+4)*100)**2)) + (sin(7)**2 + cos(7)**2)/2)-0.5
    
    int i = 4;
    while (i--) {
        //gets(test_expr);
        cout << "=" << expression_eval(test_expr);
        cout << "\n";
    }
    

    return 0;
}
 
