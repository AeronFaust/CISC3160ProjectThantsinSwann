/*
Swann Thantsin
Professor Neng Fa Zhou
CISC 3160
Due: 23 May 2022
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

//Global Variables
string type[]= {"ERR", "Ide", "Lit", "Mod" , "PaO" , "PaC", "EOF"};

//Header
class Token
{
    private:
        int typeindex;
        string data;
    public:
        Token(int t, string s);
        void settoken(int t, string s);
        void printtoken();
        void printspectoken(int i);
        int gettype();
        string getdata();
};

vector<Token> lexer(string str);

bool isnumber(string s);

template <class T>
struct node
{
    T data;
    node *next;
};
template <class T>
class Stack
{
    private:
        node<T> *top;
    public:
        Stack();
        bool isEmpty();
        void push(T item);
        void pop();
        void getTop(T &item);
        void get(T &item);
};

class Identifier
{
    private:
        string name;
        int data;
    public:
        Identifier(string n, int d);
        int getnum(string n, int &d);
        void replace(int d);
        void printide();
};

bool lowerPrecedence(string op, Stack<string> &opstack);
void calculate(Stack<int> &num,Stack<string> &op);
int parser(vector<Identifier>&ide,vector<Token> tok);

//main
int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " input.txt" << endl;
        return -1;
    }

    string filename(argv[1]);

    string buffer;
    vector<Identifier> idelist;

    ifstream inputfile(filename);

    while(getline(inputfile, buffer))
    {
        vector <Token> input = lexer(buffer);
        if (parser(idelist,input) == -1)
        {
            cout << "Parsing Error" << endl;
            return -1;
        }
    }

    for (int i = 0; i < idelist.size();i++)
    {
        idelist[i].printide();
    }
    
    inputfile.close();
    
    return 0;
}

//Implementation
//Token Implementation
Token::Token(int t, string s)
{
    typeindex = t;
    data = s;  
}

void Token::settoken(int t,string s)
{
    typeindex = t;
    data = s;  
}

void Token::printtoken()
{
    cout << type[typeindex] <<": " << data << endl;  
}

void Token::printspectoken(int i)
{
    if (typeindex == i)
        cout << data << " ";
}

int Token::gettype()
{
    return typeindex;
}

string Token::getdata()
{
    return data;
}

//tokenizer/lexical analizer
vector<Token> lexer(string str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    
    vector <Token> tok;
    vector <int> tokIn;
    vector <int> tokId;
    Token err(0, "-1");
    
    int curIn = 0;
    
    //Sets up identifier
    tokIn.push_back(curIn);
    if(islower(str[curIn]))
    {
        while(str[curIn] != '=')
            curIn++;
    }
    else
    {
        cout << "Token Error" << endl;
        tok.push_back(err);
        return tok;
    }
    tokId.push_back(1);
    tokIn.push_back(curIn);
    
    curIn++;

    //To assign a value to the identifier
    while(curIn < str.length())
    {
        if(isdigit(str[curIn]))
        {
            if (str[curIn] == '1' || str[curIn] == '2' || str[curIn] == '3' || 
                str[curIn] == '4' || str[curIn] == '5' || str[curIn] == '6' || 
                str[curIn] == '7' || str[curIn] == '8' || str[curIn] == '9')
            {
                tokIn.push_back(curIn);
                curIn++;
                while (isdigit(str[curIn]))
                    curIn++;
                tokId.push_back(2);
                tokIn.push_back(curIn);
            }
            else  if(str[curIn] == '0' && !isdigit(str[curIn+1]))
            {
                tokIn.push_back(curIn);
                curIn++;
                tokId.push_back(2);
                tokIn.push_back(curIn);
            }
            else
            {
                cout << "Numeric Token Error" << endl;
                tok.push_back(err);
                return tok;
            }
        }
        else if(islower(str[curIn]))
        {
            tokIn.push_back(curIn);
            curIn++;
            while(str[curIn] != '+' && str[curIn] != '-' && str[curIn] != '*'&& str[curIn] != ')'&& str[curIn] != ';')
                curIn++;
            tokId.push_back(2);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == '+')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(3);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == '-')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(3);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == '*')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(3);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == '(')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(4);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == ')')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(5);
            tokIn.push_back(curIn);
        }
        else if(str[curIn] == ';')
        {
            tokIn.push_back(curIn);
            curIn++;
            tokId.push_back(6);
            tokIn.push_back(curIn);
        }
        else
        {
            cout << "Token Error" << endl;
            tok.push_back(err);
            return tok;
        }
    }
    for (int i = 0; i < tokId.size(); i++)
    {
        Token elem(tokId[i], str.substr(tokIn[2*i],tokIn[2*i+1]-tokIn[2*i]));
        tok.push_back(elem);
    }
    
    return tok;
}

bool isnumber(string s)
{
    for (char c : s) 
    {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

//Stack Implementation
template<class T>
Stack<T> :: Stack()
{
    top = NULL;
}

template<class T>
bool Stack<T> :: isEmpty()
{
    return top == NULL;
}

template<class T>
void Stack<T> :: push (T item)
{
    node<T> *temp;
    temp = new node<T>;

    temp->data = item;
    temp->next = top;

    top = temp;
}

template<class T>
void Stack<T> :: pop ()
{   
    if (top != NULL)
    {
        node<T> *temp;

        temp = top;
        top = top->next;

        delete temp;
    }
}

template <class T>
void Stack<T> :: getTop (T &item)
{
    if(top != NULL) item = top->data;
    else cout << "List is empty." << endl;
}

template <class T>
void Stack<T> :: get (T &item)
{
    getTop(item);
    pop();
}

//Identifier token Implementation
Identifier :: Identifier(string n, int d)
{
    name = n;
    data = d;
}

int Identifier :: getnum(string n, int &d)
{
    if(name == n)
    {
        d = data;
        return 1;
    }
    
    return 0;
}

void Identifier :: replace(int d)
{
    data = d;
}

void Identifier :: printide()
{
    cout << name << " = " << data << endl;
}

bool lowerPrecedence(string op, Stack<string> &opstack)
{
    bool lower = 0;
    if (!opstack.isEmpty())
    {
        string oper[2] = {op};
        opstack.getTop(oper[1]);

        for (int i = 0; i < 2; i++)
        {
            if(oper[i] == "(")
                oper[i] = 1;
            else if (oper[i] == "+" )
                oper[i] = 1;
            else if (oper[i] == "*")
                oper[i] = 3;
        }
        if (oper[0] < oper[1]) lower = 1;
    }
    return lower;
}

void calculate(Stack<int> &num,Stack<string> &op)
{
    int x, y;
    string a;

    op.get(a);

    num.get(x);
    num.get(y);

    if (a == "+")
        y += x;
    else if (a == "*")
        y *= x;

    num.push(y);
}

//Parser/Stack Calculator
int parser(vector<Identifier>&ide,vector<Token> tok)
{
    if(tok[0].gettype() != 1)
    {
        return -1;
    }
    else
    {
        Stack<string> operators;
        Stack<int> numbers;
    
        string name = tok[0].getdata();
        string op;
        int res;
        
        bool wasNum = 0;
        bool minus = 0;
        bool ending = 0;
        bool parenthesisnotminus = 1;
        int parenthesisminus = 0;

        for (int i = 1; i < tok.size(); i++)
        {
            if (tok[i].gettype() == 6)
            {
                ending = 1;
                break;
            }
            //Check for modifier
            else if(tok[i].gettype() == 2)
            {
                if(!wasNum)
                {
                    int num;
                    //Check if literal is NUM or identifier
                    if(isnumber(tok[i].getdata()))
                        num = stoi(tok[i].getdata());
                    else
                    {
                        int check = 0;
                        for (int j = 0; j < ide.size() && !check; j++)
                        {
                            check = ide[j].getnum(tok[i].getdata(),num);
                        }
                        if (!check)
                        {
                            return -1;
                        }
                    }

                    //Check if +ive or not
                    if(!minus)
                        numbers.push(num);
                    else
                    {
                        numbers.push(-num);
                        minus = 0;
                    }
                    
                    wasNum = 1;
                }   
            }
            //Case for '('and ')'
            else if(tok[i].gettype() == 4 || tok[i].gettype() == 5)
            {
                if(tok[i].gettype() == 4)
                {
                    if (minus)
                    {
                        parenthesisnotminus = 0;
                        minus = 0;
                        parenthesisminus++;
                    }
                    else 
                        parenthesisnotminus = 1;
                    operators.push("(");
                    wasNum = 0;
                }
                else if (tok[i].gettype() == 5)
                {
                    operators.getTop(op);
                    while (op != "(")
                    {
                        calculate(numbers,operators);
                        operators.getTop(op);
                    }
                    operators.pop();
                    if (!parenthesisnotminus)
                    {
                        int tominus;
                        numbers.get(tominus);
                        numbers.push(-tominus);
                        parenthesisminus--;
                    }
                    else
                    {
                        if(parenthesisminus)
                            parenthesisnotminus = 0;
                        else
                            parenthesisnotminus = 1;
                    }
                }
            }
            else if(tok[i].gettype() == 3)
            {
                string newop = tok[i].getdata();
                if (wasNum && lowerPrecedence(newop,operators))
                {
                    //Calculations will keep on being made until an operator with equal precedence is reached
                    while(lowerPrecedence(newop,operators))
                        calculate(numbers, operators);
                    //Exception for minus operator
                    if(newop == "-")
                    {
                        operators.push("+");
                        if(minus)
                            minus = 0;
                        else
                            minus = 1;
                    }
                    else
                        operators.push(newop);
                    wasNum = 0;
                }
                //Exception for minus operation
                else if(newop == "-")
                {
                    if(!wasNum)
                    {    
                        if(minus)
                            minus = 0;
                        else
                            minus = 1;
                    }
                    else
                    {
                        operators.push("+");
                        wasNum = 0;
                        if(minus)
                            minus = 0;
                        else
                            minus = 1;
                    }
                }
                else
                {
                    operators.push(newop);
                    wasNum = 0;
                }
            }
        }
        
        numbers.getTop(res);
        while (!operators.isEmpty())
        {
            calculate(numbers, operators);
            numbers.getTop(res);
        }
        
        Identifier result(name,res);
        if (ending)
            ide.push_back(result);
        else
            cout << "Syntax Error: ; required at the end." << endl;
    }
    
    return 0;
}