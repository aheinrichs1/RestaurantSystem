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
bool areAllBarSeatsClosed();
bool areAllBarSeatsOpen();

//Declare Program Variables
const int NUMBER_OF_BAR_SEATS = 10;
const int NUMBER_OF_TABLES = 19;

//Based on the plain restaurant layout created by Suzette
//6 Tables in main area, 7 Booths, and 5 Tables in party room, and 10 seats at the bar

//Bar seating
vector<bool> isBarSeatOccupied;
int numBarSeatsOpen;

//Table seating
vector<bool> isTableOccupied;

int main() {

    //variables for main:
    int userInput; //int used for user to select and navigate menus
    int tableSelection; //used to select tables for adding, editing, or deleting status
    bool endDay = false; //bool used to end user input loop

    //Start day:

    //Reset tables (maybe find a way to save table status at a later point)
    isTableOccupied = vector<bool>(NUMBER_OF_TABLES, false);

    //Reset bar seats
    isBarSeatOccupied = vector<bool>(NUMBER_OF_BAR_SEATS, false);
    numBarSeatsOpen = NUMBER_OF_BAR_SEATS;

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

            //Check if bar seating was selected
            if(tableSelection == (NUMBER_OF_TABLES + 1)) {

                //BAR SEATING

                //Check if any bar seats are open
                if(numBarSeatsOpen == 0) {
                    cout << endl << "Sorry, there are no bar seats open." << endl << endl;
                } else {
                    //Determine number of people to sit at bar
                    cout << "Bar selected. Seats open: " << numBarSeatsOpen << endl <<
                            "How many are sitting? ";
                    //Initialize temp variable for determining amount sitting
                    int numBarSeatInput;
                    cin >> numBarSeatInput;
                    //Validate input
                    while(numBarSeatInput <= 0) {
                        cout << "Invalid input" << endl;
                        cout << "Bar selected. How many are sitting? ";
                        cin >> numBarSeatInput;
                    }
                    //Check if too many people are trying to sit
                    if(numBarSeatInput > numBarSeatsOpen) {
                        cout << endl << "Sorry, there are not enough seats available at the bar." << endl << endl;
                    } else {
                        //Change unoccupied bar seats to occupied
                        for(int i = 0; i < NUMBER_OF_BAR_SEATS; i++) {
                            //iterate i to next unoccupied seat
                            while(isBarSeatOccupied[i]) {
                                i++;
                            }
                            if(numBarSeatInput > 0) {
                                isBarSeatOccupied[i] = true;
                                --numBarSeatInput;
                                //Subtract total bar seats open
                                --numBarSeatsOpen;
                            }
                        }
                        cout << endl << "Party Seated at bar." << endl << endl;
                    }
                }
            } else {

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
        }

        //Edit table
        else if(userInput == 2) {
            //display tables
            displayTables();
            cout << "Which table would you like to edit? ";
            cin >> tableSelection;
            //Validate input
            validateTableSelection(tableSelection);
            //Check for bar seating input
            if(tableSelection == (NUMBER_OF_TABLES + 1)) {
                cout << endl << "Editing bar seats" << endl;
                cout << "Seats open: " << numBarSeatsOpen << endl;
                cout << "How many seats would you like to set open? ";
                cin >> userInput;
                //Validate
                while((userInput < 0) || (userInput > NUMBER_OF_BAR_SEATS)) {
                    cout << "Invalid number of seats, enter between 0 and " << NUMBER_OF_BAR_SEATS << endl <<
                            "how many seat you would like to set open: ";
                    cin >> userInput;
                }
                //Reset isBarSeatOccupied to userInput number of bar seats open
                //Resetting isBarSeatOccupied
                isBarSeatOccupied = vector<bool>(NUMBER_OF_BAR_SEATS, false);
                numBarSeatsOpen = NUMBER_OF_BAR_SEATS;
                //iterating through isBarSeatOccupied (NUMBER_OF_BAR_SEATS - userInput) number of times
                for(int i = 0; i < (NUMBER_OF_BAR_SEATS - userInput); i++) {
                    isBarSeatOccupied[i] = true;
                    --numBarSeatsOpen;
                }
                cout << endl << "Bar seats set to " << userInput << " open." << endl << endl;
            } else {
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
        }

        //Reset table or bar seating
        else if(userInput == 3) {
            //display tables
            displayTables();
            if (areAllTablesOpen() && areAllBarSeatsOpen()) {
                cout << endl << "All tables and bar seats are open!" << endl;
                cout << "Returning to main menu" << endl << endl;
            } else {
                cout << endl << "Which table would you like to clear? Select 20 for bar seating ";
                cin >> tableSelection;
                //Validate input
                validateTableSelection(tableSelection);
                //Check if bar table is asked to be cleared
                if(tableSelection == (NUMBER_OF_TABLES + 1)) {
                    cout << "Are you sure you would like to clear the bar seating to open?" << endl <<
                            "1 for yes, 0 for cancel: ";
                    cin >> userInput;
                    //Validate bool
                    validateBoolSelection(userInput);
                    if(userInput == 1) {
                        //Reset bar seating to open
                        isBarSeatOccupied = vector<bool>(NUMBER_OF_BAR_SEATS, false);
                        numBarSeatsOpen = NUMBER_OF_BAR_SEATS;
                        cout << "Bar seating has been reset to " << NUMBER_OF_BAR_SEATS << endl <<
                                "Returning to the main menu." << endl << endl;
                    } else {
                        cout << "Cancelled, returning to the main menu." << endl << endl;
                    }
                } else {
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
    //Display bar seats
    cout << endl << (NUMBER_OF_TABLES + 1) << ": BAR" << endl <<
            "-------" << endl <<
            "Seats Open: " << numBarSeatsOpen << endl;
}

void validateMenuSelection(int &input) {
    while((input < 1) || (input > 9) || (input == 8)) {
          cout << "Invalid input" << endl <<
                  "Choose an option: ";
          cin >> input;
    }
}

void validateTableSelection(int &input) {
    while((input < 1) || (input > (NUMBER_OF_TABLES + 1))) {
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

bool areAllBarSeatsClosed() {
    if(numBarSeatsOpen == 0) return true;
    return false;
}

bool areAllBarSeatsOpen() {
    if(numBarSeatsOpen == NUMBER_OF_BAR_SEATS) return true;
    return false;
}
