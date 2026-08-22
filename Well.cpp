#include "Well.h"
#include <iostream>

using namespace std;

Well::Well(int id, string name)
{
    wellID = id;
    wellName = name;
    running = true;
}

int Well::getID() const
{
    return wellID;
}

string Well::getName() const
{
    return wellName;
}

bool Well::isRunning() const
{
    return running;
}

void Well::shutdown()
{
    running = false;
}

void Well::displayStatus() const
{
    cout << "Well " << wellID
        << " - " << wellName
        << " - ";

    if (running)
    {
        cout << "RUNNING";
    }
    else
    {
        cout << "STOPPED";
    }

    cout << endl;
}