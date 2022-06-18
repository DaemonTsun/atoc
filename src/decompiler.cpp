
#include <psp_elf.hpp>
#include <parse_instructions.hpp>
#include "decompiler.hpp"

void decompile_allegrex(const decompile_conf *conf, file_stream *out)
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
    pdatas.resize(epdata.sections.size());

    for (int i = 0; i < epdata.sections.size(); ++i)
    {
        elf_section *sec = &epdata.sections[i];

        parse_config pconf;
        pconf.log = conf->log;
        pconf.vaddr = sec->vaddr;
        pconf.verbose = conf->verbose;
        pconf.emit_pseudo = true;

        parse_data *pdata = &pdatas[i];
        pdata->jump_destinations = &jumps;
        parse_allegrex(&sec->content, &pconf, pdata);
    }
}
