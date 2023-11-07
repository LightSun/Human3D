#pragma once

#include <QVector>
#include <QColor>
#include <QMatrix4x4>

namespace h7_3d {

typedef struct _ShaderManager_ctx _ShaderManager_ctx;

enum GLT_SHADER_ATTRIBUTE {
    GLT_ATTRIBUTE_VERTEX = 0,
    GLT_ATTRIBUTE_COLOR,
    GLT_ATTRIBUTE_NORMAL,
    GLT_ATTRIBUTE_TEXTURE0,
    GLT_ATTRIBUTE_TEXTURE1,
    GLT_ATTRIBUTE_TEXTURE2,
    GLT_ATTRIBUTE_TEXTURE3,
    GLT_ATTRIBUTE_LAST
};

enum{
    kShaderT_VERTEX_NORMAL,
    kShaderT_VERTEX,
    kShaderT_VERTEX_COLOR,
};

class ShaderManager
{
public:
    using CMat4 = const QMatrix4x4&;

    ShaderManager();
    ~ShaderManager();

    void setVertexNormalData(const QVector<float>& data);
    void setVertexData(const QVector<float>& data);
    void setColor(const QColor& c);
    void onInitGl();
    void onDraw(CMat4 model, CMat4 view, CMat4 proj);
    void onDestroy();

private:
    _ShaderManager_ctx* m_ptr;
};

}
