#pragma once

#include <QList>
#include <QVector3D>

class QOpenGLFunctions_3_3_Core;


namespace h7_3d {
class STLTriangle;
class Base3dWidget;

struct Vec3F{
    float v1, v2, v3;
    Vec3F(){
        v1 = 0;
        v2 = 0;
        v3 = 0;
    }
    Vec3F(float v1, float v2, float v3):v1(v1), v2(v2), v3(v3){
    }
    bool isAllZero()const{
        return v1 == .0 && v2 == .0 && v3 == .0;
    }
    bool isAll(float t)const{
        return v1 == t && v2 == t && v3 == t;
    }
    bool isNotAllZero()const{
        return !isAll(0);
    }
    bool isNotAllOne()const{
        return !isAll(1);
    }
};
typedef struct _STLFileLoader_ctx _STLFileLoader_ctx;
class STLFileLoader {
public:
  STLFileLoader(QString filename, int ratio);
  STLFileLoader();
  ~STLFileLoader();

  void setRatio(float ratio){
      m_ratio = ratio;
  }

  void loadStl(QString filename);
  void loadTextStl(QString filename);
  void loadBinaryStl(QString filename);

  void onInitGL(QOpenGLFunctions_3_3_Core* );
  void draw()const;

  void destroy();
private:
  _STLFileLoader_ctx* m_ptr {nullptr};
  float m_ratio = 1;
};

class STLFileLoader2 {
public:
   //ratio  放大系数（有的3d模型单位很小，通过放大系数将模型放大）
  STLFileLoader2(QString filename, int ratio);
  STLFileLoader2(){}
  ~STLFileLoader2();

  void setRatio(float ratio){
      mRatio = ratio;
  }

  void loadStl(QString filename);
  void loadTextStl(QString filename);
  void loadBinaryStl(QString filename);

  void onInitGL(){}
  //bad performance.
  void draw()const;

private:
  QList<STLTriangle> model;
  float mRatio = 1; //比例系数
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
    Vec3F scale {1, 1, 1};

    void setRgbColor(float* rgb){
        r = rgb[0];
        g = rgb[1];
        b = rgb[2];
    }
    void setRgbColor(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

struct Module3DBatch{
    std::vector<STLFileLoader> loaders;
    float r, g, b, a; //0-1
    Vec3F translate;
    Vec3F rotate;
    Vec3F scale {1, 1, 1};
};

}
