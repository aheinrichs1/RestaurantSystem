#include <iostream>
#include <vector>
using namespace std;

//function declarations:
void displayMenu();
void displayTables();
void validateMenuSelection(int &input);
void validateTableSelection(int &input);
void validateBoolSelection(int &input);
bool areAllTablesClosed();
bool areAllTablesOpen();

//Declare Program Variables
const int NUMBER_OF_TABLES = 19;
//Based on the plain restaurant layout created by Suzette
//6 Tables in main area, 7 Booths, and 5 Tables in party room

//Bar seating not implemented yet

vector<bool> isTableOccupied; //Early working model
//TODO: create a Table object, and a vector list of Table objects

int main() {

    //variables for main:
    int userInput; //int used for user to select and navigate menus
    int tableSelection; //used to select tables for adding, editing, or deleting status
    bool endDay = false; //bool used to end user input loop

    //Start day:

    //Reset tables (maybe find a way to save table status at a later point)
    isTableOccupied = vector<bool>(NUMBER_OF_TABLES, false);

    //START USER INPUT LOOP (runs until end of day):
    while(endDay == false) {
        //Display menu:
        displayMenu();

        cout << "Choose an option: ";
        cin >> userInput;
        //validate input
        validateMenuSelection(userInput);
        cout << endl;

        //Seat party
        if(userInput == 1) {
            cout << "Seat party:" << endl <<
                    "-----------" << endl;
            //display tables
            displayTables();

            cout << endl << "Which table would you like to seat party at? ";
            cin >> tableSelection;
            //Validate input
            validateTableSelection(tableSelection);
            //Check that table selected is open
            while(isTableOccupied[tableSelection - 1]) {
                cout << "Sorry, that table is occupied. Choose another table: ";
                cin >> tableSelection;
                //Validate input
                validateTableSelection(tableSelection);
            }
            //Set table selected to occupied and notify user
            isTableOccupied[tableSelection - 1] = true;
            cout << "Table number " << tableSelection << " has been set to occupied." << endl <<
                    "--------------------------------------" << endl;
        }

        //Edit table
        else if(userInput == 2) {
            //display tables
            displayTables();
            cout << "Which table would you like to edit? ";
            cin >> tableSelection;
            //Validate input
            validateTableSelection(tableSelection);
            cout << endl << "Editing table " << tableSelection << endl;
            cout << "Enter 1 to set table to occupied, Enter 0 to set table to open: ";
            cin >> userInput;
            //Validate input
            validateBoolSelection(userInput);
            isTableOccupied[tableSelection - 1] = userInput;
            cout << "Table " << tableSelection << " has been set to " <<
                    (isTableOccupied[tableSelection - 1] ? "Occupied" : "Open") << endl <<
                    "--------------------------------------" << endl;

        }

        //Unseat party (reset table)
        else if(userInput == 3) {
            //display tables
            displayTables();
            if (areAllTablesOpen()) {
                cout << endl << "All tables are open!" << endl;
                cout << "Returning to main menu" << endl << endl;
            } else {
                cout << endl << "Which table would you like to clear? ";
                cin >> tableSelection;
                //Validate input
                validateTableSelection(tableSelection);
                //Check that table selected is occupied
                while(!isTableOccupied[tableSelection - 1]) {
                    cout << "Sorry, that table is already open. Choose another table: ";
                    cin >> tableSelection;
                    //Validate input
                    validateTableSelection(tableSelection);
                }
                cout << "Clearing table " << tableSelection << endl;
                isTableOccupied[tableSelection - 1] = 0;
                cout << "--------------------------------------" << endl;
            }
        }

        //Check all table status
        else if(userInput == 4) {
            displayTables();
            cout << endl << endl;
        }

        //View reservations for the day
        else if(userInput == 5) {

        }

        //Make reservation
        else if(userInput == 6) {

        }

        //Cancel/remove reservation
        else if(userInput == 7) {

        }

        //End day
        else if(userInput == 9) {
            cout << "Are you sure you would like to end the day?" << endl;
            cout << "Enter 1 to accept or enter 0 to cancel: ";
            cin >> userInput;
            validateBoolSelection(userInput);
            if(userInput == 1) {
                cout << "Ending day." << endl;
                endDay = 1;
            } else {
                cout << "Cancelled, returning to main menu." << endl <<
                        "--------------------------------------" << endl;
            }
        }

    }
    return 0;
}

void displayMenu() {
    cout << "Main Menu:" << endl <<
            "----------" << endl <<
            "1. Seat party" << endl <<
            "2. Edit table" << endl <<
            "3. Clear table" << endl <<
            "4. Check table status" << endl <<
            "5. View Reservations" << endl <<
            "6. Make Reservation" << endl <<
            "7. Cancel Reservation" << endl <<
            "---------------------" << endl <<
            "9. End Day" << endl << endl;
}

void displayTables(){
    //Display tables and availability:
    for(size_t i = 0; i < isTableOccupied.size(); i++) {
        if(i == 0) cout << "MAIN AREA:" << endl <<
                           "----------" << endl;
        if(i == 6) cout << endl << endl << "BOOTHS:" << endl <<
                           "-------" << endl;
        if(i == 12) cout << endl << endl << "PARTY ROOM:" << endl <<
                            "-----------" << endl;


        //print each table and status
        cout << i + 1 << ":" << (isTableOccupied[i] ? "Occupied" : "Open") << " ";
    }
    cout << endl;
}

void validateMenuSelection(int &input) {
    while((input < 1) || (input > 9) || (input == 8)) {
          cout << "Invalid input" << endl <<
                  "Choose an option: ";
          cin >> input;
    }
}

void validateTableSelection(int &input) {
    while((input < 1) || (input > NUMBER_OF_TABLES)) {
        cout << "Invalid selection" << endl;
        cout << "Choose a table between 1 and " << NUMBER_OF_TABLES;
        cin >> input;
    }
}

void validateBoolSelection(int &input) {
    while((input != 0) && (input != 1)) {
        cout << "Invalid selection" << endl;
        cout << "Enter a 1 or a 0: ";
        cin >> input;
    }
}

bool areAllTablesClosed() {
    for(size_t i = 0; i < NUMBER_OF_TABLES; i++) {
        if(!isTableOccupied[i]) return false;
    }
    return true;
}

bool areAllTablesOpen() {
    for(size_t i = 0; i < NUMBER_OF_TABLES; i++) {
        if(isTableOccupied[i]) return false;
    }
    return true;
}
