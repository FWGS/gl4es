#ifndef _GL4ES_BUFFERS_H_
#define _GL4ES_BUFFERS_H_

#include "khash.h"
#include "../config.h"
#include "gles.h"

// VBO *****************
typedef struct {
    GLuint      buffer;
    GLuint      real_buffer;
    GLenum      type;
    GLsizeiptr  size;
    GLenum      usage;
    GLint		access;
    int			mapped;
    int         ranged;
    GLintptr    offset;
    GLsizeiptr  length;
    GLvoid     *data;
} glbuffer_t;

KHASH_MAP_DECLARE_INT(buff, glbuffer_t *);

void gl4es_glGenBuffers(GLsizei n, GLuint * buffers);
void gl4es_glBindBuffer(GLenum target, GLuint buffer);
void gl4es_glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
void gl4es_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
void gl4es_glDeleteBuffers(GLsizei n, const GLuint * buffers);
GLboolean gl4es_glIsBuffer(GLuint buffer);
void gl4es_glGetBufferParameteriv(GLenum target, GLenum value, GLint * data);

void *gl4es_glMapBuffer(GLenum target, GLenum access);
GLboolean gl4es_glUnmapBuffer(GLenum target);
void gl4es_glGetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params);
void gl4es_glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);

void *gl4es_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void gl4es_glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);

void glGenBuffers(GLsizei n, GLuint * buffers);
void glBindBuffer(GLenum target, GLuint buffer);
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
void glDeleteBuffers(GLsizei n, const GLuint * buffers);
GLboolean glIsBuffer(GLuint buffer);
void glGetBufferParameteriv(GLenum target, GLenum value, GLint * data);
void *glMapBuffer(GLenum target, GLenum access);
GLboolean glUnmapBuffer(GLenum target);
void glGetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params);
void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);

void *glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);

void glGenBuffersARB(GLsizei n, GLuint * buffers);
void glBindBufferARB(GLenum target, GLuint buffer);
void glBufferDataARB(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
void glBufferSubDataARB(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
void glDeleteBuffersARB(GLsizei n, const GLuint * buffers);
GLboolean glIsBufferARB(GLuint buffer);
void glGetBufferParameterivARB(GLenum target, GLenum value, GLint * data);
void *glMapBufferARB(GLenum target, GLenum access);
GLboolean glUnmapBufferARB(GLenum target);
void glGetBufferSubDataARB(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);
void glGetBufferPointervARB(GLenum target, GLenum pname, GLvoid ** params);

// Pointer..... ****** => map them in vertexattrib (even with GLES1.1). So no more pointer_state_t, use vertexattrib_t
// and map .enabled to .vaarray

// This map correspond to ARB_vertex_program map
typedef enum {
    ATT_VERTEX = 0,
    //ATT_WEIGHT
    ATT_NORMAL = 2,
    ATT_COLOR,
    ATT_SECONDARY,
    ATT_FOGCOORD,
    // 6 and 7 are nothing specifics
    ATT_MULTITEXCOORD0 = 8,
    ATT_MULTITEXCOORD1,
    ATT_MULTITEXCOORD2,
    ATT_MULTITEXCOORD3,
    ATT_MULTITEXCOORD4,
    ATT_MULTITEXCOORD5,
    ATT_MULTITEXCOORD6,
    ATT_MULTITEXCOORD7,
    ATT_MULTITEXCOORD8,
    ATT_MULTITEXCOORD9,
    ATT_MULTITEXCOORD10,
    ATT_MULTITEXCOORD11,
    ATT_MULTITEXCOORD12,
    ATT_MULTITEXCOORD13,
    ATT_MULTITEXCOORD14,
    ATT_MULTITEXCOORD15,
    //ATT_POINTSIZE,   //this one is supported by GLES hardware
    ATT_MAX
} reserved_attrib_t;

#define NB_VA (ATT_MAX)
// glLockArrays should lock all arrays, but Quake3 do change values on Color and Textures UV, so limit Lock to 3D" Coords arrays
#define NB_LOCKVA (ATT_NORMAL+1)

// Vertex Attrib.. ***
typedef struct {
    GLint           size;
    GLenum          type;
    GLsizei         stride;
    const GLvoid*   pointer;
    GLboolean       enabled;
    GLboolean       normalized;
    glbuffer_t      *buffer;    // reference buffer
    GLfloat         current[4];
    GLint           divisor;
    GLuint          real_buffer;    // If there is a real VBO binded
    const GLvoid*   real_pointer;   // the pointer related to real VBO
} vertexattrib_t;

typedef struct {
    GLfloat *ptr;
    vertexattrib_t state;
    GLboolean enabled;
} pointer_cache_t;

// VAO ****************
typedef struct {
    GLuint           array;
    // buffer state
    glbuffer_t *vertex;
    glbuffer_t *elements;
    glbuffer_t *pack;
    glbuffer_t *unpack;
    // locked arrays
    int       locked;
    GLsizei   count;
    GLint     first;
    GLboolean locked_mapped[NB_VA];

    int maxtex; // upper limit enabled tex_coord
    // VAO optimisation: keep a shared copy of the digested datas (unless the vao is the default one)
    int *shared_arrays;
    pointer_cache_t vert;
    pointer_cache_t normal;
    pointer_cache_t color;
    pointer_cache_t secondary;
    pointer_cache_t fog;
    pointer_cache_t tex[MAX_TEX];
    int cache_count;
    // Vertex Attrib
    vertexattrib_t  vertexattrib[MAX_VATTRIB];
    // TODO: Cache VA also?
} glvao_t;

void VaoSharedClear(glvao_t *vao);
void VaoInit(glvao_t *vao);

KHASH_MAP_DECLARE_INT(glvao, glvao_t*);

void gl4es_glGenVertexArrays(GLsizei n, GLuint *arrays);
void gl4es_glBindVertexArray(GLuint array);
void gl4es_glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
GLboolean gl4es_glIsVertexArray(GLuint array);

void glGenVertexArrays(GLsizei n, GLuint *arrays);
void glBindVertexArray(GLuint array);
void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
GLboolean glIsVertexArray(GLuint array);

void gl4es_glNamedBufferData(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
void gl4es_glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
void gl4es_glGetNamedBufferParameteriv(GLuint buffer, GLenum value, GLint * data);
void* gl4es_glMapNamedBuffer(GLuint buffer, GLenum access);
GLboolean gl4es_glUnmapNamedBuffer(GLuint buffer);
void gl4es_glGetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data);
void gl4es_glGetNamedBufferPointerv(GLuint buffer, GLenum pname, GLvoid ** params);

#endif // _GL4ES_BUFFERS_H_
