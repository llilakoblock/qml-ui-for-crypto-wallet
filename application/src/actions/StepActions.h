#pragma once

#include "data/Step.h"

#include <variant>

namespace XWallet::StepActions {

struct EmptyData
{
};

struct PasswordData
{
    QString password;
};

struct InheritanceKeyData
{
    QString key;
};

struct InheritanceInactivityDurationData
{
    int maxInactivityDuration;
};

struct PassphraseData
{
    QList<QString> passphrase;
};

struct FileData
{
    QUrl    filePath;
    QString password;
};

using StepData = std::variant<EmptyData,
                              PasswordData,
                              PassphraseData,
                              FileData,
                              InheritanceKeyData,
                              InheritanceInactivityDurationData>;

struct SetInitScreen
{
    Data::Step::Screen screen { Data::Step::Screen::Unknown };
};

struct FinishScreen
{
    Data::Step::Screen currentScreen { Data::Step::Screen::Unknown };
    Data::Step::Screen nextScreen { Data::Step::Screen::Unknown };

    StepData stepData;
};

struct NextScreen
{};

struct PreviousScreen
{};

using Action = std::variant<SetInitScreen,
                            FinishScreen,
                            NextScreen,
                            PreviousScreen>;

inline QDebug operator<< (QDebug debug, SetInitScreen item)
{
    QDebugStateSaver saver(debug);

    debug << "SetInitScreen" << Qt::endl
          << "Init screen:" << item.screen;

    return debug;
}

inline QDebug operator<< (QDebug debug, const FinishScreen& item)
{
    QDebugStateSaver saver(debug);

    debug << "FinishScreen" << Qt::endl
          << "Current screen:" << item.currentScreen << Qt::endl
          << "Next screen:" << item.nextScreen;

    return debug;
}

inline QDebug operator<< (QDebug debug, NextScreen item)
{
    QDebugStateSaver saver(debug);

    debug << "NextScreen";

    return debug;
}

inline QDebug operator<< (QDebug debug, PreviousScreen item)
{
    QDebugStateSaver saver(debug);

    debug << "PreviousScreen";

    return debug;
}

} // namespace XWallet::StepActions
