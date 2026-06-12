#ifndef PAPI_WRAPPER_H
#define PAPI_WRAPPER_H

// Initialize PAPI library and create the event set for PAPI_L2_TCM
bool papi_init();

// Start hardware counters
bool papi_start();

// Stop counters and return the total L2 Cache Misses
long long papi_stop();

#endif // PAPI_WRAPPER_H