#include "stlfileloader.h"
#include <GL/glu.h>
#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>

using namespace h7_3d;

STLFileLoader::STLFileLoader(QString filename, int ratio) {
  mRatio = ratio;
  model.clear();
  loadStl(filename);
}

STLFileLoader::~STLFileLoader() {}

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

void STLFileLoader::loadTextStl(QString filename) {
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

void STLFileLoader::loadBinaryStl(QString filename) {
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

void STLFileLoader::draw() const{
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
