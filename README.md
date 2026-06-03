# computer-programming-file-handling-project
**Project Overview**

The Smart Parking System is a C++-based application integrated with an ESP32 microcontroller for secure parking record management. The system combines file handling, user authentication, and hardware-software communication to provide a simple parking management solution.

The application stores parking information in a CSV database file and communicates with the ESP32 through serial communication. Before accessing the system, the user must pass both hardware-based authentication and software-based login verification, ensuring secure operation.

**Objectives**
Manage parking records efficiently.
Store parking data in a CSV file.
Prevent duplicate parking spot entries.
Enable secure access using ESP32 authentication.
Allow searching and updating of parking records.
Demonstrate integration of C++ programming with embedded hardware.
Technologies Used
Software
C++
File Handling (CSV Database)
Windows API for Serial Communication
Hardware
ESP32 Development Board
Database
CSV File (parking_database (1).csv)
Features
1. ESP32 Connection Verification

The program first checks whether the ESP32 is connected through the serial port. If the device is not detected, the program terminates.

2. Access Key Authentication

An access key stored inside the ESP32 is requested by the application. The user must enter the correct key before proceeding.

3. User Login System

After hardware authentication, the user must provide a valid username and password to access the system.

4. Add Parking Records

Users can add new parking records including:

Spot ID
Vehicle Number
Vehicle Type
Owner Name
Parking Status (IN/OUT)
5. Duplicate ID Prevention

The system verifies that each parking spot ID is unique before saving a new record.

6. Search Records Manually

Users can search for parking records using the parking spot ID.

7. Search Records Using ESP32

The ESP32 can send a parking ID to the computer through serial communication, allowing automatic record searching.

8. Update Existing Records

Users can modify vehicle information and parking status for an existing parking record.

9. Display All Records

**All stored parking records can be viewed directly from the database file.**

System Workflow
Start Program
Check ESP32 Connection
Verify Access Key
Verify User Login
Load CSV Database
Display Main Menu
Perform Selected Operation:
Add Record
Search Record
Search Using ESP32
Update Record
Display All Records
Exit Program
Menu Options
Add Record
Search Record Manually
Search Record Using ESP32 Key
Update Record
Display All Records
Exit
Database Format

The system stores data in CSV format.

**Example:**

P001,ABC123,Car,Ahsan,IN

Where:

P001 = Parking Spot ID
ABC123 = Vehicle Number
Car = Vehicle Type
Ahsan = Owner Name
IN = Parking Status
Security Features
ESP32 Hardware Authentication
Access Key Verification
Username and Password Login
Duplicate Record Prevention
Learning Outcomes

**This project demonstrates:**

File Handling in C++
Structures and Functions
CSV Database Management
Serial Communication
ESP32 Integration
Authentication Systems
Hardware and Software Interfacing
Future Improvements
RFID-based vehicle identification
Real-time parking slot monitoring
Graphical User Interface (GUI)
Cloud database integration
Mobile application support
Automatic entry and exit logging
Conclusion

The Smart Parking System successfully integrates C++ programming with ESP32 hardware to create a secure and efficient parking management solution. The project demonstrates practical implementation of file handling, authentication mechanisms, and serial communication while providing a foundation for future smart parking applications.
