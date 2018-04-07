//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: qiyang xia
// Engineer id:100837661
// Create Date:    2016/12/12
// Design Name:    readfile 
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>  
#include <fstream>  
using namespace std;  
void testByChar()  
{  
    fstream testByCharFile;  
    char c;  
    testByCharFile.open("example_TOP_input1",ios::in);  
    while(!testByCharFile.eof())  
    {  
        testByCharFile>>c;  
        cout<<c;  
    }  
    testByCharFile.close();  
}  
void testByLine()  
{  
    char buffer[256];  
    fstream outFile;  
    outFile.open("example_TOP_input1.txt",ios::in);  
    cout<<"example_TOP_input1.txt<<--- all file is as follows:---"<<endl;  
    while(!outFile.eof())  
    {  
        outFile.getline(buffer,256,'\n');
        cout<<buffer<<endl;  
    }  
    outFile.close();  
}  
int main()  
{  
   //cout<<endl<<"testByChar():"<<endl<<endl;  
   //testByChar();  
   cout<<endl<<"testByLine():"<<endl<<endl;  
   testByLine();  
}  



