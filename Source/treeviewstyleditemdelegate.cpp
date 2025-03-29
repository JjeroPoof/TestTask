#include "treeviewstyleditemdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QFont>
#include <qtreeview.h>

void TreeViewStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option,
                                  QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorBranch)
    {
        const QStyleOptionViewItem *opt = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        if (opt)
        {
            QRect r = option->rect.adjusted(2, 2, -2, -2);
            QPoint c = r.center();

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor("#404040"));

            if (opt->state & QStyle::State_Children)
            {
                bool expanded = opt->state & State_Open;
                QPolygon triangle;

                if (expanded)
                {
                    triangle << QPoint(c.x() - 4, c.y() - 2)
                    << QPoint(c.x() + 4, c.y() - 2)
                    << QPoint(c.x(), c.y() + 4);
                } else {
                    triangle << QPoint(c.x() - 2, c.y() - 4)
                    << QPoint(c.x() - 2, c.y() + 4)
                    << QPoint(c.x() + 4, c.y());
                }

                painter->drawPolygon(triangle);
            }


            painter->restore();
            return;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}



void TreeViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    painter->save();

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

    const QTreeView* tree = qobject_cast<const QTreeView*>(widget);
    if (tree)
    {
        QRect rect = opt.rect;

        painter->setPen(QColor("#dddddd"));
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen pen(QColor("#cccccc"));
        pen.setWidth(1);
        painter->setPen(pen);

        // 1. Визначити рівень вкладеності
        QModelIndex parent = index.parent();
        int level = 0;
        while (parent.isValid())
        {
            ++level;
            parent = parent.parent();
        }

        int indent = tree->indentation();
        int centerY = rect.center().y();
        int baseX = rect.left() - indent * (level + 1);

        // 2. Вертикальні лінії для кожного предка
        for (int i = 0; i < level; ++i)
        {
            QModelIndex ancestor = index;
            for (int j = 0; j < level - i; ++j)
                ancestor = ancestor.parent();

            bool hasMoreSiblings = ancestor.row() < ancestor.model()->rowCount(ancestor.parent()) - 1;
            if (hasMoreSiblings)
            {
                int x = baseX + indent * i + indent / 2;
                painter->drawLine(x, rect.top(), x, rect.bottom());
            }
        }

        // 3. Гілка поточного елемента
        {
            int x = baseX + indent * level + indent / 2;

            bool drawBranch = true;
            if (!index.parent().isValid() && index.model()->rowCount(index) > 0)
                drawBranch = false;


            if (drawBranch)
            {
                bool isLastChild = false;
                if (index.parent().isValid())
                    isLastChild = index.row() == index.model()->rowCount(index.parent()) - 1;
                else
                    isLastChild = index.row() == index.model()->rowCount(QModelIndex()) - 1;


                if (isLastChild)
                    painter->drawLine(x, rect.top(), x, centerY);
                else
                    painter->drawLine(x, rect.top(), x, rect.bottom());

                painter->drawLine(x, centerY, x + indent / 2, centerY);
            }
        }

        painter->restore();

        // 4. Малювання іконки та тексту
        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
        QString text = index.data(Qt::DisplayRole).toString();

        QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
        QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, widget);

        bool isBold = !index.parent().isValid();
        QFont font = opt.font;
        font.setBold(isBold);
        painter->setFont(font);
        painter->setPen(opt.palette.color(QPalette::Text));

        if (!icon.isNull())
            icon.paint(painter, iconRect, Qt::AlignVCenter | Qt::AlignLeft);

        int iconOffset = icon.isNull() ? 0 : iconRect.width() + 6;
        QRect alignedTextRect = textRect.adjusted(iconOffset, 0, 0, 0);
        painter->drawText(alignedTextRect, Qt::AlignVCenter | Qt::AlignLeft, text);
    }

    painter->restore();
}



