/*-------------------std::move---------------------------------------------*/
/*std::move is used to indicate that an object t may be "moved from", 
i.e. allowing the efficient transfer of resources from t to another object.
In particular, std::move produces an xvalue expression that identifies its argument t. 
It is exactly equivalent to a static_cast to an rvalue reference type.*/

/*std::move internal implmentation*/
/*
Defined in header <utility>
template< class T >
typename std::remove_reference<T>::type&& move( T&& t ); (since C++11) 

template< class T >
constexpr typename std::remove_reference<T>::type&& move( T&& t ); (until C++14)
*/

// Example program
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;
class A
{
    public: 
    A(){} 
    /*Names of rvalue reference variables are lvalues and have to be converted to xvalues 
    to be bound to the function overloads that accept rvalue reference parameters, 
    which is why move constructors and move assignment operators typically use std::move*/
    // Simple move constructor
    A(A&& arg) : member(std::move(arg.member)) // the expression "arg.member" is lvalue
    { cout<<"A Move constructor"<<endl; } 
    // Simple move assignment operator
    A& operator=(A&& other) 
    {
        cout<<"A Move Assignment"<<endl;
        member = std::move(other.member);
        return *this;
    }
    string member;
};

int main()
{
  A o1;
  A o2(std::move(o1));
  A o3;
  A o4;
 o4 = std::move(o3);
  
  std::string str = "Hello";
  std::vector<std::string> v;
 
  // uses the push_back(const T&) overload, which means 
  // we'll incur the cost of copying str
  v.push_back(str);
  std::cout << "After copy, str is \"" << str << "\"\n";
 
 // uses the rvalue reference push_back(T&&) overload, 
 // which means no strings will be copied; instead, the contents
 // of str will be moved into the vector.  This is less
 // expensive, but also means str might now be empty.
 v.push_back(std::move(str));
 std::cout << "After move, str is \"" << str << "\"\n";
 std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"\n";
}

