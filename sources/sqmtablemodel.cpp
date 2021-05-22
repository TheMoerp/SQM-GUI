#include "sqmtablemodel.h"


SQMTableModel::SQMTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
}

QVariant SQMTableModel::data(const QModelIndex &index, int role) const {

    if (role == Qt::DisplayRole) {
        int row = index.row();
        int col = index.column();

        return sqmMatrix.at(col).at(row);
    }
    return QVariant();
}

void SQMTableModel::SetStartValues(int pBase, int pExp, int pMod) {
    base = pBase;
    exp = pExp;
    mod = pMod;
}


std::string SQMTableModel::IntToBinary(int n) {
    string bin;
    int mask = 1;
    while (n != 0) {
        bin += (n & mask) == 0 ? "0" : "1";
        n = n >> 1;
    }

    return bin;
}

void SQMTableModel::CalculateSqmMatrix(int startRow) {
    string bin = IntToBinary(exp);
    int binLen = bin.length();

    // Initialize bin column
    vector<int> colBin;
    for (int i = 0; i < binLen; i++) {
        colBin.push_back(bin[i] - '0');
    }
    sqmMatrix.at(0) = colBin;

    for (int i = startRow; i < binLen; i++) {
        if (i == 0) {
            // Initialize first row
            sqmMatrix.at(1).at(0) = 1;
            sqmMatrix.at(2).at(0) = base;
        }
        else {
            // Square Multiply
            sqmMatrix.at(1).at(i) = sqmMatrix.at(2).at(i - 1) * sqmMatrix.at(2).at(i - 1);

            if (sqmMatrix.at(0).at(i) == 0) {
                sqmMatrix.at(2).at(i) = sqmMatrix.at(1).at(i);
            }
            else {
                sqmMatrix.at(2).at(i) = sqmMatrix.at(1).at(i) * base;
            }
        }
    }
}
