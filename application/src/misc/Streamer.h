#pragma once

#include <QDebug>

template<class T>
struct Streamer
{
    Streamer(const T& value)
        : value { value }
    {}

    const T& value;
};

template<class T>
Streamer(T) -> Streamer<T>;

template<class T>
QDebug operator<< (QDebug debug, const Streamer<T> s)
{
    QDebugStateSaver saver(debug);

    debug << Qt::endl
          << "======================================" << Qt::endl
          << "Action posted:" << s.value << Qt::endl
          << "======================================" << Qt::endl;

    return debug;
}

template<class... Ts>
QDebug operator<< (QDebug debug, Streamer<std::variant<Ts...>> sv)
{
    std::visit([&debug](const auto& v) { debug << Streamer { v }; }, sv.value);

    return debug;
}
