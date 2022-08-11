#ifndef INC_TRIGGER_HPP_
#define INC_TRIGGER_HPP_

#include "DataAccessor.hpp"

class TriggerI
{
public:
    virtual bool IsTrigger() = 0;
    virtual void Reset() = 0;

private:
    static const Datagram& time_;

};

class TimeTrigger : public TriggerI
{
public:
    TimeTrigger(Datagram& time, int32_t threshold);
    void Set(int32_t threshold);

    bool IsTrigger() override;
    void Reset() override;

private:
    Datagram& time_;
    int32_t   threshold_ {1000};
};

class TriggerManager
{
public:
    static TriggerManager& GetInstance();

    void SetInspi(TriggerI& trigger);
    void SetExpi(TriggerI& trigger);
    bool IsTrigger();

private:
    TriggerManager(TriggerI& trg_Inspi, TriggerI& trg_Expi);

    TriggerI& triggerInspi_ ;
    TriggerI& triggerExpi_ ;

    Datagram& state_;
};

#endif /* INC_TRIGGER_HPP_ */
