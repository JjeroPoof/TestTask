#ifndef TREEVIEWSTYLEDITEMDELEGATE_H
#define TREEVIEWSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>

class TreeViewStyle : public QProxyStyle
{
public:
    using QProxyStyle::QProxyStyle;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;
};

class TreeViewStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // TREEVIEWSTYLEDITEMDELEGATE_H
