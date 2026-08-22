#pragma once

#include <string>

using namespace std;

class Well
{
private:

    int wellID;

    string wellName;

    bool running;

public:

    Well(int id, string name);

    int getID() const;

    string getName() const;

    bool isRunning() const;

    void shutdown();

    void displayStatus() const;
};