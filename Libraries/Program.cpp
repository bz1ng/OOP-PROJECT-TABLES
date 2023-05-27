#include "Table.cpp"
#include <iostream>

// help command text
const std::string help = "\nCOMMANDS:\nopen <table name> - opens a table\nclose - closes the current table\nview <0 or 1> - changes the view of the table 1 - basic, 0 - more good looking one \nsave - saves the current table\nsaveas <table name> - saves the current table as <table name>\nhelp - prints this information\nexit - exits the program\nprint - prints the current table\ninfo <row> <col> - prints information about the cell at <row> <col>\nedit <row> <col> <new data> - edits the cell at <row> <col> with <new data>\n\nDATA TYPES:\nstring - \"<data>\" (use \\\" to type quotation mark and \\\\ to type backslash)\ndecimal - <data>\ninteger - <data>\nequation - =<data> (supported operations +, -, *, /, ^)\n";


void Program(){ // Main function of the program
    std::string line = "";
    std::string command[4] = {"", "", "", ""};
    bool isTableOpen = false;
    Table table;

        while(ToUpper(command[0]) != "EXIT"){ // Main loop of the program

        std::cout << ">>Command: "; // User input
        getline(std::cin, line);
        int p = 0;
        for (int i = 0; i < line.length(); i++){  // Splits the input into words
            if (line[i] == ' '){
                p++;
            }else {
                command[p] += line[i];
            }
        }

        if (ToUpper(command[0]) == "OPEN" && isTableOpen == false) { // If the user wants to open a table
            if (table.Open(command[1]) != false) {
                isTableOpen = true; 
            }
        }else if (ToUpper(command[0]) == "OPEN" && isTableOpen == true){ // If the user wants to open a table but there is already one open one
            std::cout << "A table is already open. Do you want to save it? Y/N" << std::endl;
            std::string answer = "";
            getline(std::cin, answer);
            if (ToUpper(answer) == "Y") table.Save();

            table.Clear(); 
            isTableOpen = true;
            table.Open(command[1]);
        }else if (ToUpper(command[0]) == "CREATE" && isTableOpen == false){ // If the user wants to create a table
            isTableOpen = true;
            table.Create(command[1], int(ConvertToDouble(command[2])), int(ConvertToDouble(command[3])));
        }else if (ToUpper(command[0]) == "CREATE" && isTableOpen == true){ // If the user wants to create a table but there is already one open one
            std::cout << "A table is already open. Do you want to save it? Y/N" << std::endl;
            std::string answer = "";
            getline(std::cin, answer);
            if (ToUpper(answer) == "Y") table.Save();

            table.Clear();
            isTableOpen = true;
            table.Create(command[1], int(ConvertToDouble(command[2])), int(ConvertToDouble(command[3])));
        }else if (ToUpper(command[0]) == "HELP") std::cout << help << std::endl; // If the user wants to see the help menu
        else if (ToUpper(command[0]) == "PRINT" && isTableOpen == true){ // If the user wants to print the table
            table.Generate();
            table.Print();
        }
        else if (ToUpper(command[0]) == "PRINT" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to print but there is no open table
        else if (ToUpper(command[0]) == "INFO" && isTableOpen == true) table.GetInfoOfCell(int(ConvertToDouble(command[1])) - 1, int(ConvertToDouble(command[2])) - 1 ); // If the user wants to see the info of a cell
        else if (ToUpper(command[0]) == "INFO" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to see info of a cell but there is no open table
        else if (ToUpper(command[0]) == "EDIT" && isTableOpen == true) { // If the user wants to edit a cell
            table.SetData(command[3], int(ConvertToDouble(command[1])) - 1, int(ConvertToDouble(command[2])) - 1);
            table.Generate();
        }
        else if (ToUpper(command[0]) == "VIEW") { // If the user wants to change the view of the table
                if (command[1] == "0" || command[1] == "1") {
                    table.SetView(command[1][0]);
                    std::cout << "View changed" << std::endl;
                }else std::cout << command[1] << " is not a valid view" << std::endl;

        }else if (ToUpper(command[0]) == "EDIT" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to edit a cell but there is no open table
        else if (ToUpper(command[0]) == "SAVE" && isTableOpen == true) table.Save(); // If the user wants to save the table
        else if (ToUpper(command[0]) == "SAVE" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to save but there is no open table
        else if (ToUpper(command[0]) == "SAVEAS" && isTableOpen == true) table.SaveAS(command[1]); // If the user wants to save the table as a new file
        else if (ToUpper(command[0]) == "SAVEAS" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to save as but there is no open table
        else if (ToUpper(command[0]) == "CLOSE" && isTableOpen == true) { // If the user wants to close the table
            table.Clear();
            isTableOpen = false;
            std::cout << "Table closed successfully!" << std::endl;
        }
        else if (ToUpper(command[0]) == "CLOSE" && isTableOpen == false) std::cout << "No table is open" << std::endl; // ERROR if the user wants to close but there is no open table
        else if (ToUpper(command[0]) == "EXIT") break; // If the user wants to exit the program
        else std::cout << "Invalid command" << std::endl; // ERROR if the user inputs an invalid command
        
        line = "";
        for (int i = 0; i < 4; i++) command[i] = "";
    }

}
