#ifndef ISTORAGE_H
#define ISTORAGE_H

#include <QString>
#include <QVector>

struct OperatorNode
{
    QString mcc;
    QString mnc;
    QString name;
};

struct CountryNode
{
    QString mcc;
    QString code;
    QString name;
    QVector<OperatorNode> operators;
};

class IStorage
{
public:
    virtual QVector<CountryNode> loadAllCountries() = 0;
    virtual ~IStorage() {}
};

#endif // ISTORAGE_H
