// WellKillerSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>

#include "Well.h"

using namespace std;


// ============================================================
// WELL KILLER
// LoRa Irrigation Well Control System
//
// This program simulates the software side of my Well Killer
// project.
//
// The user/farmer can:
// 1. View irrigation well status.
// 2. Shut down one well.
// 3. Shut down all wells. (useful when shutting down everything in a bad storm)
// 4. View the shutdown log. 
// 5. Test the simulated LoRa connection.
// 6. Exit.
// ============================================================

//Multi-Threading, Templates, Iterators, Recursion, and Smart Pointers

class Device 
{
protected:

    // The device name can be used by the derived classes.
    string deviceName;

public: 

    // Constructor
    Device(string name)
    {
        deviceName = name;
    }

    virtual void communicate() = 0; 

    // Virtual destructor.
    virtual ~Device()
    {
    }
};


class LoRaTransmitter : public Device
{
public:

    // Call the Device constructor and give this device a name.
    LoRaTransmitter()
        : Device("Home LoRa Transmitter")
    {
    }

    // This overrides the communicate() function from Device.
  
    void communicate() override
    {
        cout << "\n[" << deviceName << "]" << endl;
        cout << "Sending test signal..." << endl;
        cout << "Signal transmitted successfully." << endl;
    }

    // Function used when sending a well shutdown command.
    void sendShutdownCommand(int wellID)
    {
        cout << "\nLoRa Transmitter:" << endl;
        cout << "Sending shutdown command to Well "
            << wellID << "..." << endl;
    }
};


class LoRaReceiver : public Device
{
public:

    LoRaReceiver()
        : Device("Well LoRa Receiver")
    {
    }

    // Another override version of communicate().
    void communicate() override
    {
        cout << "\n[" << deviceName << "]" << endl;
        cout << "Listening for LoRa signal..." << endl;
        cout << "Test signal received successfully." << endl;
    }

    // Simulate receiving a shutdown command.
    void receiveShutdownCommand(int wellID)
    {
        cout << "LoRa Receiver:" << endl;
        cout << "Shutdown command received for Well "
            << wellID << "." << endl;
    }
};


class WellController
{
private:

    // A vector stores all irrigation wells.
    vector<Well> wells;

    // Simulated LoRa hardware.
    LoRaTransmitter transmitter;

    LoRaReceiver receiver;

    // File where shutdown events will be stored.
    string logFileName;


    void logAction(string message)
    {
        ofstream outputFile(logFileName, ios::app);

        // Check that the file opened correctly.
        if (!outputFile)
        {
            throw runtime_error(
                "The Well Killer log file could not be opened."
            );
        }

        outputFile << message << endl;

        outputFile.close();
    }


public:


    WellController()
    {
        logFileName = "WellKillerLog.txt";

        // Add seven wells to the vector.
        wells.push_back(Well(1, "North Well"));
        wells.push_back(Well(2, "South Well"));
        wells.push_back(Well(3, "East Well"));
        wells.push_back(Well(4, "West Well"));
        wells.push_back(Well(5, "Center Well"));
        wells.push_back(Well(6, "Field Well"));
        wells.push_back(Well(7, "Pivot Well"));
    }



    void displayWells()
    {
        cout << "\n==================================" << endl;
        cout << "          WELL STATUS" << endl;
        cout << "==================================" << endl;

        // Use a simple indexed loop.
        for (int i = 0; i < wells.size(); i++)
        {
            wells[i].displayStatus();
        }

        cout << "==================================" << endl;
    }


  

    void shutdownWell(int wellID)
    {
        // Make sure the ID is valid.
        if (wellID < 1 || wellID > wells.size())
        {
            throw out_of_range(
                "That well number does not exist."
            );
        }

        // Vector positions start at zero.
        Well& selectedWell = wells[wellID - 1];

        // Check if the well has already been shut down.
        if (!selectedWell.isRunning())
        {
            cout << "\nWell " << wellID
                << " is already stopped." << endl;

            return;
        }

        cout << "\n==================================" << endl;
        cout << "       SHUTDOWN SEQUENCE" << endl;
        cout << "==================================" << endl;

        // Simulate the actual LoRa command.
        transmitter.sendShutdownCommand(wellID);

        cout << "LoRa signal traveling to well..." << endl;

        receiver.receiveShutdownCommand(wellID);

        cout << "Murphy shutdown circuit triggered." << endl;

        // Change the well's running status.
        selectedWell.shutdown();

        cout << endl;
        cout << selectedWell.getName()
            << " HAS BEEN SHUT DOWN." << endl;

        cout << "==================================" << endl;

        // Save the event to the log file.
        logAction(
            "Well " +
            to_string(wellID) +
            " - " +
            selectedWell.getName() +
            " - SHUTDOWN"
        );
    }


 

    void shutdownWell(string wellName)
    {
        for (int i = 0; i < wells.size(); i++)
        {
            if (wells[i].getName() == wellName)
            {
                shutdownWell(wells[i].getID());
                return;
            }
        }

        // If the program reaches this point, the name was
        // not found.
        throw invalid_argument(
            "A well with that name could not be found."
        );
    }



