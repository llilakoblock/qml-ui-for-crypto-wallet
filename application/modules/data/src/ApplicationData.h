#pragma once

#include "Application.h"

#include <QSharedData>
#include <QUrl>

namespace Data {

class ApplicationData : public QSharedData
{
public:
    inline bool operator== (const ApplicationData& other) const
    {
        return state == other.state;
    }

    Application::State state { Application::State::Unknown };
};

} // namespace Data
