#pragma once
#include <string>
#include <cmath>
#include <iostream>
#include "Coordinates.cpp"

std::string ToUpper(std::string data){ // Converts a string to uppercase
    for (int i = 0; i < data.length(); i++){
        if (data[i] >= 'a' && data[i] <= 'z') data[i] -= 32;
    }
    return data;
}

int DecimalFinder(std::string data){ // Finds the number of the decimal points in a string
    int counter = 0;
    for (int i = 0; i < data.length(); i++){
        if (data[i] == '.') counter++;
    }
    return counter;
}

bool IsThereChar(std::string data){ // Checks if there is a character in a string
    for (int i = 0; i < data.length(); i++){
        if (data[i] >= 'a' && data[i] <= 'z' || (data[i] >= 'A' && data[i] <= 'Z') || data[i] == 92 || (data[i] == '"' && i !=0 && i != data.length() - 1)) return true;
    }
    return false;
}

double ConvertToDouble(std::string data){ // Converts a string to a double
    double num = 0;
    bool isNegative = false;
    if (data[0] == '-'){
        isNegative = true;
        data.erase(0, 1);
    }
    int decimalPlace = -1;
    for (int i = 0; i < data.length(); i++){
        if (data[i] == '.'){
            decimalPlace = i;
            break;
        }
    }
    for (int i = 0; i < data.length(); i++){
        if (decimalPlace == -1){
            num += (data[i] - '0') * pow(10, data.length() - i - 1);
        }else{
            if (i < decimalPlace) num += (data[i] - '0') * pow(10, decimalPlace - i - 1);
            else if (i > decimalPlace) num += (data[i] - '0') * pow(10, decimalPlace - i);
        }
    }
    if (isNegative) num *= -1;
    return num;
}

Coordinates StringCoordinateSplit(std::string data){ // Splits a string into coordinates
    Coordinates coordinates;

    data.erase(0, 1);

    while(data[0] != 'C'){
        coordinates.row *= 10;
        coordinates.row += data[0] - '0';
        data.erase(0, 1);
    }

    if (data[0] == 'C'){
        data.erase(0, 1);

        while(data[0] != '\0'){
            coordinates.col *= 10;
            coordinates.col += data[0] - '0';
            data.erase(0, 1);
        }
    }
    coordinates.row--;
    coordinates.col --;

    return coordinates;
}

int NumberOfDigits(double num){ // Finds the number of digits in a number
    int counter = 0;
    std::string num2 = std::to_string(num);

    while (num2[num2.length() - 1] == '0' || num2[num2.length() - 1] == '.'){
        num2.erase(num2.length() - 1, 1);
        if (num2[num2.length() - 1] == '.') {
            num2.erase(num2.length() - 1, 1);
            break;
        }
    }
    for (int i = 0; i < num2.length(); i++){
         counter++;
    }
    return counter;
}