    void shutdownAllWells()
    {
        cout << "\n==================================" << endl;
        cout << "     EMERGENCY ALL-WELL STOP" << endl;
        cout << "==================================" << endl;

        for (int i = 0; i < wells.size(); i++)
        {
            // Only shut down wells that are still running.
            if (wells[i].isRunning())
            {
                shutdownWell(wells[i].getID());
            }
        }

        cout << "\nAll wells are now stopped." << endl;
    }


 

    void testLoRa()
    {
        cout << "\n==================================" << endl;
        cout << "       LORA CONNECTION TEST" << endl;
        cout << "==================================" << endl;

        // Device pointers point to derived objects.
        //
        // Calling communicate() causes the appropriate
        // overridden function to execute.
        Device* device1 = &transmitter;
        Device* device2 = &receiver;

        device1->communicate();

        cout << endl;
        cout << "Wireless LoRa transmission..." << endl;

        device2->communicate();

        cout << "\nLoRa communication test PASSED."
            << endl;
    }


 

    void viewLog()
    {
        ifstream inputFile(logFileName);

        cout << "\n==================================" << endl;
        cout << "        WELL KILLER LOG" << endl;
        cout << "==================================" << endl;

        // The log may not exist yet if nothing has been shut
        // down.
        if (!inputFile)
        {
            cout << "No shutdown events have been logged yet."
                << endl;

            return;
        }

        string line;

        while (getline(inputFile, line))
        {
            cout << line << endl;
        }

        inputFile.close();

        cout << "==================================" << endl;
    }
};



void displayOpeningScreen()
{
    cout << "==================================================" << endl;
    cout << "                  WELL KILLER" << endl;
    cout << "          LoRa Irrigation Safety System" << endl;
    cout << "==================================================" << endl;

    cout << endl;

    cout << "The Well Killer is designed to allow a farmer" << endl;
    cout << "to monitor and remotely shut down irrigation" << endl;
    cout << "wells using long-range LoRa communication." << endl;

    cout << endl;

    cout << "This software simulates the operation of the" << endl;
    cout << "home transmitter, LoRa communication system," << endl;
    cout << "well receiver, and Murphy shutdown circuit." << endl;

    cout << endl;

    cout << "Use the menu to monitor or shut down wells." << endl;

    cout << "==================================================" << endl;
}




void displayMenu()
{
    cout << "\n";
    cout << "============== MAIN MENU ==============" << endl;

    cout << "1. Display Well Status" << endl;
    cout << "2. Shut Down One Well" << endl;
    cout << "3. Shut Down All Wells" << endl;
    cout << "4. View Shutdown Log" << endl;
    cout << "5. Test LoRa Communication" << endl;
    cout << "6. Exit" << endl;

    cout << "=======================================" << endl;

    cout << "Enter your choice: ";
}



int getMenuChoice()
{
    int choice;

    cin >> choice;

    // Check if the user entered something other than a number.
    if (cin.fail())
    {
        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        throw invalid_argument(
            "Please enter a number from 1 through 6."
        );
    }

    // Check the number range.
    if (choice < 1 || choice > 6)
    {
        throw out_of_range(
            "Menu choice must be between 1 and 6."
        );
    }

    return choice;
}


int main()
{
    // Create the main Well Killer controller.
    WellController controller;

    int choice = 0;

    // Display the opening instructions.
    displayOpeningScreen();


    // Continue until the user selects Exit.
    while (choice != 6)
    {
        try
        {
            displayMenu();

            choice = getMenuChoice();


          

            if (choice == 1)
            {
                controller.displayWells();
            }


          

            else if (choice == 2)
            {
                int wellID;

                controller.displayWells();

                cout << endl;
                cout << "Enter the well number to shut down: ";

                cin >> wellID;

                // Validate numeric input.
                if (cin.fail())
                {
                    cin.clear();

                    cin.ignore(
                        numeric_limits<streamsize>::max(),
                        '\n'
                    );

                    throw invalid_argument(
                        "The well number must be a number."
                    );
                }

                controller.shutdownWell(wellID);
            }



            else if (choice == 3)
            {
                char confirm;

                cout << endl;
                cout << "WARNING:" << endl;
                cout << "This will shut down EVERY well." << endl;

                cout << "Continue? (Y/N): ";

                cin >> confirm;

                if (confirm == 'Y' || confirm == 'y')
                {
                    controller.shutdownAllWells();
                }
                else
                {
                    cout << "Shutdown cancelled." << endl;
                }
            }


         

            else if (choice == 4)
            {
                controller.viewLog();
            }


        

            else if (choice == 5)
            {
                controller.testLoRa();
            }


            

            else if (choice == 6)
            {
                cout << "\n==================================" << endl;
                cout << "Well Killer shutting down safely." << endl;
                cout << "Thank you for using Well Killer." << endl;
                cout << "==================================" << endl;
            }
        }



        catch (const invalid_argument& error)
        {
            cout << endl;
            cout << "INPUT ERROR: "
                << error.what() << endl;
        }


      

        catch (const out_of_range& error)
        {
            cout << endl;
            cout << "RANGE ERROR: "
                << error.what() << endl;
        }



        catch (const exception& error)
        {
            cout << endl;
            cout << "SYSTEM ERROR: "
                << error.what() << endl;
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
