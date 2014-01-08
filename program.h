#ifndef PROGRAM_H
#define PROGRAM_H

#include "wsdata.h"
#include "tool.h"

class Program
{
public:
    Program();
    virtual WSData* getWSData() = 0;
    virtual Tool* getTool() = 0;
};

#endif // PROGRAM_H
