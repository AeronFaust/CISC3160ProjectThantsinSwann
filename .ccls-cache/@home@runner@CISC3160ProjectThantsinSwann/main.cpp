#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

string type[]= {"ERR", "Num", "Add", "Sub", "Mul", "Eql", "Lit", "Ide", "EOF"};

bool isLiteral = 0;

bool isNumber(const string& s)
{
    for (char const &ch : s) 
    {
        if (isdigit(ch) == 0) 
            return false;
    }
    return true;
}

bool isValidStr(const string s)
{
    char c = s[0];
    if (islower(c))
        return true;

    return false;
}

class Token
{
    private:
        int typeindex;
        string data;
    public:
        Token(string s);
        void settoken(string s);
        void printtoken();
};

int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " input.txt" << endl;
        return -1;
    }

    fstream fs;
    fs.open(argv[1]);

    string buffer;
    vector <Token> tok;

    while(!fs.eof())
    {
        getline(fs, buffer);
        stringstream str(buffer);
        while (getline(str,buffer, ' '))
        {
            if (buffer.find(';') != string::npos)
            {
                buffer.pop_back();
                Token t(buffer);
                tok.push_back(t);
                Token eof(";");
                tok.push_back(eof);
            }
            else
            {
                Token t(buffer);
                tok.push_back(t);
            }
        }
    }
    
    for (int i = 0; i < tok.size(); i++)
        tok[i].printtoken();
    
    fs.close();
    
    return 0;
}

//{"ERR", "Num", "Add", "Sub", "Mul", "Eql", "Lit", "Ide", "EOF"};      
Token::Token(string s)
{
    if(isNumber(s))
    {
        
    }
    else if(isNumber(s))
    {
        if(s == "+")
            typeindex = 2;
            
        else if(s == "-")
            typeindex = 3;
            
        else if(s == "*")
            typeindex = 4;
            
        else if(s == "=")
        {
            typeindex = 5;
            isLiteral = 1;
        }
            
        else if(s == ";")
        {
            typeindex = 7;
            isLiteral = 0;
        }
            
        else if(isValidStr(s))
        {
            if(isLiteral)
                typeindex = 5;
            else
                typeindex = 6;        
        }
            
        else
            typeindex = 0;
    }
}

void Token::settoken(string s)
{
    data = s;  
}

void Token::printtoken()
{
    cout << data << endl;  
}