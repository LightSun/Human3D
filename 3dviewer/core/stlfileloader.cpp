#include "stlfileloader.h"
#include <GL/glu.h>
#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>

#include "GLBatch.h"

using namespace h7_3d;

STLFileLoader2::STLFileLoader2(QString filename, int ratio) {
  mRatio = ratio;
  model.clear();
  loadStl(filename);
}

STLFileLoader2::~STLFileLoader2() {}

void STLFileLoader2::loadStl(QString filename) {
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QByteArray arr;
    arr = file.read(5);
    file.close();
    qDebug() << arr;
    if (arr == "solid") {
      loadTextStl(filename);
    } else {
      loadBinaryStl(filename);
    }
  }else{
      qDebug() << filename << "不存在";
  }
}

void STLFileLoader2::loadTextStl(QString filename) {
  qDebug() << "load text file:" << filename;
  model.clear();
  QList<QVector3D> triangle;
  STLTriangle tSTLTriangle;
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    while (!file.atEnd()) {
      QString line =
          file.readLine().trimmed(); // trimmed去除了开头和结尾的空白字符串
      QStringList words = line.split(' ', QString::SkipEmptyParts);
      if (words[0] == "facet") {
        triangle.clear();
        tSTLTriangle.reset();
        tSTLTriangle.setNormal(words[2].toFloat(), words[3].toFloat(),
                               words[4].toFloat());
      }
      if (words[0] == "vertex") {
        triangle.append(QVector3D(words[1].toFloat(), words[2].toFloat(),
                                  words[3].toFloat()));
      }
      if (words[0] == "endloop") {
        if (triangle.length() == 3) {
          for (size_t i = 0; i < 3; ++i) {
            tSTLTriangle.setVertex(i, triangle[i]);
          }
          model.append(tSTLTriangle);
        }
      }
    }
    file.close();
  }
}

void STLFileLoader2::loadBinaryStl(QString filename) {
  //TODO: 待完成二进制文件解析，二进制文件大小更小，这种解析效率会更高
  qDebug() << "load binary file:" << filename;
  //  std::ifstream file(filename, std::ios::in | std::ios::binary);
  //  if (file) {
  //    char header[80];
  //    char num_triangles[4];
  //    file.read(header, 80);
  //    file.read(num_triangles, 4);
  //    std::string(header);

  //  }
  //  file.close();
}

void STLFileLoader2::draw() const{
  const QList<STLTriangle>& triangles = model;
  glBegin(GL_TRIANGLES); //三角形
  foreach (const STLTriangle& tri, triangles) {
    QVector3D& normal = ((STLTriangle&)tri).getNormal();
    glNormal3f(mRatio * normal.x(), mRatio * normal.y(), mRatio * normal.z());
    for (size_t j = 0; j < 3; ++j) {
      auto& vertex = ((STLTriangle&)tri).getVertex(j);
      glVertex3f(mRatio * vertex.x(), mRatio * vertex.y(), mRatio * vertex.z());
    }
  }
  glEnd();
}

STLTriangle::STLTriangle() { reset(); }

void STLTriangle::setVertex(int index, const QVector3D& point3D) {
  if (!checkVertexIndex(index)) {
    return;
  }
  v[index] = point3D;
}


QVector3D& STLTriangle::getVertex(int index){
  if (!checkVertexIndex(index)) {
     static QVector3D DEFAULT;
     return DEFAULT;
  }
  return v[index];
}

void STLTriangle::setNormal(float nx, float ny, float nz) {
  n = QVector3D(nx, ny, nz);
}

QVector3D& STLTriangle::getNormal() { return n; }

void STLTriangle::reset() {
  n = QVector3D(0.f, 0.f, 0.f);
  for (int i = 0; i < 3; ++i) {
    v[i] = QVector3D(0.f, 0.f, 0.f);
  }
}

bool STLTriangle::checkVertexIndex(int index){
  if (index < 0 || index > 2) {
    qDebug() << "CRITICAL: invalid index provided to STLTriangle::SetVertex()!";
    return false;
  }
  return true;
}
//--------------------------------------
namespace h7_3d {
struct _STLFileLoader_ctx{
    GLBatch batch;
    std::vector<float> normals;
    std::vector<float> vertexs;

    void loadTextStl(QString filename, float ratio) {
      qDebug() << "load text file:" << filename;
      int expect_size = 1024;
      normals.reserve(expect_size * 9);
      vertexs.reserve(expect_size * 9);
      QFile file(filename);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!file.atEnd()) {
                  QString line =
                      file.readLine().trimmed(); // trimmed去除了开头和结尾的空白字符串
                  QStringList words = line.split(' ', QString::SkipEmptyParts);
                  if (words[0] == "facet") {
                      auto xyz = {ratio * words[2].toFloat(),
                                  ratio *words[3].toFloat(),
                                  ratio * words[4].toFloat()};
                      normals.insert(normals.end(), xyz);
                      normals.insert(normals.end(), xyz);
                      normals.insert(normals.end(), xyz);
                  }
                  if (words[0] == "vertex") {
                      vertexs.insert(vertexs.end(), {ratio *words[1].toFloat(),
                                                     ratio *words[2].toFloat(),
                                                     ratio *words[3].toFloat()});
                  }
                  if (words[0] == "endloop") {
                      //check vertex. normals.
                      Q_ASSERT(vertexs.size() % 3 == 0);
                      //Q_ASSERT(vertexs.size() / 3 == normals.size());
                  }
            }
            file.close();
      }else{
          Q_ASSERT_X(false, "STLFileLoader::loadTextStl", "open fale failed.");
      }
    }

    void initOnGL(Base3dWidget* w){
        batch.ptr_func = w;
        batch.Begin(GL_TRIANGLES, vertexs.size() / 9);
        batch.CopyVertexData3f(vertexs.data());
        batch.CopyNormalDataf(normals.data());
        batch.End();
    }

    void draw(){
        batch.Draw();
    }
};
}

STLFileLoader::STLFileLoader(QString filename, int ratio){
    setRatio(ratio);
    loadStl(filename);
}
STLFileLoader::STLFileLoader(){
    m_ptr = new _STLFileLoader_ctx();
}
STLFileLoader::~STLFileLoader(){
    if(m_ptr){
        delete m_ptr;
        m_ptr = nullptr;
    }
}
void STLFileLoader::loadTextStl(QString filename) {
    m_ptr->loadTextStl(filename, m_ratio);
}
void STLFileLoader::loadBinaryStl(QString filename) {
    Q_ASSERT_X(false, "loadBinaryStl", "binary not impl");
}
void STLFileLoader::loadStl(QString filename) {
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QByteArray arr;
    arr = file.read(5);
    file.close();
    qDebug() << arr;
    if (arr == "solid") {
      loadTextStl(filename);
    } else {
      loadBinaryStl(filename);
    }
  }else{
      qDebug() << filename << "不存在";
  }
}
void STLFileLoader::draw()const{
    m_ptr->draw();
}
void STLFileLoader::onInitGL(Base3dWidget* w){
    m_ptr->initOnGL(w);
}
