
#pragma once

#include <file_stream.hpp>
#include "decompile_data.hpp"

struct decompile_format_config
{
    file_stream *log;
    bool verbose;

    const decompile_data *data;
    // TODO: maybe some formatting options here
};

void decompile_format(const decompile_format_config *conf, file_stream *out);
