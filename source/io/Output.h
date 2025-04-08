#pragma once
#include <ostream>
#include <string>
#include <fstream>
#include <H5Cpp.h>
#include "../Run.h"

class Run;
class Output
{
public:
    Output() = default;
    Output(Run* run);

    // Operate
    void WriteToHDF5(const std::string& filename);

protected:
    Run* m_run;
};