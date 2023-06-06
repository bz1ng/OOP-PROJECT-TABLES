#pragma once
#include "functions.cpp"

class Cell{ // Class for a cell
    private:
        std::string data;
        std::string type;
        double value;
        bool error = false;
        int length = -1;
    public:
        Cell(){ // Default constructor
            data = "";
            type = "empty";
        }

        Cell(std::string data, std::string type, double value, bool error, int length){ // Constructor with parameters
            this->data = data;
            this->type = type;
            this->value = value;
            this->error = error;
            this->length = length;
        }

        Cell(const Cell& other){ // Copy constructor
            this->data = other.data;
            this->type = other.type;
            this->value = other.value;
            this->error = other.error;
            this->length = other.length;
        }

        Cell& operator=(const Cell& other){ // Assignment operator
            if (this != &other){
                this->data = other.data;
                this->type = other.type;
                this->value = other.value;
            }
            return *this;
        }

        bool setData(std::string data){ // Sets the data of the cell
            if (data == ""){ // Checks if the data is empty
                this->data = data;
                this->type = "empty";
                this->length = -1;
                return true;
            }else if (data[0] == '"' && data[data.length() - 1] == '"'){ // Checks if the data is a string
                this->data = data;
                this->type = "string";
                this->length = -1;
                return true;
            }else if (data[0] == '='){ // Checks if the data is a equation
                this->data = data;
                this->type = "equation";
                this->length = -1;
                return true;
            }else if (DecimalFinder(data) == 1){  // Checks if the data is a decimal
                this->data = data;
                this->type = "decimal";
                this->length = -1;
                return true;
            }else if (IsThereChar(data) == false && DecimalFinder(data) == 0){ // Checks if the data is an integer
                this->data = data;
                this->type = "integer";
                this->length = -1;
                return true;
            }else return false;
        }

        void setValue(double value){ // Sets the value of the cell
            this->value = value;
        }

        double getValue(){ // Gets the value of the cell
            return value;
        }

        void setError(bool error){ // Sets the error of the cell
            this->error = error;
        }

        bool getError(){ // Gets the error of the cell
            return error;
        }

        std::string getData(){ // Gets the data of the cell
            return data;
        }

        std::string getType(){ // Gets the type of the cell
            return type;
        }

        int getLength(){ // Gets the length of the cell
            if (this->length != -1 ){
                if (this->type == "equation" && this->error == false && this->length != NumberOfDigits(this->value)){
                    this->length = NumberOfDigits(this->value);
                    return this->length;
                }else if (this->type == "equation" && this->error == true && this->length != 5){
                    this->length = 5;
                    return this->length;
                }else return this->length;
            }else{
                this -> length = 0;
                if (this->type == "integer" || this->type == "decimal") this->length = this->data.length(); // Finds the length of the data if int or double
                else if (this->type == "equation" && this->error == false)this->length = NumberOfDigits(this->value); // Finds the number of digits in the value of equation
                else if (this->type == "equation" && this->error == true) this->length = 5; // If the equation is invalid
                else if (this->type == "empty") this->length = 0; // If the cell is empty
                else if (this->type == "string") { // If the cell is a string
                    for (int i = 0; i < this->data.length(); i++){
                        if ((this->data[i] == 92 && this->data[i + 1] == '"') || (this->data[i] == 92 && this->data[i + 1] == 92)){
                            this->length++;
                            i++;
                        }else this->length++;
                    }
                    this->length -= 2;
                }
                
                return this->length;
            }
        }
};