
#include <stdio.h>
#include "decompile_format.hpp"

void format_header_files(const decompile_format_config *conf, file_stream *out)
{
    out->format("\n// module import header files\n");

    for (const char *header : conf->data->header_files)
        out->format("#include <%s>\n", header);

    out->format("\n");
}

void decompile_format(const decompile_format_config *conf, file_stream *out)
{
    // TODO: top comments
    format_header_files(conf, out);
}
