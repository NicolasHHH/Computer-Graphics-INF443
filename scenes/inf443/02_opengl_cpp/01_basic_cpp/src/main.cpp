#include <iostream>
using namespace std;

void addOne(int x){
    x = x + 1;
    cout<<"basic func"<<endl;
}

void addOne(int* x){
    *x = *x + 1;
    cout<<"pointer func"<<endl;
}

void addOneRef(int& x){
    x = x + 1;
    cout<<"reference func"<<endl;
}


int main()
{
    std::cout << "Hello world" << std::endl;

    // pointeurs
    int a = 5;
    int* p ;
    p = &a;
    cout << *p; // 5
    *p = 12;
    cout << a; // 12

    int b = 8;
    *p = b;
    b = 6;
    cout << *p <<endl; // 8

    int c = 5;
    addOne(c);
    cout << " " << c ;
    addOne(&c);
    cout << " " << c ;
    cout <<endl;

    // references
    a = 5;
    int& ref_a = a;
    ref_a = 8;
    cout << "reference : "<< a <<endl;
    addOneRef(ref_a);
    cout << a ;


    return 0;
}
