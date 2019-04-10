#include<iostream>
#include<cstdio>
using namespace std;

int func(int x)
{
    if(x == 0)
        return 1;
    else
        return func(x-1) * x;
}

int main()
{
    int x;
    cin >> x;
    cout << func(x) << endl;   
    return 0; 
}