#include "DataAccessor.hpp"


Datagram DataItems[] =
{
// Measurements
[TIME_ID]          =    {.value = 0,    .reset = 0,  .min = INT32_MIN,  .max = INT32_MAX,  .div = 1000,    .name = "time\0"},
[QOUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 30000,      .div = 100,     .name = "qout\0"},
[POUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "pout\0"},
[PPROX_ID]         =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "prox\0"},
[MOTOR_SPEED_ID]   =    {.value = 0,    .reset = 0,  .min = 0,          .max = 100000,     .div = 1,       .name = "mot_spd\0"},
[MOTOR_CURRENT_ID] =    {.value = 0,    .reset = 0,  .min = 0,          .max = 1000,       .div = 100,     .name = "mot_crt\0"},

// Targets
[MAIN_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "mot_target\0"},
[PEEP_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "pep_target\0"},
[VALVE_IE_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "ie_target\0"},
[TEST_TARGET_ID] =          {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "test_target\0"},

// State
[BREATH_STATE_ID] =     {.value = 0,    .reset = 0,  .min = 0,          .max = 1,          .div = 1,       .name = "br_state\0"},
[INSPI_TIME_ID] =       {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "i_time\0"},
[EXPI_TIME_ID] =        {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "e_time\0"},

};

DataItem::DataItem(DataItemId id)
        :   data_(DataItems[id]),
            writable_(false)
{}

DataItem::DataItem(DataItemId id, bool writable)
        :   data_(DataItems[id]),
            writable_(writable)
{}

DataItem::~DataItem()
{}

Datagram& DataItem::get()
{
    return data_;
}


void DataItem::set(int32_t val)
{
    if (writable_)
    {
        if (val >= data_.min and val <= data_.max)
        {
            data_.value = val;
        }
    }
}

void DataItem::reset()
{
    set(data_.reset);
}
