#include "data/SwapLog.h"

#include "SwapLogData.h"

namespace Data {

SwapLog::SwapLog()
    : data(new SwapLogData)
{
}

SwapLog::SwapLog(SwapLog&& other) noexcept      = default;
SwapLog::SwapLog(const SwapLog& other) noexcept = default;
SwapLog::~SwapLog()                             = default;

SwapLog& SwapLog::operator= (const SwapLog& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool SwapLog::operator== (const SwapLog& other) const
{
    return data == other.data || *data == *other.data;
}

SwapLog::State SwapLog::state() const
{
    return data->state;
}

void SwapLog::setState(State state)
{
    data->state = state;
}

QString SwapLog::text() const
{
    return data->text;
}

void SwapLog::setText(const QString& text)
{
    data->text = text;
}

QDateTime SwapLog::time() const
{
    return data->time;
}

void SwapLog::setTime(const QDateTime& time)
{
    data->time = time;
}

} // namespace Data
