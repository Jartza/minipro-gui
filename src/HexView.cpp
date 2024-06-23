#include "HexView.h"

//Constructor implementation
HexView::HexView(QObject *parent) : QAbstractTableModel(parent) {
  clearHexTable();
}

int HexView::rowCount(const QModelIndex &) const {
  return table.size();
}

int HexView::columnCount(const QModelIndex &) const {
  return table.at(0).size();
}

QVariant HexView::data(const QModelIndex &index, int role) const {
  switch (role) {
    case Qt::DisplayRole: {
      return table.at(index.row()).at(index.column());
    }
    case Qt::TextAlignmentRole: {
      return Qt::AlignCenter;
    }
    case Qt::FontRole: {
      QFont monospace_font;
      monospace_font.setFamily("Courier New");
      monospace_font.setStyleHint(QFont::Monospace);
      return monospace_font;
    }
    default:break;
  }

  return QVariant();
}

QVariant HexView::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal && section == 0) {
    return "00";
  } else if (orientation == Qt::Horizontal && section == 1) {
    return "01";
  } else if (orientation == Qt::Horizontal && section == 2) {
    return "02";
  } else if (orientation == Qt::Horizontal && section == 3) {
    return "03";
  } else if (orientation == Qt::Horizontal && section == 4) {
    return "04";
  } else if (orientation == Qt::Horizontal && section == 5) {
    return "05";
  } else if (orientation == Qt::Horizontal && section == 6) {
    return "06";
  } else if (orientation == Qt::Horizontal && section == 7) {
    return "07";
  } else if (orientation == Qt::Horizontal && section == 8) {
    return "08";
  } else if (orientation == Qt::Horizontal && section == 9) {
    return "09";
  } else if (orientation == Qt::Horizontal && section == 10) {
    return "0A";
  } else if (orientation == Qt::Horizontal && section == 11) {
    return "0B";
  } else if (orientation == Qt::Horizontal && section == 12) {
    return "0C";
  } else if (orientation == Qt::Horizontal && section == 13) {
    return "0D";
  } else if (orientation == Qt::Horizontal && section == 14) {
    return "0E";
  } else if (orientation == Qt::Horizontal && section == 15) {
    return "0F";
  } else if (orientation == Qt::Horizontal && section == 16) {
    return "ASCII";
  }

  if (orientation == Qt::Vertical) {
    return QVariant::fromValue(section + 1);
  }
  return QVariant();
}

void HexView::clearHexTable() {
  beginResetModel();
  table.clear();
  const int number_of_rows = 8;
  // Create 8 lines of blank data to display
  for (int i = 0; i < number_of_rows; i++) {
    table.append({"--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--",
                  "................"});
  }
  endResetModel();
}

void HexView::buildHexTable(QString temp_file_content) {
  beginResetModel();
  table.clear();

  QString ascii_string_line;
  QVector<QVector<QString>> new_hex_table;

  const int chars_per_line = 32; // (16 bytes * 2)
  int line_counter = 0;
  QVector<QString> next_row;

  for (int i = 0; i <= temp_file_content.length(); i++) {

    // Parse out 1-byte hex values, and convert to ASCII
    if (i % 2 != 0) {
      QString byte_string = QChar(temp_file_content[i - 1]);
      byte_string += QChar(temp_file_content[i]);

      next_row.append(byte_string.toUpper());

      auto ascii_int = byte_string.toUInt(nullptr, 16);
      // Non-extended ASCII only
      if (ascii_int > 126) ascii_int = 46;
      auto ascii_char = QChar(ascii_int);
      // Printable characters only
      if (!ascii_char.isPrint() || ascii_char.isNull()) {
        ascii_char = '.';
      }
      ascii_string_line += ascii_char;
    }
    // Add ASCII character translation, and move on to next 16-byte line
    if (i != 0 && i % chars_per_line == 0) {
      next_row.append(ascii_string_line);
      new_hex_table.append(next_row);
      ascii_string_line.clear();
      line_counter++;
    }
  }
  table = new_hex_table;
  endResetModel();
}

//void refresh_table_view(){
//  emit layoutChanged();
//}