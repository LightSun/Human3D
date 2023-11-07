#ifndef QOBJLOAD_H
#define QOBJLOAD_H

#include <QVector>
#include <QString>

class D3ModelLoader
{
public:
    D3ModelLoader() {}
    ~D3ModelLoader() {}

    bool loadObj(QString fileName, QVector<float>& vPoints);
    bool loadStl(QString fileName, QVector<float>& vPoints);
};

#endif // QOBJLOAD_H
