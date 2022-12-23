#include <iostream>
using namespace std;

int main(){
  float A, B;
  do{
    cin>>A;
    cin>>B;
    if(A>B){
      if(A>50){
        if(A>75){
          cout<<(A);
        }
      }
    }
    else{
      A++;
      if(!(A<B)){
        cout<<(B);
      }
    }
  }
  while();

  return 0;
}