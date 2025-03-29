#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include <QVector>
#include <QVariant>
#include <QIcon>
#include <QString>
#include "istorage.h"

class TreeNode
{
public:
    explicit TreeNode(const QVariant &a_data, TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
    TreeNode *child(int row) const;
    int childCount() const;
    int row() const;
    QVariant data() const;
    TreeNode *parentItem();
    void setIcon(const QIcon &icon);
    QIcon icon() const;

private:
    QVector<TreeNode*> m_childItems;
    QVariant m_itemData;
    QIcon m_icon;
    TreeNode *m_parentItem;
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void loadFromStorage(IStorage *storage);

private:
    void setupModelData(const QVector<CountryNode> &countries);
    TreeNode *m_rootItem;
};

#endif // TREEMODEL_H
