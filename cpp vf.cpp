#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cstring>

using namespace std;

// ================= STRUCT =================
struct Record {
    string spotID;
    string vehicleNumber;
    string vehicleType;
    string ownerName;
    string status;
};

// ================= INITIALIZE DATABASE =================
void initializeDatabase() {
    ifstream file("parking_database (1).csv");
    if (!file.good()) {
        cout << "Database file not found.\n";
    }
    file.close();
}

// ================= CHECK UNIQUE ID =================
bool isUnique(string id) {
    ifstream file("parking_database (1).csv");
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string currentID;
        getline(ss, currentID, ',');
        if (currentID == id) {
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

// ================= APPEND RECORD =================
void appendRecord(string data) {
    ofstream file("parking_database (1).csv", ios::app);
    file << data << endl;
    file.close();
    cout << "Record added successfully.\n";
}

// ================= SEARCH BY ID =================
void searchByID(string id) {
    ifstream file("parking_database (1).csv");
    string line;
    bool found = false;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string currentID;
        getline(ss, currentID, ',');
        if (currentID == id) {
            cout << "\n===== RECORD FOUND =====\n";
            cout << line << endl;
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "Record not found.\n";
    }
}

// ================= UPDATE RECORD =================
void updateRecord(string id, string newData) {
    ifstream file("parking_database (1).csv");
    ofstream tempFile("temp.csv");
    string line;
    bool found = false;
    getline(file, line);
    tempFile << line << endl;
    while (getline(file, line)) {
        stringstream ss(line);
        string currentID;
        getline(ss, currentID, ',');
        if (currentID == id) {
            tempFile << newData << endl;
            found = true;
        } else {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();
    remove("parking_database (1).csv");
    rename("temp.csv", "parking_database (1).csv");
    if (found)
        cout << "Record updated successfully.\n";
    else
        cout << "Record not found.\n";
}

// ================= DISPLAY ALL RECORDS =================
void displayAll() {
    ifstream file("parking_database (1).csv");
    string line;
    cout << "\n===== ALL RECORDS =====\n\n";
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

// ================= OPEN SERIAL PORT (SHARED HELPER) =================
HANDLE openSerialPort() {

    HANDLE hSerial = CreateFile(
        "COM4",
        GENERIC_READ | GENERIC_WRITE,
        0, 0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 2000;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 2000;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    return hSerial;
}

// ================= SEND COMMAND & READ RESPONSE (SHARED HELPER) =================
string sendCommand(HANDLE hSerial, const char* command, int waitMs = 1000) {

    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    DWORD bytesWritten;
    WriteFile(hSerial, command, strlen(command), &bytesWritten, NULL);

    Sleep(waitMs);

    char buffer[256] = {0};
    DWORD bytesRead;
    ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);

    string response(buffer, bytesRead);

    // -- FIXED: replaced back() and pop_back() with index-based trim --
    while (!response.empty() &&
           (response[response.size() - 1] == '\r' ||
            response[response.size() - 1] == '\n' ||
            response[response.size() - 1] == ' ')) {
        response.erase(response.size() - 1);
    }

    return response;
}

// ================= CHECK ESP32 CONNECTION =================
bool checkESP32() {

    HANDLE hSerial = openSerialPort();

    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;
    }

    Sleep(3000);

    string response = sendCommand(hSerial, "CHECK_ID\n");

    CloseHandle(hSerial);

    return (response == "CONNECTED");
}

// ================= GET ACCESS KEY FROM ESP32 =================
string getESP32AccessKey() {

    HANDLE hSerial = openSerialPort();

    if (hSerial == INVALID_HANDLE_VALUE) {
        return "";
    }

    Sleep(3000);

    string key = sendCommand(hSerial, "GET_ACCESS\n");

    CloseHandle(hSerial);

    return key;
}

// ================= GET PARKING KEY FROM ESP32 =================
string getESP32Key() {

    HANDLE hSerial = openSerialPort();

    if (hSerial == INVALID_HANDLE_VALUE) {
        return "";
    }

    Sleep(3000);

    string key = sendCommand(hSerial, "SEND_KEY\n");

    CloseHandle(hSerial);

    return key;
}

// ================= ACCESS KEY VERIFICATION =================
bool verifyAccessKey() {

    cout << "\n===== ACCESS KEY VERIFICATION =====\n";
    cout << "Fetching access key from ESP32...\n";

    string esp32Key = getESP32AccessKey();

    if (esp32Key.empty()) {
        cout << "Failed to retrieve access key from ESP32.\n";
        cout << "Program Cannot Execute.\n";
        return false;
    }

    cout << "Enter Access Key: ";

    string userKey;
    getline(cin, userKey);

    // -- FIXED: replaced back() and pop_back() with index-based trim --
    while (!userKey.empty() &&
           (userKey[userKey.size() - 1] == ' '  ||
            userKey[userKey.size() - 1] == '\r' ||
            userKey[userKey.size() - 1] == '\n')) {
        userKey.erase(userKey.size() - 1);
    }

    if (userKey == esp32Key) {
        cout << "\nAccess Key Verified Successfully.\n";
        return true;
    } else {
        cout << "\nInvalid Access Key. Authorization Denied.\n";
        cout << "Program Cannot Execute.\n";
        return false;
    }
}

// ================= LOGIN SYSTEM =================
bool loginSystem() {

    string username;
    string password;

    cout << "\n===== LOGIN SYSTEM =====\n";

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);

    if (username == "Ahsan" && password == "2501522") {
        cout << "\nLogin Successful.\n";
        return true;
    } else {
        cout << "\nInvalid Username or Password.\n";
        return false;
    }
}

// ================= MAIN FUNCTION =================
int main() {

    // -- STEP 1 : Hardware Handshake --------------------------------------
    cout << "Checking ESP32 Connection...\n";

    if (!checkESP32()) {
        cout << "ESP32 NOT Connected.\n";
        cout << "Program Cannot Execute.\n";
        system("pause");
        return 0;
    }

    cout << "ESP32 Connected Successfully.\n";

    // -- STEP 2 : Access Key Must Match ESP32 Stored Key ------------------
    if (!verifyAccessKey()) {
        system("pause");
        return 0;
    }

    // -- STEP 3 : Operator Login -------------------------------------------
    if (!loginSystem()) {
        system("pause");
        return 0;
    }

    // -- STEP 4 : Main Application -----------------------------------------
    initializeDatabase();

    int choice;

    do {

        cout << "\n===== SMART PARKING SYSTEM =====\n";
        cout << "1. Add Record\n";
        cout << "2. Search Record Manually\n";
        //cout << "3. Search Record Using ESP32 Key\n";
        cout << "3. Update Record\n";
        cout << "4. Display All Records\n";
        cout << "5. Exit\n";
        cout << "Enter Choice: ";

        cin >> choice;
        cin.ignore();

        // ADD RECORD
        if (choice == 1) {

            string id, vehicle, type, owner, status;

            cout << "Enter Spot ID (P001): ";
            getline(cin, id);

            if (!isUnique(id)) {
                cout << "Duplicate ID Not Allowed.\n";
                continue;
            }

            cout << "Enter Vehicle Number: ";
            getline(cin, vehicle);

            cout << "Enter Vehicle Type: ";
            getline(cin, type);

            cout << "Enter Owner Name: ";
            getline(cin, owner);

            cout << "Enter Status (IN/OUT): ";
            getline(cin, status);

            appendRecord(id + "," + vehicle + "," +
                         type + "," + owner + "," + status);
        }

        // SEARCH MANUAL
        else if (choice == 2) {

            string id;
            cout << "Enter Spot ID: ";
            getline(cin, id);
            searchByID(id);
        }

        // SEARCH USING ESP32
        else if (choice == 3) {

            string receivedID = getESP32Key();

            if (receivedID.empty()) {
                cout << "Failed To Receive ID From ESP32.\n";
            } else {
                cout << "\nID Received From ESP32: " << receivedID << endl;
                searchByID(receivedID);
            }
        }

        // UPDATE RECORD
        else if (choice == 4) {

            string id, vehicle, type, owner, status;

            cout << "Enter Existing Spot ID: ";
            getline(cin, id);

            cout << "Enter New Vehicle Number: ";
            getline(cin, vehicle);

            cout << "Enter New Vehicle Type: ";
            getline(cin, type);

            cout << "Enter New Owner Name: ";
            getline(cin, owner);

            cout << "Enter New Status (IN/OUT): ";
            getline(cin, status);

            updateRecord(id, id + "," + vehicle + "," +
                            type + "," + owner + "," + status);
        }

        // DISPLAY
        else if (choice == 5) {
            displayAll();
        }

        else if (choice != 6) {
            cout << "Invalid Choice.\n";
        }

    } while (choice != 6);

    cout << "\nProgram Terminated Successfully.\n";

    return 0;
}
