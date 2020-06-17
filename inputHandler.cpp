#include "inputHandler.h"

string inputHandler::getPath()
{
    cout << "Please enter file name to create a matrix." << endl;
    bool firstTime = true;
    string currentPath;
    string fileName;
    string filePath;
    currentPath = get_working_path();
    cout << "Current Path: " << currentPath << endl;
    do
    {
        if (!firstTime)
        {
            cout << "Incorrect File Name." << endl;
        }
        cin >> fileName;
        filePath = currentPath + "\\" + fileName;
        firstTime = false;
    } while (!validPath(filePath));
    return filePath;
}

bool inputHandler::validPath(string path)
{
    ifstream valid(path);
    return !!valid;
}

string inputHandler::get_working_path()
{
    char temp[MAXPATHLEN];
    return (_getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}