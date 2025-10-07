// HexView.cpp
#include "HexView.h"
#include <QStringView>

static constexpr int kBytesPerRow = 16;
static constexpr int kAsciiCol    = 16; // 0..15 hex, 16 = ASCII

HexView::HexView(QObject *parent) : QAbstractTableModel(parent) {
    m_mono.setFamily("Courier New");    // or "Monaco" on macOS; Qt will substitute if missing
    m_mono.setStyleHint(QFont::Monospace);
}

int HexView::rowCount(const QModelIndex &) const {
    if (m_bytes.isEmpty()) return 0;
    return (m_bytes.size() + kBytesPerRow - 1) / kBytesPerRow;
}

int HexView::columnCount(const QModelIndex &) const {
    return kBytesPerRow + 1; // 16 hex bytes + ASCII strip
}

QVariant HexView::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};

    const int row = index.row();
    const int col = index.column();

    switch (role) {
    case Qt::FontRole:
        return m_mono;

    case Qt::TextAlignmentRole:
        return col == kAsciiCol ? QVariant{Qt::AlignLeft | Qt::AlignVCenter}
                                : QVariant{Qt::AlignCenter};

    case Qt::DisplayRole: {
        const int start = row * kBytesPerRow;
        if (col == kAsciiCol) {
            // ASCII strip for the whole row
            QString ascii;
            ascii.reserve(kBytesPerRow);
            for (int i = 0; i < kBytesPerRow; ++i) {
                const int idx = start + i;
                if (idx >= m_bytes.size()) break;
                unsigned char c = static_cast<unsigned char>(m_bytes[idx]);
                if (c < 32 || c > 126) c = '.';
                ascii.append(QChar(c));
            }
            return ascii;
        } else {
            const int idx = start + col;
            if (idx >= m_bytes.size()) return QVariant{};
            unsigned char b = static_cast<unsigned char>(m_bytes[idx]);
            // fast 2-char hex without allocations beyond the QString result
            return QString::asprintf("%02X", b);
        }
    }
    default:
        return {};
    }
}

QVariant HexView::headerData(int section, Qt::Orientation o, int role) const {
    if (role != Qt::DisplayRole) return {};
    if (o == Qt::Horizontal) {
        if (section < kBytesPerRow) return QString::asprintf("%02X", section);
        if (section == kAsciiCol)   return "ASCII";
        return {};
    } else {
        // Show byte offset at row start
        const int offset = section * kBytesPerRow;
        return QString::asprintf("%08X", offset);
    }
}

Qt::ItemFlags HexView::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void HexView::setBytes(QByteArray bytes) {
    beginResetModel();
    m_bytes = std::move(bytes);
    endResetModel();
}

void HexView::clear() {
    beginResetModel();
    m_bytes.clear();
    endResetModel();
}

// HexView.cpp (add at end)

void HexView::buildHexTable(const QByteArray &bytes) {
    setBytes(bytes);
}

void HexView::buildHexTable(const QString &hexText) {
    // Accept either plain hex or hex with spaces/newlines; QByteArray::fromHex ignores whitespace.
    QByteArray ba = QByteArray::fromHex(hexText.toUtf8());
    setBytes(std::move(ba));
}

void HexView::clearHexTable() {
    clear();
}
