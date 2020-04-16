#include "triage_reeses.h"

//#define DEBUG

#ifdef DEBUG
#define debug_cout cerr << hex << "[TRIAGE_REESES] "
#else
#define debug_cout if (0) cerr
#endif

TriageReeses::TriageReeses()
    : temporal_update(0),
      spatial_update(0)
{
}

void TriageReeses::set_conf(TriageConfig *config)
{
    TriageBase::set_conf(config);
    tu.FOOTPRINT = config->reeses_footprint;
}

/* This is the main training function for Reeses,
 * which is called on every cache miss. I've removed all the extraneous
 * functions for simplicity's sake, so that you can get a general idea
 * of how to use the training unit.
 */
void TriageReeses::train(uint64_t cur_pc, uint64_t addr, bool cache_hit)
{
    // get new correlated pair from training unit
    // result contains last entry (spatial/temporal from the same pc)
    // Connect from last entry to current entry
    TUEntry *result = tu.update(cur_pc, addr);
    if (result != nullptr) {
        ++new_addr;
        uint64_t trigger = result->temporal;
        if (!result->has_spatial) {
            // correct ordering for temporal entries
            result->temporal = addr;
            //temporal_counts[cur_pc] += 1;
        }
        // link end of spatials to next temporal
        if (!tu.FOOTPRINT && result->has_spatial) {
            uint64_t last_addr = result->spatial->last_address();
            TUEntry *link = new TUEntry(addr);
            debug_cout << "Update Spatial: " << last_addr << " To " << addr << " @PC "
                << cur_pc << endl;
            ++spatial_update;
            on_chip_data.update(last_addr, addr, cur_pc, true, link);
        } else {
            debug_cout << "Update Temporal: " << trigger << " To " << addr << " @PC "
                << cur_pc << endl;
            ++temporal_update;
            on_chip_data.update(trigger, addr, cur_pc, true, result->clone());
        }
        delete result;
    } else {
        ++same_addr;
    }
}

void TriageReeses::predict(uint64_t pc, uint64_t addr, bool cache_hit)
{
    TriageBase::predict(pc, addr, cache_hit);
}

void TriageReeses::print_stats()
{
    TriageBase::print_stats();
    cout << "TemporalUpdate=" << temporal_update << endl;
    cout << "SpatialUpdate=" << spatial_update << endl;
}

