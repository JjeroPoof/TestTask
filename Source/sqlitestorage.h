#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include "istorage.h"

class SQLiteStorage : public IStorage
{
public:
    explicit SQLiteStorage(const QString &a_path);
    QVector<CountryNode> loadAllCountries() override;

private:
    QString m_path;
};

#endif // SQLITESTORAGE_H
