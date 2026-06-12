#include "papi_wrapper.h"
#include <papi.h>
#include <iostream>

static int EventSet = PAPI_NULL;
static bool is_initialized = false;

bool papi_init() {
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        std::cerr << "PAPI library init error: " << PAPI_strerror(retval) << "\n";
        return false;
    }
    
    retval = PAPI_create_eventset(&EventSet);
    if (retval != PAPI_OK) {
        std::cerr << "PAPI create event set error: " << PAPI_strerror(retval) << "\n";
        return false;
    }

    // Add Level 2 Data Cache Misses event
    retval = PAPI_add_event(EventSet, PAPI_L2_DCM);
    if (retval != PAPI_OK) {
        std::cerr << "PAPI add event PAPI_L2_DCM error: " << PAPI_strerror(retval) << "\n";
        return false;
    }
    
    is_initialized = true;
    return true;
}

bool papi_start() {
    if (!is_initialized) return false;
    PAPI_reset(EventSet);
    return PAPI_start(EventSet) == PAPI_OK;
}

long long papi_stop() {
    if (!is_initialized) return -1;
    long long values[1] = {0};
    PAPI_stop(EventSet, values);
    return values[0];
}