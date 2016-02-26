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

//std::move is merely function (actually function templates) that perform casts. 
//std::move unconditionally casts its argument to an rvalue.
//It does cast. It doesn’t move.

//rvalues are candidates for moving, so applying std::move to an object tells the compiler that the object is eligible to be moved from.
//That’s why std::move has the name it does: to make it easy to designate objects that may be moved from.

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


/*std::move on const object*/
class Annotation {
public:
explicit Annotation(const std::string text)
: value(std::move(text)) // "move" text into value; this code
{ … } // doesn't do what it seems to!
…
private:
std::string value;
};

/*This code compiles. This code links. This code runs. This code sets the data member
value to the content of text. The only thing separating this code from a perfect realization
of your vision is that text is not moved into value, it’s copied. Sure, text is
cast to an rvalue by std::move, but text is declared to be a const std::string, so
before the cast, text is an lvalue const std::string, and the result of the cast is an
rvalue const std::string, but throughout it all, the constness remains.*/

/*Consider the effect that has when compilers have to determine which std::string
constructor to call. There are two possibilities:*/

class string { // std::string is actually a
public: // typedef for std::basic_string<char>
…
string(const string& rhs); // copy ctor
string(string&& rhs); // move ctor
…
};
/*the result of
std::move(text) is an rvalue of type const std::string. That rvalue can’t be
passed to std::string’s move constructor, because the move constructor takes an
rvalue reference to a non-const std::string. The rvalue can, however, be passed to
the copy constructor, because an lvalue-reference-to-const is permitted to bind to a
const rvalue. The member initialization therefore invokes the copy constructor in
std::string, even though text has been cast to an rvalue!*/

/*
There are two lessons to be drawn from this example. 
1. don’t declare objects const if you want to be able to move from them. Move requests on const objects are
silently transformed into copy operations. 
2. std::move not only doesn’t actually move anything, it doesn’t even guarantee that the object it’s casting will be
eligible to be moved. The only thing you know for sure about the result of applying std::move to an object is that it’s an rvalue.
*/
