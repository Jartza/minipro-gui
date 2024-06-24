#ifndef MINIPRO_GUI_SRC_HEXVIEW_H_
#define MINIPRO_GUI_SRC_HEXVIEW_H_

#include <QTime>
#include <QAbstractTableModel>
#include <QFont>

class HexView : public QAbstractTableModel {
 Q_OBJECT
 public:
  QFont monospace_font;

  explicit HexView(QObject *parent = nullptr);
  int rowCount(const QModelIndex & = QModelIndex()) const override;
  int columnCount(const QModelIndex & = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  void clearHexTable();
  void buildHexTable(QString);

 private:
  QVector<QVector<QString>> table;

};

#endif //MINIPRO_GUI_SRC_HEXVIEW_H_
