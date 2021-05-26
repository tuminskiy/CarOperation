#pragma once

#include "ui_mainwindow.h"
#include "queriesdialog.hpp"
#include "selectiddialog.hpp"
#include "storage/entitymanager.hpp"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <memory>

namespace carop
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

  using FuncInsert = std::function<bool ()>;
  using FuncShow = std::function<void ()>;
  using FuncRemove = std::function<bool (id_t)>;

public:
  explicit MainWindow(QWidget* parent = nullptr);

  void load_data();

private slots:
  void submit_click();
  void remove_click();

private:
  template <class Container>
  auto func_for_current_tab(const Container& c) -> typename Container::mapped_type
  {
    auto it = c.find(ui_.twTables->currentWidget());

    if (it == c.end()) {
      return {};
    }

    return it->second;
  }

private:
  Ui::MainWindow ui_;

  EntityManager entity_manager_;
  QueriesDialog dqueries_;
  SelectIdDialog dselectid_;
  
  QSqlQueryModel view_model_;

  std::unordered_map<const QWidget*, FuncInsert> widget_func_insert;
  std::unordered_map<const QWidget*, FuncShow>   widget_func_show;
  std::unordered_map<const QWidget*, FuncRemove> widget_func_remove;
};

} // namespace carop