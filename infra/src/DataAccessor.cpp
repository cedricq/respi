#include "DataAccessor.hpp"


Datagram DataItems[] =
{
// Measurements
[TIME_ID]          =    {.value = 0,    .reset = 0,  .min = INT32_MIN,  .max = INT32_MAX,  .div = 1000,    .name = "time"},
[QOUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 30000,      .div = 100,     .name = "m_qout"},
[POUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "m_pout"},
[PPROX_ID]         =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "m_prox"},
[MOTOR_SPEED_ID]   =    {.value = 0,    .reset = 0,  .min = 0,          .max = 100000,     .div = 1,       .name = "m_spd"},
[MOTOR_CURRENT_ID] =    {.value = 0,    .reset = 0,  .min = 0,          .max = 1000,       .div = 100,     .name = "m_crt"},

// Targets
[MAIN_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_mot"},
[PEEP_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_pep"},
[VALVE_IE_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_ie"},
[PUSHPULL_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_pp"},

// SI Targets
[QOUT_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 10000,      .div = 100,      .name = "t_qout"},
[POUT_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 600,        .div = 100,      .name = "t_pout"},

// State
[BREATH_STATE_ID] =     {.value = 0,    .reset = 0,  .min = 0,          .max = 1,          .div = 1,       .name = "breath"},
[INSPI_TIME_ID] =       {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "m_ti"},
[EXPI_TIME_ID] =        {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "m_te"},

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
