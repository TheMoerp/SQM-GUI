#ifndef SQMTABLEMODEL_H
#define SQMTABLEMODEL_H
#include <QAbstractTableModel>
#include <vector>

using namespace std;

class SQMTableModel : public QAbstractTableModel {
        Q_OBJECT
public:
    SQMTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void SetStartValues(int pBase, int pExp, int pMod);

private:
    std::string IntToBinary(int n);
    void CalculateSqmMatrix(int startRow);

    int base;
    int exp;
    int mod;
    int binLen;

    vector<vector <int>> sqmMatrix;
};

#endif // SQMTABLEMODEL_H
