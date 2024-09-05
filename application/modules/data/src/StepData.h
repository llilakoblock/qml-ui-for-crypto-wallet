#pragma once

#include "data/Step.h"

#include <QList>
#include <QSharedData>

namespace Data {

class StepData : public QSharedData
{
public:
    inline bool operator== (const StepData& other) const
    {
        return screen == other.screen &&
               direction == other.direction &&
               errors == other.errors &&
               inheritanceErrors == other.inheritanceErrors &&
               restorationMethod == other.restorationMethod &&
               password == other.password &&
               mnemonicPassphrase == other.mnemonicPassphrase &&
               encryptedSeed == other.encryptedSeed &&
               inheritanceKey == other.inheritanceKey &&
               screensStack == other.screensStack;
    }

    Step::Screen               screen { Step::Screen::Unknown };
    Step::Direction            direction { Step::Direction::UnknownDirection };
    Step::Errors               errors { Step::Error::None };
    Step::InheritanceKeyErrors inheritanceErrors { Step::InheritanceKeyError::None };
    Step::RestorationMethod    restorationMethod { Step::RestorationMethod::Unknown };

    QString        password;
    QList<QString> mnemonicPassphrase;

    std::vector<unsigned char> encryptedSeed;

    QString inheritanceKey;

    QStack<Step::Screen> screensStack;
};

} // namespace Data
