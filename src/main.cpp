
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string.hpp>
#include <file_stream.hpp>
#include <memory_stream.hpp>

#include "decompile_format.hpp"
#include "decompile_data.hpp"
#include "decompiler.hpp"
#include "config.hpp"

struct arguments
{
    std::string output_file; // -o, --output
    std::string log_file;    // --log
    bool verbose;            // -v, --verbose

    std::string input_file;
};

const arguments default_arguments{
    .output_file = "",
    .log_file = "",
    .verbose = false,
    .input_file = ""
};

void print_usage()
{
    puts("Usage: " ATOC_NAME " [-h] [-g] [-o OUTPUT] [-p] [-a VADDR] [-v] OBJFILE\n"
         "\n"
         ATOC_NAME " v" ATOC_VERSION ": allegrex to c decompiler\n"
         "by " ATOC_AUTHOR "\n"
         "\n"
         "Optional arguments:\n"
         "  -h, --help                  show this help and exit\n"
         "  -o OUTPUT, --output OUTPUT  output filename (default: stdout)\n"
         "  --log LOGFILE               output all information messages to LOGFILE (stdout by default)\n"
         "  -v, --verbose               verbose progress output\n"
         "\n"
         "Arguments:\n"
         "  OBJFILE      ELF object file to decompile\n"
         );
}

void parse_arguments(int argc, const char **argv, arguments *out)
{
    for (int i = 1; i < argc;)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            print_usage();
            exit(EXIT_SUCCESS);
        }

        if (arg == "-o" || arg == "--output")
        {
            if (i >= argc - 1)
                throw std::runtime_error(str(arg, " expects a positional argument: the output file"));

            out->output_file = argv[i+1];
            i += 2;
            continue;
        }

        if (arg == "--log")
        {
            if (i >= argc - 1)
                throw std::runtime_error(str(arg, " expects a positional argument: the log file"));

            out->log_file = argv[i+1];
            i += 2;
            continue;
        }

        if (arg == "-v" || arg == "--verbose")
        {
            out->verbose = true;
            ++i;
            continue;
        }

        // etc
        if (begins_with(arg, str("-")))
            throw std::runtime_error(str("unknown argument '", arg, "'"));

        if (out->input_file.empty())
        {
            out->input_file = arg;
            ++i;
            continue;
        }
        else
            throw std::runtime_error(str("unexpected argument '", arg, "'"));
    }
}

int main(int argc, const char **argv)
try
{
    arguments args = default_arguments;

    parse_arguments(argc, argv, &args);

    if (args.input_file.empty())
        throw std::runtime_error("expected input file");

    // get logfile
    FILE *logfd = stdout;

    if (!args.log_file.empty())
        logfd = fopen(args.log_file.c_str(), "a");

    file_stream log(logfd);

    if (!log)
        throw std::runtime_error("could not open file to log");

    file_stream in(args.input_file, "rb");

    if (!in)
        throw std::runtime_error("could not open input file");

    in.calculate_size();
    memory_stream in_buf = memory_stream(in.size());
    in.read_at(in_buf.data(), 0, in.size());

    in.close();

    FILE *outfd = stdout;

    if (!args.output_file.empty())
        outfd = fopen(args.output_file.c_str(), "w");

    file_stream out(outfd);

    decompile_conf dconf;
    dconf.in = &in_buf;
    dconf.log = &log;
    dconf.verbose = args.verbose;
    
    decompile_data ddata;
    decompile_allegrex(&dconf, &ddata);

    decompile_format_config dfconf;
    dfconf.log = &log;
    dfconf.verbose = args.verbose;
    dfconf.data = &ddata;
    decompile_format(&dfconf, &out);

    out.close();
    return 0;
}
catch (std::runtime_error &e)
{
    fprintf(stderr, "error: %s\n", e.what());
    return 1;
}
