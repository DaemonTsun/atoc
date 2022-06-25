
#pragma once

#include <vector>

// the output data of a decompilation
struct decompile_data
{
    std::vector<const char*> header_files;

    // TODO: the rest of the decompilation output
};
