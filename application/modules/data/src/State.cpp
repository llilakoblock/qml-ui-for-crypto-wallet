#include "data/State.h"

#include "StateData.h"

namespace Data {

State::State()
    : data(new StateData)
{
}

State::State(State&& other) noexcept      = default;
State::State(const State& other) noexcept = default;
State::~State()                           = default;

State& State::operator= (const State& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool State::operator== (const State& other) const
{
    return data == other.data || *data == *other.data;
}

Application State::application() const
{
    return data->application;
}

void State::setApplication(const Application& application)
{
    data->application = application;
}

Data::Wallet State::wallet() const
{
    return data->wallet;
}

void State::setWallet(const Data::Wallet& wallet)
{
    data->wallet = wallet;
}

Step State::step() const
{
    return data->step;
}

void State::setStep(const Step& step)
{
    data->step = step;
}

} // namespace Data
