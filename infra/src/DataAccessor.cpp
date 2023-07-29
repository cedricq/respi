#include "DataAccessor.hpp"


Datagram DataItems[] =
{
// Measurements
[TIME_ID]          =    {.value = 0,    .reset = 0,  .min = INT32_MIN,  .max = INT32_MAX,  .div = 1000,    .name = "time"  , false},
[QOUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 30000,      .div = 100,     .name = "m_qout", false},
[POUT_ID]          =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "m_pout", false},
[PPROX_ID]         =    {.value = 0,    .reset = 0,  .min = -1000,      .max = 10000,      .div = 100,     .name = "m_prox", false},
[MOTOR_SPEED_ID]   =    {.value = 0,    .reset = 0,  .min = 0,          .max = 100000,     .div = 1,       .name = "m_spd" , false},
[MOTOR_CURRENT_ID] =    {.value = 0,    .reset = 0,  .min = 0,          .max = 1000,       .div = 100,     .name = "m_crt" , false},

// Targets
[MAIN_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_mot", false},
[PEEP_MOTOR_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_pep", false},
[VALVE_IE_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_ie" , false},
[PUSHPULL_TARGET_ID] =      {.value = 0,     .reset = 0,  .min = 0,          .max = 1000,       .div = 10,      .name = "t_pp" , false},

// SI Targets
[QOUT_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 10000,      .div = 100,      .name = "t_qout", false},
[POUT_TARGET_ID] =    {.value = 0,     .reset = 0,  .min = 0,          .max = 600,        .div = 100,      .name = "t_pout", false},

// State
[BREATH_STATE_ID] =     {.value = 0,    .reset = 0,  .min = 0,          .max = 1,          .div = 1,       .name = "breath", false},
[INSPI_TIME_ID] =       {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "m_ti"  , false},
[EXPI_TIME_ID] =        {.value = 0,    .reset = 0,  .min = 0,          .max = INT32_MAX,  .div = 1000,    .name = "m_te"  , false},

// Dev
[STREAM_ON_ID] =        {.value = 0,    .reset = 0,  .min = 0,          .max = 1,  		   .div = 1,       .name = "stream", false},
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
    if (writable_ && !data_.override)
    {
        if (val >= data_.min and val <= data_.max)
        {
            data_.value = val;
        }
    }
}

void DataItem::setOverride(int32_t val)
{
    data_.override = true;
    if (val >= data_.min and val <= data_.max)
    {
        data_.value = val;
    }
}

void DataItem::releaseOverride()
{
    data_.override = false;
}

void DataItem::reset()
{
    set(data_.reset);
}
