#pragma once

#include <QList>
#include <QVector3D>


namespace h7_3d {
class STLTriangle;

struct Vec3F{
    float v1, v2, v3;
    Vec3F(){
        v1 = 0;
        v2 = 0;
        v3 = 0;
    }
    bool isAllZero()const{
        return v1 == .0 && v2 == .0 && v3 == .0;
    }
    bool isNotAllZero()const{
        return !isAllZero();
    }
};

class STLFileLoader {
public:

   //ratio  放大系数（有的3d模型单位很小，通过放大系数将模型放大）
  STLFileLoader(QString filename, int ratio);
  ~STLFileLoader();

  void draw()const;

private:

  void loadStl(QString filename);
  void loadTextStl(QString filename);
  void loadBinaryStl(QString filename);

private:
  QList<STLTriangle> model;
  int mRatio = 1; //比例系数
};

//stl format: three vertex + 3 normals
class STLTriangle {
public:
  STLTriangle();

  void setVertex(int index, const QVector3D& point3D);

  QVector3D& getVertex(int index);

  void setNormal(float nx, float ny, float nz);

  QVector3D& getNormal();

  void reset();

private:
  bool checkVertexIndex(int index);

private:
  QVector3D v[3]; //三角顶点坐标
  QVector3D n;    //三角形法向量的三个分量
};

struct Module3D{
    STLFileLoader loader;
    float r, g, b, a; //0-1
    Vec3F translate;
    Vec3F rotate;
    Vec3F scale;
};

struct Module3DBatch{
    std::vector<STLFileLoader> loaders;
    float r, g, b, a; //0-1
    Vec3F translate;
    Vec3F rotate;
    Vec3F scale;
};

}
