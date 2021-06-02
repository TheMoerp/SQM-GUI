#include "sqmtablemodel.h"
#include <QColor>
#include <QBrush>


SQMTableModel::SQMTableModel(QObject *parent)
    : QAbstractTableModel(parent),
    binLen(0) {

}


int SQMTableModel::rowCount(const QModelIndex & /*parent*/) const {
    return binLen;
}

int SQMTableModel::columnCount(const QModelIndex & /*parent*/) const {
    return 3;
}

QVariant SQMTableModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();

    QVariant result;
    switch (role) {
    case Qt::DisplayRole:

        // Tries to get data for current index
        try {
            result = QString::number(sqmMatrix.at(col).at(row));
        } catch (...) {
            result = "x";
        }

        break;
    case Qt::ForegroundRole:
        // ChangedHere index is set and Highlighting is on
        if (changedHere.isValid() && highlightChanged) {
            // Does the current index has to be highlighted
            if (((row > changedHere.row()) ||
                 ((col >= changedHere.column()) && (row == changedHere.row()))) &&
                ((col != 0) || ((changedHere.column() == 0) && (changedHere.row() == row)))) {

                result = QColor(Qt::darkRed);
            }
        }
        break;
    case Qt::TextAlignmentRole:
        // Everything is aligned right
        result = Qt::AlignRight;
        break;
    case Qt::BackgroundRole:
        // Result field is transparent red
        if ((row == binLen - 1) && (col == 2)) {
            result = QBrush(QColor(255, 0, 0, 50));
            break;
        }
    }
    return result;

}

QVariant SQMTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    QVariant result;
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                result = QString("BIN");
                break;
            case 1:
                result = QString("SQN");
                break;
            case 2:
                result = QString("MUL");
                break;
            }
        }
        return result;
}


bool SQMTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        if (!checkIndex(index)) {
            return false;
        }
        int row = index.row();
        int col = index.column();

        // Do not change values in BIN column to anything but 0 or 1
        if (col == 0 && value.toInt() != 0 && value.toInt() != 1)  {
            return false;
        }

        // Call calculateSqmMatrix
        sqmMatrix.at(col).at(row) = value.toInt();

        // Changed index (sometimes col 0 will not get highlighted)
        if (changedHere.isValid() && highlightChanged) {
            if (((row <= changedHere.row()) && (col <= changedHere.column())) ||
                 (changedHere.column() == 0 && row <= changedHere.row()) ||
                 (changedHere.column() == 1 && (row < changedHere.row() || col == 0))){

                changedHere = index;
            }
        }
        else {
            changedHere = index;
        }
        highlightChanged = true;

        // Update sqmMatrix
        UpdateSqmMatrix(index);
        return true;
    }
    return false;
}


Qt::ItemFlags SQMTableModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}



void SQMTableModel::SetStartValues(int pBase, int pExp, int pMod) {
    // Init start values
    base = pBase;
    exp = pExp;
    mod = pMod;

    // Nothing gets highlighted
    highlightChanged = false;
    // Calculate sqmMatrix with start values
    CalculateSqmMatrix();
}

void SQMTableModel::CalculateSqmMatrix() {
    // Clear sqmMatrix
    sqmMatrix.clear();

    // Exp to bin vector
    vector<int> colBin;
    binLen = 0;
    while ((1 << binLen) <= exp) {
        colBin.insert(colBin.begin(), ((exp >> binLen) & 1));
        binLen += 1;
    }
    // If exp not set use 0 as exp
    if (binLen == 0) {
        binLen = 1;
        colBin.push_back(0);
    }

    // Add or remove rows
    beginResetModel();
    sqmMatrix.push_back(colBin);
    endResetModel();

    // Init SQN & MUL Column
    vector<int> colSqn, colMul;
    colSqn.push_back(1 % mod);
    colMul.push_back(base % mod);
    sqmMatrix.push_back(colSqn);
    sqmMatrix.push_back(colMul);


    // Calculate SQM
    for (int i = 1; i < binLen; i++) {
        // SQN
        sqmMatrix.at(1).push_back((sqmMatrix.at(2).at(i - 1) * sqmMatrix.at(2).at(i - 1)) % mod);

        // MUL
        if (sqmMatrix.at(0).at(i) == 0) {
            sqmMatrix.at(2).push_back(sqmMatrix.at(1).at(i));
        }
        else {
            sqmMatrix.at(2).push_back((sqmMatrix.at(1).at(i) * base) % mod);
        }
    }
}


// Can be merged with CalculateSqmMatrix
void SQMTableModel::UpdateSqmMatrix(QModelIndex startIndex) {
    int start_row = startIndex.row();
    int start_col = startIndex.column();

    // If change appeared in col 2, start in next row
    if (start_col == 2) {
        start_row++;
    }

    // Update sqmMatrix
    for (int i = start_row; i < binLen; i++) {
        // If change appeared in 0,0 do not calculate SQN as usuall
        if (start_row == 0 && start_col == 0) {
            sqmMatrix.at(1).at(i) = 1 % mod;
            start_row = 1;
        }
        // If change appeared in col 1, skip calculation for this row in col 1. Otherwise it would get overwritten
        else if (start_col != 1) {
            // MUL
            sqmMatrix.at(1).at(i) = (sqmMatrix.at(2).at(i - 1) * sqmMatrix.at(2).at(i - 1)) % mod;
        }
        start_col = 0;

        // MUL
        if (sqmMatrix.at(0).at(i) == 0) {
            sqmMatrix.at(2).at(i) = sqmMatrix.at(1).at(i);
        }
        else {
            sqmMatrix.at(2).at(i) = (sqmMatrix.at(1).at(i) * base) % mod;
        }
    }
}
