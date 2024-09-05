#pragma once

#include "data/Application.h"
#include "data/Step.h"
#include "data/Wallet.h"

#include <QSharedData>

namespace Data {

class StateData : public QSharedData
{
public:
    inline bool operator== (const StateData& other) const
    {
        return application == other.application &&
               wallet == other.wallet &&
               step == other.step;
    }

    Data::Application application;

    Data::Wallet wallet;

    Data::Step step;
};

} // namespace Data
