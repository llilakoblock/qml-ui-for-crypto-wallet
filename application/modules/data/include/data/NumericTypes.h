#pragma once

#include "boost/multiprecision/cpp_dec_float.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/multiprecision/number.hpp"

#include <QString>

namespace Data {

using UInt256  = boost::multiprecision::uint256_t;
using Int256   = boost::multiprecision::int256_t;
using Float100 = boost::multiprecision::cpp_dec_float_100;

inline static QString toPrettyString(const Float100& value, quint32 precision = 8, quint32 keepZeros = 0, bool thousandsSeparator = true)
{
    QString result = QString::fromStdString(value.str(precision, std::ios::fixed));

    if (thousandsSeparator)
    {
        qsizetype fromIndex = result.indexOf('.');

        if (fromIndex == -1)
        {
            fromIndex = result.size() - 1;
        }

        for (qsizetype i = fromIndex - 3; i > 0; i -= 3)
        {
            result.insert(i, ',');
        }
    }

    qsizetype dotIndex = result.indexOf('.');

    if (dotIndex == -1)
    {
        return result;
    }

    if (dotIndex == result.size() - 1)
    {
        result.chop(1);
        return result;
    }

    if (dotIndex + keepZeros == result.size() - 1)
    {
        return result;
    }

    qsizetype it = result.size() - 1;

    while (it > dotIndex && result[it] == '0')
    {
        --it;
    }

    if (it != dotIndex)
    {
        ++it;
    }

    if (it < result.size())
    {
        result.chop(result.size() - it);
    }

    if (result[result.size() - 1] == '.')
    {
        result.chop(1);
    }

    return result;
}

inline static QString toPrettyString(const UInt256& value, bool thousandsSeparator = false)
{
    QString result = QString::fromStdString(value.str());

    if (thousandsSeparator)
    {
        for (qsizetype i = result.size() - 3; i > 0; i -= 3)
        {
            result.insert(i, ' ');
        }
    }

    return result;
}

inline static QString toPrettyString(const Int256& value, bool thousandsSeparator = false)
{
    QString result = QString::fromStdString(value.str());

    if (thousandsSeparator)
    {
        for (qsizetype i = result.size() - 3; i > 0; i -= 3)
        {
            result.insert(i, ' ');
        }
    }

    return result;
}

} // namespace Data
