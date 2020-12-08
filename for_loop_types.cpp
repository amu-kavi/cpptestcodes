#include <iostream>
#include <vector>
using namespace std;

int main(){
    // for (initialization; condition; increase) statement;
    for (int i =0; i < 10; i++){
        if (5 == i) continue;
        cout << "i: " << i << endl;
    }

    //breaks continues the next looping  
    cout << "second list"<< endl;
    for (int i =0; i < 10; i++){
        if (5 == i) break;
        cout << "i: " << i << endl;
    }


    //breaks the loop and comes out of loop
    vector<string> strVec1={"Kavitha","Deep","Kavin"};
    vector<string> strVec2;
    strVec2.push_back("kavitha");
    strVec2.push_back("Kavin");
    strVec2.push_back("Deep");
    strVec1.push_back("lala");
    for(auto& name : strVec1){
        cout<< name << endl;
    }


    for (int i =0; i < strVec2.size(); i++){
        cout <<i << " :"<<strVec2[i]<< endl;
    }
    cout << "end"<< endl;
    return 0;
}
