#ifndef __TABLEISB_H__
#define __TABLEISB_H__

#include <iostream>
#include <map>
#include <vector>

#include "tableisb_training_unit.h"
#include "tableisb_onchip.h"

struct TableISBConfig {
    int lookahead;
    int degree;

    int on_chip_size, on_chip_assoc;
    int training_unit_size;

    TableISBReplType repl;
};

class TableISB {
    TableISBTrainingUnit training_unit;

    int lookahead, degree;

    void train(uint64_t pc, uint64_t addr, bool hit);
    void predict(uint64_t pc, uint64_t addr, bool hit);

    // Stats
    uint64_t same_addr, new_addr, new_stream;
    uint64_t predict_count, trigger_count;

    std::vector<uint64_t> next_addr_list;

    public:
    TableISBOnchip on_chip_data;
        TableISB();
        void set_conf(TableISBConfig *config);
        void calculatePrefetch(uint64_t pc, uint64_t addr,
                bool cache_hit, uint64_t *prefetch_list,
                int max_degree);
        void print_stats();
};

#endif // __TABLEISB_H__