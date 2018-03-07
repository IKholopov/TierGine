#include <AppEntry.h>
#include <GLBaseApp.h>
#include <SimplePipeline.h>
#include <Tensor.h>

class SampleApp : public TierGine::GLBaseApp {
public:
    SampleApp() : GLBaseApp(TierGine::WindowGLFW::Config(1024, 720, "Sample OpenGL App")) {}
protected:
    virtual TG_Status MainLoop() override;

private:
    bool buffersInitialized = false;
    GLuint vao = 0;
    void initializeBuffers();
    void initializePipeline();
};

static TierGine::AppEntry<SampleApp> appEntry;

TG_Status SampleApp::MainLoop()
{
    if( !buffersInitialized ) {
        initializeBuffers();
        assert(buffersInitialized);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
      // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return GLBaseApp::MainLoop();
}

void SampleApp::initializeBuffers()
{
    TierGine::Tensor t = TierGine::CreateTensor(3, 3,
        {    // Polygon
             0.0f, 0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             -0.5f, -0.5f, 0.0f
        });
    float points[] = {
        // Polygon
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        // Color
        1.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
    GLuint vbo = 0;
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, 21 * sizeof(float), points, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &vao);
    int elementsCount = 3;
    GLuint attrPolygon = 0;
    GLuint componentsSizePolygon = 3;
    glEnableVertexArrayAttrib(vao, attrPolygon);
    GLenum componentTypePolygon = GL_FLOAT;
    GLboolean isNormalizedPolygon = GL_FALSE;
    GLuint offsetPolygon = 0;
    glVertexArrayAttribFormat(vao, attrPolygon, componentsSizePolygon, componentTypePolygon,
                         isNormalizedPolygon, offsetPolygon);

    GLuint attrColor = 1;
    GLuint componentsSizeColor = 4;
    glEnableVertexArrayAttrib(vao, attrColor);
    GLenum componentTypeColor = GL_FLOAT;
    GLboolean isNormalizedColor = GL_FALSE;
    GLuint offsetColor = 0;
    glVertexArrayAttribFormat(vao, attrColor, componentsSizeColor, componentTypeColor,
                         isNormalizedColor, offsetColor);
    glVertexArrayAttribBinding(vao, attrPolygon, 0);
    glVertexArrayAttribBinding(vao, attrColor, 1);

    GLuint firstBinding = 0;
    GLsizei bindingsCount = 2;

    GLuint buffers[] = { vbo, vbo };
    GLintptr offsets[] = { 0, static_cast<GLintptr>(elementsCount * componentsSizePolygon * sizeof(float)) };
    GLsizei strides[] = { static_cast<GLsizei>(componentsSizePolygon * sizeof(float)),
                          static_cast<GLsizei>(componentsSizeColor * sizeof(float)) };
    glVertexArrayVertexBuffers(vao, firstBinding, bindingsCount, buffers, offsets, strides );

    initializePipeline();

    buffersInitialized = true;
}

void SampleApp::initializePipeline()
{
    auto context = GetContext();
    assert(context != nullptr);
    TierGine::SimplePipeline pipeline(*context, "./res/shaders/rgb.vert.glsl",
                            "./res/shaders/rgb.frag.glsl");
    pipeline.Activate();
}
