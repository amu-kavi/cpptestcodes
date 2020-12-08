#include <iostream>

int main(){
    unsigned short myNum;
    std::cout << "Enter number : ";
    std::cin >> myNum;

   /* if(0 == myNum){
        std::cout << "Invalid number0" << std::endl;
        return 0;
    }else*/ if(myNum == 0){
        std::cout << "Invalid number1" << std::endl;
        return 0;
    }
    else if (0 == myNum % 2){
        std::cout << "EVEN number" << std::endl;
    }
    else{
        std::cout << "ODD number" << std::endl;
    }

    //(0 == myNum % 2) ? std::cout << "EVEN number" << std::endl : \
    std::cout << "ODD number" << std::endl;;

    return 0;
}