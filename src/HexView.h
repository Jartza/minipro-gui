// HexView.h
#pragma once
#include <QAbstractTableModel>
#include <QByteArray>
#include <QFont>

class HexView : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit HexView(QObject *parent = nullptr);

    // Model API
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Load bytes (raw, not hex text)
    void setBytes(QByteArray bytes);
    void clear();

// HexView.h (additions)
public slots:
    // legacy API compatibility
    void buildHexTable(const QByteArray &bytes);     // old code may pass raw bytes
    void buildHexTable(const QString &hexText);      // old code may pass hex-as-text
    void clearHexTable();

private:
    QByteArray m_bytes;
    QFont m_mono;
};
