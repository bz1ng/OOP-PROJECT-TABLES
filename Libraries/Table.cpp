#pragma once
#include "Functions.cpp"
#include "Cell.cpp"
#include <fstream>

class Table{
    private:
        int rows = 0;
        int columns = 0;
        Cell** cells;
        std::string name;
        std::string FilePath;

    public:
    bool Open(std::string name /*string path*/ ){ // Opens a file and reads the data
        int maxCol = 0;
        this->name = name;
        this->FilePath = "Files\\" + name;
        /*this->FilePath = path;
        std::string temp = "";
        for (int i = path.length() - 1; path[i] != '\\' ; i--) temp += path[i];
        for (int i = temp.length() - 1; i >= 0; i--) this->name += temp[i];
         */
        std::ifstream File;
        File.open(FilePath);
        if (File.good()){
            std::cout << "Successfully opened "<< this->name << std::endl;
            std::string data;
            while (File){ // Finds the number of rows and columns
                std::getline(File, data);
                for (int i = 0; i < data.length(); i++){
                    if (data[i] == ',' && data[i-1] >= '0' && data[i-1] <= '9' && (data[i+1] == ' ' || i == data.length() - 1)) maxCol++;
                    else if ((i==0 && data[i + 1 ] == ',') || (data[i] == ',' && data[i-1] == ' ' && data[i+1] == ' ' )) maxCol++;
                    else if (data[i] == ',' && data[i-1] == '"' && data[i-2] != 92 && (data[i+1] == ' ' || i == data.length() - 1)) maxCol++;
                }
                maxCol++;
                if (maxCol > this->columns) this->columns = maxCol;
                maxCol = 0;
                this->rows++;
            }

            this->rows--; // Because the last line is empty

            cells = new Cell*[rows]; // Creates the table
            for (int i = 0; i < this->rows; i++){
                cells[i] = new Cell[this->columns];
            }

        }else{

            std::cout << "File " << name << " is not found" << std::endl;
            return false;
        }

        File.close();


        File.open(FilePath);

        int rowCounter = 0;
        int colCounter = 0;

        while (File){
            std::string data;
            getline(File, data);
            std::string temp = "";

            if (data == "" && rowCounter < this->rows) { // if the whole row is empty
                for (int i = 0; i < this->columns; i++){
                    cells[rowCounter][i].setData(temp);
                }
            }else {
                for(int i = 0; i < data.length(); i++){ // Fills the table with data from file
                    if (temp[0] == ' ') temp.erase(0, 1);
                    if ((i == 0 && data[i] == ',' && data[i + 1] == ' ') || (data[i] == ',' && data[i-1] == ' ' && data[i+1] == ' ')){
                        cells[rowCounter][colCounter].setData(temp);
                        colCounter++;
                        temp = "";
                    }else if (data[i] == ',' && data[i-1] == '"' && ((data[i-2] != 92 && data[i-2] != '"') || (data[i-2] == 92 && data[i-3] == 92 ))&& (data[i+1] == ' ' || data[i+1] == '\0')){ // If the data is a string 
                        cells[rowCounter][colCounter].setData(temp);
                        colCounter++;
                        temp = "";
                    }else if (data[i] == ',' && data[i-1] >= '0' && data[i-1] <= '9' && (data[i+1] == ' ' || data[i+1] == '\0')){ // If the data is a number
                        cells[rowCounter][colCounter].setData(temp);
                        colCounter++;
                        temp = "";
                    }else if (i == data.length() - 1 && data[i] == ' '){ // If the data is empty
                        cells[rowCounter][colCounter].setData(temp);
                        colCounter++;
                        temp = "";
                    }else if (i == data.length() - 1){ // If the data is the last one in the line
                        temp += data[i];
                        cells[rowCounter][colCounter].setData(temp);
                        temp = "";
                    }else {
                        temp += data[i]; // If the data is not the last one in the line
                    }
                }
            }
            rowCounter++;
            colCounter = 0;
        }
        File.close();
        return true;
    }

    void Create(std::string name /*std::string path*/, int rows, int columns){ // Creates a new table
        this->name = name;
        this->FilePath = "Files\\" + name;
        /*this->FilePath = path;
         std::string temp = "";
        for (int i = path.length() - 1; path[i] != '\\' ; i--) temp += path[i];
        for (int i = temp.length() - 1; i >= 0; i--) this->name += temp[i];
         */
        std::ofstream File;
        File.open(FilePath);

        if(File.good()){
            this->rows  = rows;
            this->columns = columns;
            cells = new Cell*[rows];
            for (int i = 0; i < rows; i++){
                cells[i] = new Cell[columns];
            }
        } else std::cout << "Something went wrong with creating "<< name << std::endl;
        File.close();
    }
    
