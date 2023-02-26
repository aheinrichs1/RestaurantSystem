/*
 * Program: RestaurantSystem
 * Authors: Alex Heinrichs, Laura Thornburg, Suzette Senephansiri
 *
 * This program contains a restaurant system containing the functionality
 * to seat, unseat, and edit the status tables or bar seats,
 * add and cancel reservations, and add to a waitlist
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime> //used for reservations
#include <string>
#include <stdlib.h>
using namespace std;

//Structure for a Reservation
struct Reservation {
    string name;
    string phone;
    int sizeOfParty;
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

//function declarations:
void displayMenu();
void displayTables();
void displayDate(Reservation reservation);
void displayTime(Reservation reservation);
void validateMenuSelection(int &input);
void validateTableSelection(int &input);
void validateBoolSelection(int &input);
bool areAllTablesClosed();
bool areAllTablesOpen();
bool areAllBarSeatsClosed();
bool areAllBarSeatsOpen();
tm *getTimeNow();
vector<Reservation> createSampleReservations();
void addToWaitList();
void print();
void clearScreen();

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

//Reservations
vector<Reservation> reservations;

//Waitlist
vector<string> waitlist;
string waitListCustomerName;
int waitListPartySize;
string convertedPartySize;

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

    //set Reservations to an empty list
    reservations = vector<Reservation>();

    //TESTING/DEMONSTRATION
    reservations = createSampleReservations();
    //cout << "Size of reservations : " << reservations.size() << endl << endl;

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
            clearScreen();
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
                    cout << endl << "Bar selected. Seats open: " << numBarSeatsOpen << endl <<
                            "How many are sitting? ";

                    //Initialize temp variable for determining amount sitting
                    int numBarSeatInput;
                    cin >> numBarSeatInput; //input variable
                    //Validate input

                    while(numBarSeatInput <= 0) {
                        cout << "Invalid input" << endl;
                        cout << "Bar selected. How many are sitting? ";
                        cin >> numBarSeatInput;
                    }
                    //Check if too many people are trying to sit
                    if(numBarSeatInput > numBarSeatsOpen) {
                        //Clear Screen
                        clearScreen();

                        cout << "Sorry, there are not enough seats available at the bar." << endl << endl;

                    //Else Change unoccupied bar seats to occupied
                    } else {
                        //set userInput to numBarSeatInput for printing
                        userInput = numBarSeatInput;
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
                        //Clear Screen
                        clearScreen();
                        cout << "Party of " << userInput << " seated at bar." << endl <<
                                "------------------------------" << endl << endl;
                    }
                }
            } else {

                //Check that table selected is open
                while(isTableOccupied[tableSelection - 1]) {
                    cout << endl << "Sorry, that table is occupied. Choose another table: ";
                    cin >> tableSelection;
                    //Validate input
                    validateTableSelection(tableSelection);
                }
                //Clear screen
                clearScreen();
                //Set table selected to occupied and notify user
                isTableOccupied[tableSelection - 1] = true;
                cout<< "Table number " << tableSelection << " has been set to occupied." << endl <<
                        "--------------------------------------" << endl << endl;
            }
        }

        //Edit table
        else if(userInput == 2) {
            //Clear screen
            clearScreen();

            cout << "EDIT TABLE:" << endl <<
                    "-------------------" << endl;

            //display tables
            displayTables();
            cout << "-----------------------------------" << endl <<
                    "Which table would you like to edit? ";
            cin >> tableSelection;
            //Validate input
            validateTableSelection(tableSelection);

            //Check for bar seating input
            if(tableSelection == (NUMBER_OF_TABLES + 1)) {
                cout << endl << "Editing bar seats" << endl;
                cout << "Seats open: " << numBarSeatsOpen << endl <<
                        "----------------------" << endl;
                cout << "How many seats would you like to set open? ";
                cin >> userInput;
                //Validate

                while((userInput < 0) || (userInput > NUMBER_OF_BAR_SEATS)) {
                    cout << endl << "Invalid number of seats, enter between 0 and " << NUMBER_OF_BAR_SEATS << endl <<
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
                //Clear Screen
                clearScreen();
                cout << "Bar seats set to " << userInput << " open." << endl << endl;

            //Else edit selected table
            } else {
                cout << endl << "Editing table " << tableSelection << endl;
                cout << "Enter 1 to set table to occupied, Enter 0 to set table to open: ";
                cin >> userInput;
                //Validate input
                validateBoolSelection(userInput);

                //Clear screen
                clearScreen();

                //Set table
                isTableOccupied[tableSelection - 1] = userInput;
                cout << "Table " << tableSelection << " has been set to " <<
                        (isTableOccupied[tableSelection - 1] ? "Occupied" : "Open") << endl <<
                        "--------------------------------------" << endl;
            }
        }

        //Reset table or bar seating
        else if(userInput == 3) {
            //Clear Screen
            clearScreen();

            //check if tables and bar seats are all open
            if (areAllTablesOpen() && areAllBarSeatsOpen()) {
                cout << "All tables and bar seats are open!" << endl << endl;
                cout << "Returning to main menu" << endl <<
                        "-----------------------" << endl;
            } else {
                cout << "RESET TABLE OR BAR:" << endl <<
                        "------------------------" << endl;

                //display tables
                displayTables();

                cout << endl << "Which table would you like to clear? ";
                cin >> tableSelection;
                //Validate input
                validateTableSelection(tableSelection);

                //Check if bar table is asked to be cleared
                if(tableSelection == (NUMBER_OF_TABLES + 1)) {
                    cout << endl << "Are you sure you would like to clear the bar seating to open?" << endl <<
                            "1 for yes, 0 for cancel: ";
                    cin >> userInput;
                    //Validate bool
                    validateBoolSelection(userInput);

                    //Clear screen
                    clearScreen();

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
                        cout << endl << "Sorry, that table is already open. Choose another table: ";
                        cin >> tableSelection;
                        //Validate input
                        validateTableSelection(tableSelection);
                    }
                    //Clear screen
                    clearScreen();

                    cout << "Clearing table " << tableSelection << endl;
                    isTableOccupied[tableSelection - 1] = 0;
                    cout << "--------------------------------------" << endl << endl;
                }
            }
        }

        //Check all table and bar seat status
        else if(userInput == 4) {

            //Clear Screen
            clearScreen();

            displayTables();
            cout << "-------------" << endl;
        }

        //View reservations for the day
        else if(userInput == 5) {
            //Clear screen
            clearScreen();

            //Check if there are any reservations
            if(reservations.empty()) {
                cout << "There are no reservations" << endl <<
                        "-------------------------" << endl << endl;
            } else { //if there are reservations, continue with viewing options
                cout << "VIEW RESERVATIONS" << endl <<
                        "-----------------" << endl;

                //get the current time
                tm *now = getTimeNow();
                cout << endl << "Would you like to check the reservations for today or all reservations?" << endl <<
                        "Enter 1 to check for today or 0 for all: ";
                cin >> userInput;
                //Validate input
                validateBoolSelection(userInput);
                if(userInput == 1) {
                    //Define a temp vector list of type Reservation for listing today's reservations
                    vector<Reservation> reservationsToday;

                    //Go through list and select all reservations with same date as today
                    for(size_t i = 0; i < reservations.size(); i++) {
                        //Ask if reservations[i] is on same day as today
                        //Note: tm_yday displays years since 1900, so 1900 must be added for accurate year
                        //Also: tm_mon displays months from 0-11, so 1 must be added
                        if(((now->tm_year + 1900) == reservations[i].year) &&
                                ((now->tm_mon + 1) == reservations[i].month) &&
                                (now->tm_mday == reservations[i].day)) {
                            //if true, add reservations[i] to reservationsToday list
                            reservationsToday.push_back(reservations[i]);
                        }
                    }

                    //Check if there are no reservations today
                    if(reservationsToday.size() == 0) {
                        //Clear screen
                        clearScreen();
                        cout << "There are no reservations today" << endl <<
                                "-------------------------------" << endl << endl;
                    } else {
                        //Clear screen
                        clearScreen();
                        //Display today's reservations
                        cout << "TODAYS RESERVATIONS" << endl <<
                                "----------------" << endl;
                        for(size_t i = 0; i < reservationsToday.size(); i++) {
                            cout << "Reservation " << (i+1) << ": ";
                            displayTime(reservationsToday[i]);
                            cout << "  Name: " << reservationsToday[i].name <<
                                    " | Party size: " << reservationsToday[i].sizeOfParty << endl;
                        }
                        cout << "--------------------------" << endl;
                    }

                } else { //Listing all reservations
                    //Clear screen
                    clearScreen();
                    cout << "ALL RESERVATIONS" << endl <<
                            "----------------" << endl;
                    for(size_t i = 0; i < reservations.size(); i++) {
                        cout << "Reservation " << (i+1) << ": ";
                        displayDate(reservations[i]);
                        cout << " at ";
                        displayTime(reservations[i]);
                        cout << "  Name: " << reservations[i].name <<
                                " | Number: " << reservations[i].phone <<
                                " | Party size: " << reservations[i].sizeOfParty << endl;
                    }
                    cout << endl <<
                            "---------------------------" << endl;
                }
            }
        }

        //Make reservation
        else if(userInput == 6) {
            //Clear screen
            clearScreen();
            cout << "MAKE RESERVATION:" << endl <<
                    "-----------------" << endl;
            //Structure to hold new reservation
            Reservation newReservation;
            //Initialize input variables;
            string partyName;
            string partyPhone;
            int partySize;
            int resYear = (getTimeNow()->tm_year + 1900);
            int resMonth;
            int resDay;
            int resHour;
            string amPm;
            int resMinute;
            cout << "Please enter details:" << endl;
            cout << "Name: ";
            cin >> partyName;
            cout << "Phone number: ";
            cin >> partyPhone;
            cout << "Party size: ";
            cin >> partySize;

            cout << "Month: ";
            cin >> resMonth;
            //Validate month
            while((resMonth < 1) || (resMonth > 12)) {
                cout << "Invalid month, enter from 1-12: ";
                cin >> resMonth;
            }

            cout << "Day: ";
            cin >> resDay;
            //Validate day
            while((resDay < 1) || (resDay > 31)) {
                cout << "Invalid day, enter from 1-31: ";
                cin >> resDay;
            }

            cout << "Hour: ";
            cin >> resHour;
            //Validate hour
            while((resHour < 1) || (resHour > 12)) {
                cout << "Invalid hour, enter from 1-12: ";
                cin >> resHour;
            }

            cout << "Minute: ";
            cin >> resMinute;
            //Validate minute
            while((resMinute < 0) || (resMinute > 59)) {
                cout << "Invalid minute, enter from 0-59: ";
                cin >> resMinute;
            }

            cout << "am or pm? ";
            cin >> amPm;
            //Validate
            while((amPm != "am") && (amPm != "pm")) {
                cout << "Invalid entry, enter am or pm: ";
                cin >> amPm;
            }
            //Check if time is in am or pm
            //Add 12 to resHour is in pm
            if(amPm == "pm") {
                resHour += 12;
            }

            //Input data into newReservation
            newReservation.name = partyName;
            newReservation.phone = partyPhone;
            newReservation.sizeOfParty = partySize;
            newReservation.year = resYear;
            newReservation.month = resMonth;
            newReservation.day = resDay;
            newReservation.hour = resHour;
            newReservation.minute = resMinute;

            //Clear screen
            clearScreen();

            //Display reservation data before asking to save it
            cout << "The reservation you entered:" << endl;
            cout << "Name: " << newReservation.name << " | Phone: " << newReservation.phone << " | Party Size: " << newReservation.sizeOfParty << endl;
            cout << "Date: ";
            displayDate(newReservation);
            cout << " | Time: ";
            displayTime(newReservation);
            cout << endl << "----------------------------" << endl;
            cout << "Is this correct? Enter 1 for yes or press 0 to cancel: ";
            cin >> userInput;
            validateBoolSelection(userInput);

            //Clear screen
            clearScreen();

            //If yes then add to reservations
            if(userInput == 1) {
                cout << "Adding reservation to reservations." << endl <<
                        "-----------------------------------" << endl << endl;
                reservations.push_back(newReservation);
            //If no then print message and do nothing
            } else {
                cout << "Reservation cancelled, returning to the main menu." << endl <<
                        "--------------------------------------------------" << endl << endl;
            }

        }

        //Cancel/remove reservation
        else if(userInput == 7) {
            //Clear screen
            clearScreen();
            //Declare temp variable for index of reservation to be deleted
            int reservationIndex;
            //Check if there are any reservations
            if(reservations.empty()) {
                cout << "There are no reservations to cancel" << endl <<
                        "-------------------------" << endl << endl;
            //Else list reservations and ask which one to be deleted
            } else {
                cout << "CANCEL A RESERVATION" << endl <<
                        "--------------------" << endl;
                cout << endl << "ALL RESERVATIONS" << endl <<
                        "----------------" << endl;
                for(size_t i = 0; i < reservations.size(); i++) {
                    cout << "Reservation " << (i+1) << ": ";
                    displayDate(reservations[i]);
                    cout << " at ";
                    displayTime(reservations[i]);
                    cout << "  Name: " << reservations[i].name <<
                            " | Number: " << reservations[i].phone <<
                            " | Party size: " << reservations[i].sizeOfParty << endl;
                }
                cout << endl << "--------------------------" << endl << endl;
                cout << "Which reservation would you like to cancel? ";
                cin >> reservationIndex;
                while ((reservationIndex < 1) || (reservationIndex > static_cast<int>(reservations.size()))) {
                    cout << "Invalid input, enter a reservation from 1-" << reservations.size() << ": ";
                    cin >> reservationIndex;
                }

                cout << endl <<
                        "----------------------------" << endl <<
                        "You selected reservation " << reservationIndex << ":" << endl;
                cout << "Name: " << reservations[reservationIndex-1].name << " | Phone: " << reservations[reservationIndex-1].phone << " | Party Size: " << reservations[reservationIndex-1].sizeOfParty << endl;
                cout << "Date: ";
                displayDate(reservations[reservationIndex-1]);
                cout << " | Time: ";
                displayTime(reservations[reservationIndex-1]);
                cout << endl << "----------------------------" << endl;
                cout << "Are you sure you want to cancel this reservation? Enter 1 for yes or 0 for no: ";
                cin >> userInput;
                validateBoolSelection(userInput);

                //Clear screen
                clearScreen();

                //If yes, then delete reservation at index userInput-1
                if(userInput == 1) {
                    cout << "Reservation has been cancelled. Returning to the main menu" << endl;
                    reservations.erase(reservations.begin() + (reservationIndex - 1));
                    cout << "----------------------" << endl << endl;
                } else {
                    cout << "Reservation not cancelled. Returning to the main menu" << endl;
                    cout << "----------------------" << endl << endl;
                }
            }
        }

        //Add customer to wait list for table
        else if(userInput == 8) {
            //Clear screen
            clearScreen();
            cout << "WAITLIST" << endl <<
                    "--------" << endl;
            addToWaitList();
            print();
            cout << endl << "---------------------" << endl << endl;
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
            "8. Add Customer to Wait list" << endl <<
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

void displayDate(Reservation reservation) {
    cout << reservation.month << "/" << reservation.day << "/" << reservation.year;
}

void displayTime(Reservation reservation) {
    cout << setfill('0');
    cout << ((reservation.hour > 12) ? (reservation.hour - 12) : (reservation.hour)) << ":" <<
            ((reservation.minute == 0) ? (setw(2)) : ((reservation.minute < 10) ? (setw(1)) : (setw(0)))) <<
            reservation.minute << " " <<
            ((reservation.hour > 11) ? ("PM") : ("AM"));
}

void validateMenuSelection(int &input) {
    while((input < 1) || (input > 9)) {
          cout << "Invalid input" << endl <<
                  "Choose an option: ";
          cin >> input;
    }
}

void validateTableSelection(int &input) {
    while((input < 1) || (input > (NUMBER_OF_TABLES + 1))) {
        cout << "Invalid selection" << endl;
        cout << "Choose a table between 1 and " << NUMBER_OF_TABLES << " or choose 20 to select bar: ";
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

tm *getTimeNow() {
    //get time
    time_t now = time(NULL);
    //turn into a tm structure for reading contents easily
    tm *ltm = localtime(&now);
    return ltm;
}

void addToWaitList() {
    //Enter Customer Name for list
    cout << "Enter the name of the customer for waitlist: ";
    cin >> waitListCustomerName;
    cout << "Enter the number in the party: ";
    cin >> waitListPartySize;
    //Clear screen
    clearScreen();
    if(waitListPartySize > 0 && waitListPartySize < 9){
        convertedPartySize = to_string(waitListPartySize);
        waitlist.push_back(waitListCustomerName + " " + convertedPartySize);
        cout << "Party of " + convertedPartySize + " for " + waitListCustomerName + " has been added to wait list." << endl <<
                "------------------------------------------------" << endl; }
    else {
        cout << "Invalid Party size.  Enter a value between 1 - 8" << endl <<
                "------------------------------------------" << endl;
    }
}

void print() {
    cout << "Customers currently on the wait list are: " << endl;
    for(size_t i=0; i < waitlist.size(); i++)
        cout << waitlist.at(i) << ' ' << endl;
}

void clearScreen() {
    cout << flush;
    system("CLS");
}

//USED FOR TESTING:
vector<Reservation> createSampleReservations() {
    vector<Reservation> sampleList;
    Reservation a;
    a.name = "Alex Heinrichs";
    a.phone = "123-456-7899";
    a.sizeOfParty = 2;
    a.year = 2023;
    a.month = 2;
    a.day = 26;
    a.hour = 18;
    a.minute = 30;
    Reservation b;
    b.name = "Suzette Senephansiri";
    b.phone = "999-999-9999";
    b.sizeOfParty = 1;
    b.year = 2023;
    b.month = 2;
    b.day = 26;
    b.hour = 19;
    b.minute = 0;
    Reservation c;
    c.name = "Laura Thornburg";
    c.phone = "555-555-5555";
    c.sizeOfParty = 4;
    c.year = 2023;
    c.month = 2;
    c.day = 26;
    c.hour = 17;
    c.minute = 30;
    Reservation d;
    d.name = "Joe Struss";
    d.phone = "111-222-4455";
    d.sizeOfParty = 7;
    d.year = 2023;
    d.month = 2;
    d.day = 27;
    d.hour = 18;
    d.minute = 30;
    sampleList.push_back(a);
    sampleList.push_back(b);
    sampleList.push_back(c);
    sampleList.push_back(d);
    return sampleList;
}
