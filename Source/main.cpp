#include <QApplication>
#include <qtreeview.h>
#include "sqlitestorage.h"
#include "treeviewstyleditemdelegate.h"
#include "treemodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IStorage *storage = new SQLiteStorage("system.db");
    auto *model = new TreeModel();
    model->loadFromStorage(storage);

    QTreeView t_tree_view;
    auto style = new TreeViewStyle();
    t_tree_view.setStyle(style);
    t_tree_view.setItemDelegate(new TreeViewStyledItemDelegate());
    t_tree_view.setModel(model);
    t_tree_view.setHeaderHidden(true);
    t_tree_view.show();
    return a.exec();
}