    void Generate(){ // Generates the table (Calculates the equations)
        for(int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                if (cells[i][j].getType() == "equation"){
                    CalculateFunction(cells[i][j]);
                }
            }
        }
    }

    void Print(){ // Prints the table

        std::cout << std::endl;
        std::cout << "Name of the table: " << name << std::endl; // Prints name of table

        int* MaxLen = new int[columns + 1];  //array for storing the max length of each column 
        MaxLen[0] = NumberOfDigits(rows) + 1; // Length of the first column (row number)

        for (int i = 1; i <= columns; i++){
            for (int j = 0; j < rows; j++){
                if (cells[j][i - 1].getLength() > MaxLen[i]) MaxLen[i] = cells[j][i - 1].getLength(); // If the data is longer than the column name
                else if(NumberOfDigits(i) + 1 > MaxLen[i]) MaxLen[i] = NumberOfDigits(i) + 1; // If the column name is longer than the data
            }
        }

        //THE PRETTY ONE 👇

        std::cout << "╔"; // Prints top border row
        for (int i = 0; i < columns + 1; i++){
            for (int j = 0; j < MaxLen[i] + 2; j++){
                std::cout << "═";
            }
            if (i != columns) std::cout << "╦";
            else std::cout << "╗";
        }
        std::cout << std::endl;



        for (int i = 0; i < columns + 1; i++){ // Prints column numbr row
            if (i == 0) {
                std::cout << "║";
                for(int j = 0; j < MaxLen[i] + 2; j++) std::cout << " ";
                std::cout << "║ ";
            }else{
                std::cout << "C" << i;
                for (int j = 0; j < MaxLen[i] - NumberOfDigits(i) - 1; j++) { std::cout << " ";}
                std::cout << " ║ ";
            }
        }
        std::cout << std::endl;
        

        for (int i = 0; i < rows; i++){ 

                std::cout << "╠"; // Prints the border between column number row and data
                for (int j = 0; j < columns + 1; j++){
                    for (int k = 0; k < MaxLen[j] + 2; k++)std::cout << "═";
                    if (j != columns) std::cout << "╬";
                    else std::cout << "╣";
                }
                std::cout << std::endl;

                std::cout << "║ ";


            for (int j = 0; j < columns + 1; j++){ // Prints the data
                if (j == 0) std::cout << "R" << i + 1 ;
                else if (cells[i][j - 1].getType() == "equation" && cells[i][j - 1].getError() == false) {
                    std::cout << cells[i][j - 1].getValue();
                }else if (cells[i][j - 1].getType() == "equation" && cells[i][j - 1].getError() == true){
                    std::cout << "Error";
                }else if(cells[i][j - 1].getType() == "string"){
                    for (int k = 1; k < cells[i][j - 1].getData().length() - 1; k++){
                        if ((cells[i][j - 1].getData()[k] == 92 && cells[i][j - 1].getData()[k+1] == '"') || (cells[i][j - 1].getData()[k] == 92 && cells[i][j - 1].getData()[k + 1] == 92)) {
                            std::cout << cells[i][j - 1].getData()[k + 1];
                            k++;
                        }else std::cout << cells[i][j - 1].getData()[k];
                    }
                }else std::cout << cells[i][j - 1].getData();

                if (j == 0){
                    for (int k = 0; k < MaxLen[j] - NumberOfDigits(i+1) - 1; k++) std::cout << " ";
                }else{
                    for (int k = 0; k < MaxLen[j] - cells[i][j-1].getLength(); k++){ // prints spaces to make the table look nice
                        std::cout << " ";
                    }

                }
                std::cout << " ║ ";
                

            }
            std::cout << std::endl;
        }

            std::cout << "╚"; // Prints bottom border row
            for (int i = 0; i < columns + 1; i++){
                for (int j = 0; j < MaxLen[i] + 2; j++){
                    std::cout << "═";
                }
                if (i != columns) std::cout << "╩";
                else std::cout << "╝";
            }
            std::cout << std::endl;

        //THE WORKING ONE 👇

        /*for (int i = 0; i < columns + 1; i++){ // Prints column numbr row
            if (i == 0) {
                for(int j = 0; j < MaxLen[i] + 2; j++) std::cout << " ";
                std::cout << "| ";
            }else{
                std::cout << "C" << i;
                for (int j = 0; j < MaxLen[i] - NumberOfDigits(i) - 1; j++) std::cout << " ";
                std::cout << " | ";
            }
        }

        std::cout << std::endl;
        

        for (int i = 0; i < rows; i++){ // Prints the data

            for (int j = 0; j < columns + 1; j++){
                if (j == 0) std::cout << " R" << i + 1 ; // Prints row number
                else if (cells[i][j - 1].getType() == "equation" && cells[i][j - 1].getError() == false) std::cout << cells[i][j - 1].getValue(); // Prints the value of the equation
                else if (cells[i][j - 1].getType() == "equation" && cells[i][j - 1].getError() == true) std::cout << "Error"; //print ERROR if there is something wrong with equation
                else if(cells[i][j - 1].getType() == "string"){ // If the data is a string
                    for (int k = 1; k < cells[i][j - 1].getData().length() - 1; k++){
                        if ((cells[i][j - 1].getData()[k] != 92 && cells[i][j - 1].getData()[k+1] != '"') || (cells[i][j - 1].getData()[k] != 92 && cells[i][j - 1].getData()[k + 1] != 92)) std::cout << cells[i][j - 1].getData()[k];
                    }
                }else std::cout << cells[i][j - 1].getData(); // If the data is a number

                if (j == 0){  // Prints the spaces to make table evenly spaced
                    for (int k = 0; k < MaxLen[j] - NumberOfDigits(i+1) - 1; k++) std::cout << " ";
                }else{
                    for (int k = 0; k < MaxLen[j] - cells[i][j-1].getLength(); k++) std::cout << " ";
                }
                std::cout << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
    }

    void SetData( std::string data, int row, int column){ // Sets data to a cell function
        if(cells[row][column].setData(data) == 0) std::cout << "Error: "<< "row" << row + 1 << ", col" << column + 1 << ", "<< data << " is unknown data type" << std::endl; // If the data is invalid
    }

    void GetInfoOfCell(int row, int column){ // Prints info of a cell
        std::cout << std::endl;

        std::cout << "Info of cell R" << row + 1 << " C" << column + 1 << std::endl; // Prints the row and column number

        if (cells[row][column].getType() == "equation" && cells[row][column].getError() == true){ // If the cell is an equation and there is an error
            std::cout << "Data: " << cells[row][column].getData() << std::endl;
            std::cout << "Type: " << cells[row][column].getType() << std::endl;
            std::cout << "Value: " << "Error" << std::endl; 
        } else if (cells[row][column].getType() == "equation" && cells[row][column].getError() == false){ // If the cell is an equation and there is no error
            std::cout << "Data: " << cells[row][column].getData() << std::endl;
            std::cout << "Type: " << cells[row][column].getType() << std::endl;
            std::cout << "Value: " << cells[row][column].getValue() << std::endl;
        }else{ // If the cell is not an equation (int, double, string)
            std::cout << "Data: " << cells[row][column].getData() << std::endl;
            std::cout << "Type: " << cells[row][column].getType() << std::endl;
        }

        std::cout << std::endl;
    }

    void CalculateFunction(Cell& current){ // Calculates the value of a function in table

        std::string data = current.getData();
        data.erase(0, 1); // Removes the '=' from the equation
        std::string left = "";
        std::string right = "";
        char operation = ' ';
        double ifFunction; // If one of arguments is a function

        while (data[0] != '\0'){ // Splits the equation into left, right and operation
            if (operation == ' '){

                if (data[0] == '+' || data[0] == '-' || data[0] == '*' || data[0] == '/' || data[0] == '^'){
                    operation = data[0];
                    data.erase(0, 1);
                }else{
                    left += data[0];
                    data.erase(0, 1);
                }
            }else{
                right += data[0];
                data.erase(0, 1);
            }

        }

        if (left[0] == 'R') // If the left side is a cell
        {
            int ROW = StringCoordinateSplit(left).row; // Gets the row of the cell
            int COLUMN = StringCoordinateSplit(left).col; // Gets the column of the cell
            if (cells[ROW][COLUMN].getType() == "integer" || cells[ROW][COLUMN].getType() == "decimal") left = cells[ROW][COLUMN].getData(); // If the left side is a number it gives it the value of the number
            else if (cells[ROW][COLUMN].getType() == "equation") left = std::to_string(cells[ROW][COLUMN].getValue()); // If the left side is an equation it gives it the value of the equation
            else if (cells[ROW][COLUMN].getType() == "string" || cells[ROW][COLUMN].getType() == "empty") left = "0"; // If the left side is a string it gives it the value of 0
        }
        if (right[0] == 'R'){ // If the right side is a cell
            int ROW = StringCoordinateSplit(right).row; // Gets the row of the cell
            int COLUMN = StringCoordinateSplit(right).col;  // Gets the column of the cell
            if (cells[ROW][COLUMN].getType() == "integer" || cells[ROW][COLUMN].getType() == "decimal") right = cells[ROW][COLUMN].getData(); // If the right side is a number it gives it the value of the number
            else if (cells[ROW][COLUMN].getType() == "equation") right = std::to_string(cells[ROW][COLUMN].getValue()); // If the right side is an equation it gives it the value of the equation
            else if (cells[ROW][COLUMN].getType() == "string" || cells[ROW][COLUMN].getType() == "empty") right = "0"; // If the right side is a string it give it the value of 0
        }
        if (left[0] >= '0' && left[0] <= '9' && right[0] >= '0' && right[0] <= '9'){ // If both sides are numbers
            if ( DecimalFinder(left) == 0 ) left += ".0"; // If the left side is an integer it adds .0 to it
            if ( DecimalFinder(right) == 0 ) right += ".0"; // If the right side is an integer it adds .0 to it
        }

        double leftSide = ConvertToDouble(left); // Converts the left side to double
        double rightSide = ConvertToDouble(right); // Converts the right side to double

        if (rightSide == 0 && operation == ' ') current.setValue(leftSide); // if the function is with one argument
        else if (operation == '/' && rightSide == 0) current.setError(true); // if the function is division by 0
        else if (operation == '^' && leftSide == 0 && rightSide == 0) current.setError(true); // if 0^0
        else if (operation == '^') current.setValue(pow(leftSide, rightSide)); // if the function is power
        else if (operation == '+') current.setValue(leftSide + rightSide); // if the function is addition
        else if (operation == '-') current.setValue(leftSide - rightSide); // if the function is subtraction
        else if (operation == '*') current.setValue(leftSide * rightSide); // if the function is multiplication
        else if (operation == '/' && rightSide != 0) current.setValue(leftSide / rightSide); // if the function is division
    }

    void Save(){ // Function that saves the table
        std::ofstream File;
        File.open(this->FilePath);
        if(File.good()){
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < columns; j++){
                        bool isEmpty = true;
                        if (cells[i][j].getType() == "empty"){
                            int emptyCounter = 1;
                            for (int k = j + 1; k < columns; k++){
                                if (cells[i][k].getType() == "empty") emptyCounter++;
                                else break;
                            }
                            if (emptyCounter == columns - j || emptyCounter == columns) {
                                File << std::endl;
                                break;
                            }
                            else File << ", ";
                        }else if (j != columns - 1) File << cells[i][j].getData() << ", ";
                        else File << cells[i][j].getData() << std::endl;
                }
            }
            std::cout << "File saved successfully!" << std::endl;
        }else std::cout << "There was a problem with saving the file" << std::endl;
        File.close();
        return;
    }

    void SaveAS(std::string newName /*std::string newPath*/){ // Function that saves the table as a new file
        std::ofstream File;
        std::string NewFilePath =  "Files\\" + newName;
        File.open(NewFilePath /*newPath*/); 
        if (File.good()){
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < columns; j++){                                      
                    if (cells[i][j].getType() == "empty"){
                        bool isEmpty = true;
                        for (int k = j + 1; k < columns; k++){
                            if (cells[i][k].getType() != "empty"){
                                isEmpty = false;
                                File << ", ";
                                break;
                            }
                        }
                        if (isEmpty == true) {
                            File << std::endl;
                            break;
                        }
                    }else if (j != columns - 1) File << cells[i][j].getData() << ", ";
                    else File << cells[i][j].getData() << std::endl;
                }
            }
            std::cout << "File saved successfully!" << std::endl; 
        } else std::cout << "There was a problem with saving the file" << std::endl;                                                                          
        File.close();           
        return;
    }

    void Clear(){ // Clears the table
    if (cells != nullptr && rows != 0 && columns != 0 && FilePath != "" && name != ""){
        for (int i = 0; i < rows; i++){
            delete[] cells[i];
        }
        delete[] cells;
    }
        this -> FilePath = "";
        this -> rows = 0;
        this -> columns = 0;
        this -> name = "";
    }

    ~Table(){ // Destructor
        for (int i = 0; i < rows; i++){
                delete[] cells[i];
        }
        delete[] cells;
    }
};