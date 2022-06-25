
#include <string.h>

#include <algorithm>
#include <vector>

#include <psp_elf.hpp>
#include <internal/psp_module_function_pspdev_headers.hpp>
#include <parse_instructions.hpp>
#include "decompiler.hpp"

void disassemble_sections(const decompile_conf *conf, elf_parse_data *epdata, jump_destination_array *jumps, std::vector<parse_data> *pdatas)
{
    pdatas->resize(epdata->sections.size());

    for (int i = 0; i < epdata->sections.size(); ++i)
    {
        elf_section *sec = &epdata->sections[i];

        parse_config pconf;
        pconf.log = conf->log;
        pconf.vaddr = sec->vaddr;
        pconf.verbose = conf->verbose;
        pconf.emit_pseudo = true;

        parse_data *pdata = &pdatas->at(i);
        pdata->jump_destinations = jumps;
        parse_allegrex(&sec->content, &pconf, pdata);
    }
}

void get_module_headers(const elf_parse_data *epdata, std::vector<const char*> *out)
{
    out->clear();

    for (const auto &mod : epdata->imported_modules)
    for (const auto &mfunc : mod.functions)
    {
        const psp_function *func = mfunc.function;

        if (func->header_file == nullptr
         || strcmp(func->header_file, unknown_header) == 0)
            continue;

        const char *header = strstr(func->header_file, "/");

        if (header == nullptr)
            header = func->header_file;
        else
            header++; // move after the /

        out->push_back(header);
    }

    std::sort(out->begin(), out->end());
    out->erase(std::unique(out->begin(), out->end()), out->end());
    out->shrink_to_fit();
}

void decompile_allegrex(const decompile_conf *conf, decompile_data *out)
{
    psp_elf_read_config rconf;
    rconf.log = conf->log;
    rconf.section = ""; // all sections
    rconf.vaddr = INFER_VADDR;
    rconf.verbose = conf->verbose;

    elf_parse_data epdata;
    read_elf(conf->in, &rconf, &epdata);

    jump_destination_array jumps;
    std::vector<parse_data> pdatas;
    disassemble_sections(conf, &epdata, &jumps, &pdatas);

    get_module_headers(&epdata, &out->header_files);
}
