#include "DataAccessor.hpp"


Datagram DataItems[] =
{
// Measurements
[TIME_ID]          =    {.value = 0,    .reset = 0,  .min = INT32_MIN,  .max = INT32_MAX,  .div = 1000,    .name = "time"},
[QOUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 30000,      .div = 100,     .name = "qout"},
[POUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "pout"},
[PPROX_ID]         =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "prox"},
[MOTOR_SPEED_ID]   =    {.value = 0,    .reset = 0,  .min = 0,          .max = 100000,     .div = 1,       .name = "spd"},
[MOTOR_CURRENT_ID] =    {.value = 0,    .reset = 0,  .min = 0,          .max = 1000,       .div = 100,     .name = "crt"},

// Targets
[MAIN_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "mott"},
[PEEP_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "pept"},
[VALVE_IE_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "iet"},
[TEST_TARGET_ID] =          {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "tt"},

// State
[BREATH_STATE_ID] =     {.value = 0,    .reset = 0,  .min = 0,          .max = 1,          .div = 1,       .name = "bs"},
[INSPI_TIME_ID] =       {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "ti"},
[EXPI_TIME_ID] =        {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "te"},

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
