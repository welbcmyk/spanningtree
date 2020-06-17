#pragma once
#include <iostream>
#include "SpanTreeGenerator.h"
#include <direct.h>

#define MAXPATHLEN 255

class inputHandler
{
public:
    string getPath();
private:
    TreeMatrix spanTree;
    bool validPath(string path);
    string get_working_path();
};

