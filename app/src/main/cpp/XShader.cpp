//
// Created by Chia-Hao on 2018/8/27.
//

#include "XShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>
#include <vector>

#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; // Vertex Coord
        attribute vec2 aTexCoord; // Vertex's Texture Coord
        varying vec2 vTexCoord;   // to Fragment shader, vertex texture coord
        void main(){
            vTexCoord = vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position = aPosition;
        }
);

static const char *fragYUV420P = GET_STR(
        precision mediump float;    // Set Precision
        varying vec2 vTexCoord;     // from vertex shader, vertex texture position
        uniform sampler2D yTexture; // Texture sampler（single pixel）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            // Output
            gl_FragColor = vec4(rgb,1.0);
        }
);

static GLuint InitShader(const char* code, GLint type)
{
    GLuint sh = glCreateShader(static_cast<GLenum>(type));

    glShaderSource(sh, 1, &code, 0);

    glCompileShader(sh);

    GLint success = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if( success == GL_FALSE )
    {
        XLOGE("GL_COMPILE_STATUS: %d", success);
        return 0;
    }
    XLOGI("glCompileShader success!");
    return sh;
}

void XShader::Close()
{   
    mutex.lock();
    // Release Shader
    if( program )
    {
        glDeleteProgram(program);
    }
    if( vsh )
    {
        glDeleteShader(vsh);
    }
    if( fsh )
    {
        glDeleteShader(fsh);
    }

    // Release Texture
    for( int i=0; i<sizeof(texts)/sizeof(unsigned int); i++ )
    {
        if( texts[i] )
        {
            glDeleteTextures(1, &texts[i]);
        }
        texts[i] = 0;
    }
    mutex.unlock();
}

bool XShader::Init( XSHADER_TYPE type )
{   
    Close();
    mutex.lock();
    vsh = InitShader( vertexShader, GL_VERTEX_SHADER );
    if( vsh == 0 ) {
        mutex.unlock();
        XLOGE("InitShader vertexShader Failed.");
        return false;
    }
    XLOGI("InitShader vertexShader Succeeded.");

    fsh = InitShader( fragYUV420P, GL_FRAGMENT_SHADER );
    if( fsh == 0 ) {
        mutex.unlock();
        XLOGE("InitShader fragYUV420P Failed.");
        return false;
    }
    XLOGI("InitShader fragYUV420P Succeeded.");

    /* Attach Shader */
    program = glCreateProgram();
    if(program == 0)
    {   
        mutex.unlock();
        XLOGE("glCreateProgram Failed.");
        return false;
    }
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    /* Link the program */
    glLinkProgram(program);

    /* Check link status */
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if( isLinked == GL_FALSE  ) 
    {   
        mutex.unlock();
        XLOGE("glGetProgramiv failed");
        GLint maxLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);
        std::vector<GLchar> infoLog(maxLen, 0);
        glGetProgramInfoLog(program, maxLen, &maxLen, &(infoLog[0]));

        glDeleteProgram(program);
        glDeleteShader(vsh);
        glDeleteShader(fsh);

        return false;
    }

    glUseProgram(program);
    XLOGE("glUseProgram Succeeded");

    /* Vertex Data */
    static float vers[] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f
    };

    /* Get attribute from shader, only in vertex shader */
    GLint apos = glGetAttribLocation(program, "aPosition");
    /* Enable shader can read data from GPU(CPU) */
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);

    /* Texture Data */
    static float txts[] = {
            1.0f, 0.0f,
            0.0f,0.0f,
            1.0f,1.0f,
            0.0f,1.0f
    };
    GLint aText = glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(aText);
    glVertexAttribPointer(aText, 2, GL_FLOAT, GL_FALSE, 8, txts);

    /* Get uniform from shader and set texture sampler
     * specify the value of a uniform variable for the current program object */
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0);
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1);
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2);

    mutex.unlock();
    XLOGI("XShader Init Succeeded!");

    return true;
}

void XShader::Draw()
{   
    mutex.lock();
    if( !program ) {
        mutex.unlock();
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mutex.unlock();
}

void XShader::GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool is_alpha)
{
    /* Write to buffer */
    unsigned int format = GL_LUMINANCE;
    //if( isa ) format = GL_LUMINANCE_ALPHA;
    mutex.lock();
    if( texts[index] == 0 ) {
        glGenTextures(1, &texts[index]);

        glBindTexture(GL_TEXTURE_2D, texts[index]); /*the following operation if only for texts[0]*/
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER/*Specifies the symbolic name of a single-valued texture parameter.*/,
                        GL_LINEAR/*the weighted average of the four texture elements that are closest to the specified texture coordinates*/);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL/*Specifies a pointer to the image data in memory.*/);

    }

    /* Activate 0 texture */
    glActiveTexture(GL_TEXTURE0+index);
    /* Bind to texts[0] */
    glBindTexture(GL_TEXTURE_2D, texts[index]);
    /* Substitute content */
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, buf);


    mutex.unlock();
}


