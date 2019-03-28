#ifndef __TABLEISB_ONCHIP_H__
#define __TABLEISB_ONCHIP_H__

#include <vector>
#include <map>
#include <set>
#include <stdint.h>
#include "optgen_simple.h"
#include "isb_hawkeye_predictor.h"

struct TableISBConfig;

enum TableISBReplType {
    TABLEISB_REPL_LRU,
    TABLEISB_REPL_HAWKEYE,
    TABLEISB_REPL_PERFECT
};

struct TableISBOnchipEntry {
    uint64_t next_addr;
    int confidence;
    // Used for replacement policy, it is rrpv for rrpv-based replacement
    // policies, but can be used for other usages (like frequency in LFU）
    uint64_t rrpv;

    TableISBOnchipEntry();
    void increase_confidence();
    void decrease_confidence();
};

class TableISBRepl {
    protected:
        std::vector<std::map<uint64_t, TableISBOnchipEntry> > *entry_list;
        TableISBReplType type;

    public:
        TableISBRepl(std::vector<std::map<uint64_t, TableISBOnchipEntry> >* entry_list);
        virtual void addEntry(uint64_t set_id, uint64_t addr, uint64_t pc) = 0;
        virtual uint64_t pickVictim(uint64_t set_id) = 0;
        virtual void print_stats() {}

        static TableISBRepl* create_repl(std::vector<std::map<uint64_t, TableISBOnchipEntry> >* entry_list,
                TableISBReplType type, uint64_t assoc);
};

class TableISBReplLRU : public TableISBRepl
{
    public:
        TableISBReplLRU(std::vector<std::map<uint64_t, TableISBOnchipEntry> >* entry_list);
        void addEntry(uint64_t set_id, uint64_t addr, uint64_t pc);
        uint64_t pickVictim(uint64_t set_id);
};

class TableISBReplHawkeye : public TableISBRepl
{
    unsigned max_rrpv;
    std::vector<uint64_t> optgen_mytimer;
    std::vector<OPTgen> optgen;
    std::map<uint64_t, ADDR_INFO> optgen_addr_history;
    std::map<uint64_t, uint64_t> signatures;
    std::map<uint64_t, uint32_t> hawkeye_pc_ps_hit_predictions;
    std::map<uint64_t, uint32_t> hawkeye_pc_ps_total_predictions;
    IsbHawkeyePCPredictor predictor;

    public:
        TableISBReplHawkeye(std::vector<std::map<uint64_t, TableISBOnchipEntry> >* entry_list, uint64_t assoc);
        void addEntry(uint64_t set_id, uint64_t addr, uint64_t pc);
        uint64_t pickVictim(uint64_t set_id);

        void print_stats();
};

class TableISBReplPerfect : public TableISBRepl
{
    public:
        TableISBReplPerfect(std::vector<std::map<uint64_t, TableISBOnchipEntry> >* entry_list);
        void addEntry(uint64_t set_id, uint64_t addr, uint64_t pc);
        uint64_t pickVictim(uint64_t set_id);
};

class TableISBOnchip {
    uint32_t num_sets, assoc;
    uint64_t index_mask;
    std::vector<std::map<uint64_t, TableISBOnchipEntry> > entry_list;
    TableISBReplType repl_type;
    TableISBRepl *repl;

    uint64_t get_set_id(uint64_t addr);

    public:
        TableISBOnchip();
        void set_conf(TableISBConfig *config);

        void update(uint64_t prev_addr, uint64_t next_addr, uint64_t pc, bool update_repl);
        bool get_next_addr(uint64_t prev_addr, uint64_t &next_addr, uint64_t pc, bool update_stats);
        int increase_confidence(uint64_t addr);
        int decrease_confidence(uint64_t addr);

        void print_stats();
};

#endif // __TABLEISB_ONCHIP_H__
