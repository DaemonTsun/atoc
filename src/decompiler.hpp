
#pragma once

#include <file_stream.hpp>
#include <memory_stream.hpp>

#include "decompile_data.hpp"

struct decompile_conf
{
    memory_stream *in;

    file_stream *log;
    bool verbose;
};

void decompile_allegrex(const decompile_conf *conf, decompile_data *out);
