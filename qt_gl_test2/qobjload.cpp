#include "qobjload.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <algorithm>

/**
v 几何体顶点 (Geometric vertices)

vt 贴图坐标点 (Texture vertices)

vn 顶点法线 (Vertex normals)

vp 参数空格顶点 (Parameter space vertices)
f  face
 */
bool D3ModelLoader::loadObj(QString fileName, QVector<float>& vPoints)
{
    if (fileName.mid(fileName.lastIndexOf('.')) != ".obj"
            && fileName.mid(fileName.lastIndexOf('.')) != ".OBJ")
    {
        qDebug() << "file is not a obj file.";
        return false;
    }

    QFile objFile(fileName);
    if (!objFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "open" << fileName << "failed";
        return false;
    }
    else
    {
        qDebug() << "open" << fileName << "success!";
    }

    QVector<float> vertextPoints, texturePoints, normalPoints;
    QVector<int> facesIndexs;
    while (!objFile.atEnd())
    {
        QByteArray lineData = objFile.readLine();

        QList<QByteArray> strValues = lineData.trimmed().split(' ');
        QString dataType = strValues.takeFirst();
        if (dataType == "v")
        {
            std::transform(strValues.begin(), strValues.end(),
                           std::back_inserter(vertextPoints), [](QByteArray& str) {
                return str.toFloat();
            });
        }
        else if (dataType == "vt")
        {
            std::transform(strValues.begin(), strValues.end(),
                           std::back_inserter(texturePoints), [](QByteArray& str) {
                return str.toFloat();
            });
        }
        else if (dataType == "vn")
        {
            std::transform(strValues.begin(), strValues.end(),
                           std::back_inserter(normalPoints), [](QByteArray& str) {
                return str.toFloat();
            });
        }
        else if (dataType == "f")
        {
            facesIndexs << strValues.at(0).toInt()
                        << strValues.at(1).toInt()
                        << strValues.at(2).toInt();
        }
    }
    objFile.close();

    for (auto& verFaceInfo : facesIndexs)
    {
        int vIndex = verFaceInfo - 1;

        vPoints << vertextPoints.at(vIndex * 3);
        vPoints << vertextPoints.at(vIndex * 3 + 1);
        vPoints << vertextPoints.at(vIndex * 3 + 2);
    }

    vertextPoints.clear();
    texturePoints.clear();
    normalPoints.clear();
    facesIndexs.clear();

    return true;
}

bool D3ModelLoader::loadStl(QString filename, QVector<float>& vPoints){
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       while (!file.atEnd()) {
         QString line =
             file.readLine().trimmed(); // trimmed去除了开头和结尾的空白字符串
         QStringList words = line.split(' ', QString::SkipEmptyParts);
         if (words[0] == "facet") {
//           triangle.clear();
//           tSTLTriangle.reset();
//           tSTLTriangle.setNormal(words[2].toFloat(), words[3].toFloat(),
//                                  words[4].toFloat());
         }
         if (words[0] == "vertex") {
//           triangle.append(QVector3D(words[1].toFloat(), words[2].toFloat(),
//                                     words[3].toFloat()));
             vPoints << words[1].toFloat();
             vPoints << words[2].toFloat();
             vPoints << words[3].toFloat();
         }
         if (words[0] == "endloop") {
//           if (triangle.length() == 3) {
//             for (size_t i = 0; i < 3; ++i) {
//               tSTLTriangle.setVertex(i, triangle[i]);
//             }
//             model.append(tSTLTriangle);
//           }
         }
       }
       file.close();
       return true;
     }
    qDebug() << "open" << filename << "failed";
    return false;
}
