#include "sqlitestorage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SQLiteStorage::SQLiteStorage(const QString &a_path) : m_path(a_path) {}

QVector<CountryNode> SQLiteStorage::loadAllCountries()
{
    QVector<CountryNode> result;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_path);
    if (!db.open()) {
        qDebug() << "Failed to open DB:" << db.lastError().text();
        return result;
    }

    QSqlQuery countryQuery("SELECT code, name, mcc FROM countries");
    while (countryQuery.next())
    {
        CountryNode country;
        country.code = countryQuery.value(0).toString();
        country.name = countryQuery.value(1).toString();
        country.mcc = countryQuery.value(2).toString();

        QSqlQuery operatorQuery;
        operatorQuery.prepare("SELECT mcc, mnc, name FROM operators WHERE mcc = ?");
        operatorQuery.addBindValue(country.mcc);
        operatorQuery.exec();
        while (operatorQuery.next())
        {
            OperatorNode op;
            op.mcc = operatorQuery.value(0).toString();
            op.mnc = operatorQuery.value(1).toString();
            op.name = operatorQuery.value(2).toString();
            country.operators.append(op);
        }

        result.append(country);
    }

    return result;
}
