#pragma once

#include <QObject>
#include <QSharedPointer>

class Initializer : public QObject
{
    Q_OBJECT

public:
    explicit Initializer(QObject* parent = nullptr)
        : QObject(parent)
    {}

    ~Initializer() override
    {
        for (const auto& connection : m_connections)
        {
            disconnect(*connection);
        }
    }

    bool isInitialized() const { return m_connections.isEmpty(); }

    template<typename Func1>
    inline void addStage(const typename QtPrivate::FunctionPointer<Func1>::Object* sender, Func1 signal)
    {
        auto connection = QSharedPointer<QMetaObject::Connection>::create();

        *connection = connect(sender, signal, [this, connection]() {
            stageInitialized(connection);
        });

        stageAdded(connection);
    }

    template<typename Func1>
    inline void addStage(const typename QtPrivate::FunctionPointer<Func1>::Object* sender, Func1 signal, const std::function<bool()>& condition)
    {
        auto connection = QSharedPointer<QMetaObject::Connection>::create();

        *connection = connect(sender, signal, [this, connection, condition]() {
            if (condition())
            {
                stageInitialized(connection);
            }
        });

        stageAdded(connection);
    }

signals:
    void initialized();

private:
    void stageAdded(const QSharedPointer<QMetaObject::Connection>& connection)
    {
        m_connections.append(connection);
    }

    void stageInitialized(const QSharedPointer<QMetaObject::Connection>& connection)
    {
        disconnect(*connection);

        m_connections.removeAll(connection);

        if (m_connections.isEmpty())
        {
            emit initialized();
        }
    }

private:
    QList<QSharedPointer<QMetaObject::Connection>> m_connections;
};
