#pragma once

#include "data/Application.h"
#include "data/Step.h"
#include "data/Wallet.h"

#include <QSharedDataPointer>
#include <QtQmlIntegration>

namespace Data {

class StateData;

class State final
{
    Q_GADGET
    QML_VALUE_TYPE(state)

public:
    explicit State();

    State(State&& other) noexcept;
    State(const State& other) noexcept;
    ~State();

    State& operator= (const State& other) noexcept;
    bool   operator== (const State& other) const;

    Data::Application application() const;
    void              setApplication(const Data::Application& application);

    Data::Wallet wallet() const;
    void         setWallet(const Data::Wallet& wallet);

    Data::Step step() const;
    void       setStep(const Data::Step& step);

private:
    QSharedDataPointer<StateData> data;
};

} // namespace Data

Q_DECLARE_TYPEINFO(Data::State, Q_MOVABLE_TYPE);
