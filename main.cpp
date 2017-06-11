#include <stdio.h>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <glad/glad.h>

extern "C" void GetWindowSize(LONG* width, LONG* height);

static float GetWindowAspect()
{
    LONG w, h;
    GetWindowSize(&w, &h);

    return (float)w / (float)h;
}

static double GetTime()
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

//    if(start_time.to == 0)
//        start_time = std::chrono::high_resolution_clock::now();
    return 0.;
}

#define PersistStore GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT

static const float buffers[] = {
    -0.3, 0.5, -0.8,
    -0.8, -0.5, -0.8,
     0.2, -0.5, -0.8
};

GLuint buffer_handles[1];
GLuint vertex_arrays[1];

#ifndef NDEBUG
void debug_callback(GLenum source,
                    GLenum type,
                    GLuint id,
                    GLenum severity,
                    GLsizei length,
                    const GLchar *message,
                    const void *userParam)
{
    printf("GL: %s\n", message);
}
#endif

extern "C" void Init()
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(debug_callback, nullptr);
#endif

    glCreateBuffers(1, buffer_handles);
    glCreateVertexArrays(1, vertex_arrays);

    glNamedBufferStorage(buffer_handles[0], sizeof(buffers), buffers, PersistStore | GL_MAP_READ_BIT);

    glVertexArrayAttribFormat(vertex_arrays[0], 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vertex_arrays[0], 0, buffer_handles[0], 0, sizeof(float) * 3);
    glVertexArrayAttribBinding(vertex_arrays[0], 0, 0);
    glEnableVertexArrayAttrib(vertex_arrays[0], 0);

    float* ptr = (float*)glMapNamedBufferRange(buffer_handles[0], 0, sizeof(buffers), PersistStore | GL_MAP_READ_BIT);

    for(int i=0;i<sizeof(buffers)/4;i++)
    {
        printf("%f ", ptr[i]);
    }
    printf("\n");

    GetWindowAspect();
}

double last = 0.;

extern "C" void Update()
{
    glClearColor(0.8, 0., 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(GetTime() > last)
    {
        printf("%f\n", GetTime());
        fflush(stdout);
        last = GetTime() + 1.;
    }

    glBindVertexArray(vertex_arrays[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
