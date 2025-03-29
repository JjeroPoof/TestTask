#include "treemodel.h"
#include <QSqlDatabase>
#include <QPainter>
#include <QApplication>
#include <QFont>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QDebug>

TreeNode::TreeNode(const QVariant &a_data, TreeNode *parent)
    : m_itemData(a_data), m_parentItem(parent) {}

TreeNode::~TreeNode()
{
    qDeleteAll(m_childItems);
}

void TreeNode::appendChild(TreeNode *child)
{
    m_childItems.append(child);
}

TreeNode *TreeNode::child(int row) const
{
    return m_childItems.value(row);
}

int TreeNode::childCount() const
{
    return m_childItems.size();
}

int TreeNode::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeNode*>(this));
    return 0;
}

QVariant TreeNode::data() const
{
    return m_itemData;
}

TreeNode *TreeNode::parentItem()
{
    return m_parentItem;
}

void TreeNode::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

QIcon TreeNode::icon() const
{
    return m_icon;
}

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent), m_rootItem(new TreeNode("Root")) {}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());

    if (role == Qt::DisplayRole)
        return item->data();
    if (role == Qt::DecorationRole)
        return item->icon();

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : m_rootItem;

    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeNode *parentItem = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : m_rootItem;
    return parentItem->childCount();
}

void TreeModel::loadFromStorage(IStorage *storage)
{
    delete m_rootItem;
    m_rootItem = new TreeNode("Root");

    QVector<CountryNode> countries = storage->loadAllCountries();
    setupModelData(countries);
}

void TreeModel::setupModelData(const QVector<CountryNode> &countries)
{
    for (const auto &country : countries)
    {
        auto *countryItem = new TreeNode(QString("%1 (%2)").arg(country.name, country.code), m_rootItem);

        QString iconPath = QCoreApplication::applicationDirPath() + QString("/Icons/Countries/%1.png").arg(country.code);
        if (!QIcon(iconPath).isNull())
            countryItem->setIcon(QIcon(iconPath));

        for (const auto &op : country.operators)
        {
            QString displayName = op.name + " (" + op.mcc + "-" + op.mnc + ")";
            auto *opItem = new TreeNode(displayName, countryItem);
            countryItem->appendChild(opItem);
            QString opIconPath = QCoreApplication::applicationDirPath() + QString("/Icons/Operators/%1_%2.png").arg(op.mcc).arg(op.mnc);
            if (QFile::exists(opIconPath))
                opItem->setIcon(QIcon(opIconPath));
        }

        m_rootItem->appendChild(countryItem);
    }
}
