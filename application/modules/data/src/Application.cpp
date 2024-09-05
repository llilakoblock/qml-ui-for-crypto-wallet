#include "Application.h"

#include "ApplicationData.h"

namespace Data {

Application::Application()
    : data(new ApplicationData)
{}

Application::Application(Application&& other) noexcept      = default;
Application::Application(const Application& other) noexcept = default;
Application::~Application()                                 = default;

Application& Application::operator= (const Application& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Application::operator== (const Application& other) const
{
    return data == other.data || *data == *other.data;
}

Application::State Application::state() const
{
    return data->state;
}

void Application::setState(Application::State state)
{
    data->state = state;
}

} // namespace Data
