#ifndef ARC_DATAACCESSOR_HPP
#define ARC_DATAACCESSOR_HPP

#include <stdint.h>

enum DataItemId
{
    // Measurements
    TIME_ID             = 0,
    QOUT_ID             = 1,
    POUT_ID             = 2,
    PPROX_ID            = 3,
    MOTOR_SPEED_ID      = 4,
    MOTOR_CURRENT_ID    = 5,

    // Targets
    MAIN_MOTOR_TARGET_ID    = 6,
    PEEP_MOTOR_TARGET_ID    = 7,
    VALVE_IE_TARGET_ID      = 8,
    PUSHPULL_TARGET_ID      = 9,

    // SI Targets
    //MOTOR_CONTROL_ID        = 20,
    QOUT_TARGET_ID          = 10,
    POUT_TARGET_ID          = 11,

    // State
    BREATH_STATE_ID         = 12,
    INSPI_TIME_ID           = 13,
    EXPI_TIME_ID            = 14,
};

struct Datagram
{
    int32_t value;
    int32_t reset;
    int32_t min;
    int32_t max;
    int32_t div;
    const char* name;
};

class DataItem
{
public:
    DataItem(DataItemId id);
    DataItem(DataItemId id, bool writable);
    ~DataItem();

    Datagram& get();
    void set(int32_t val);
    void reset();


private:
    Datagram&   data_;
    bool        writable_ {false};
};

#endif
