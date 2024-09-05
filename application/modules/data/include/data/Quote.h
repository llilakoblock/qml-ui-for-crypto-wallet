#pragma once

#include <QSharedDataPointer>
#include <QtQmlIntegration>

namespace Data {

class QuoteData;

class Quote final
{
    Q_GADGET
    QML_VALUE_TYPE(quote)

    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT FINAL)
    Q_PROPERTY(qreal price READ price CONSTANT FINAL)
    Q_PROPERTY(qreal capacity READ capacity CONSTANT FINAL)

public:
    explicit Quote();

    Quote(Quote&& other) noexcept;
    Quote(const Quote& other) noexcept;
    ~Quote();

    Quote& operator= (const Quote& other) noexcept;
    bool   operator== (const Quote& other) const;

    QDateTime timestamp() const;
    void      setTimestamp(const QDateTime& timestamp);

    qreal price() const;
    void  setPrice(qreal price);

    qreal capacity() const;
    void  setCapacity(qreal capacity);

private:
    QSharedDataPointer<QuoteData> data;
};

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Quote, Q_MOVABLE_TYPE);
