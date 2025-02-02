#include "Shader.h"
#include "../manager/InputManager.h"

Shader::Shader()
{
    return;
}

//ComputeShader
void Shader::Compile(const char* computePath)
{
    // 1. retrieve the compute source code from filePath
    std::string computeCode;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open file
        cShaderFile.open(computePath);
        std::stringstream cShaderStream;
        // read file's buffer contents into streams
        cShaderStream << cShaderFile.rdbuf();
        // close file handler
        cShaderFile.close();
        // convert stream into string
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* cShaderCode = computeCode.c_str();

    // 2. compile shader
    unsigned int compute;
    int success;
    char infoLog[512];

    // compute Shader
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    // print compile errors if any
    glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(compute, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(1);
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, compute);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shader as it's linked into our program now and no longer necessary
    glDeleteShader(compute);
}


// Vertex e Fragment shader
void Shader::Compile(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::Compile(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        gShaderFile.open(geometryPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, gShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, geometry, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    // print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::GEOMETRY::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };


    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

void Shader::setUniform(const std::string& name, bool value)
{
    this->Use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setUniform(const std::string& name, int value)
{
    this->Use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniform(const std::string& name, float value)
{
    this->Use();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniform(const std::string& name, double value)
{
    this->Use();
    glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float x, float y)
{
    this->Use();
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform2f(loc, x, y);
}
void Shader::setUniform(const std::string& name, float x, float y, float z)
{
    this->Use();
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform3f(loc, x, y, z);
}
void Shader::setUniform(const std::string& name, float x, float y, float z, float w)
{
    this->Use();
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform4f(loc, x, y, z, w);
}


void Shader::setUniform(const std::string& name, const vec2& v)
{
    this->Use();
    this->setUniform(name, v.x, v.y);
}
void Shader::setUniform(const std::string& name, const vec3& v)
{
    this->Use();
    this->setUniform(name, v.x, v.y, v.z);
}
void Shader::setUniform(const std::string& name, const vec4& v)
{
    this->Use();
    this->setUniform(name, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const std::string& name, const mat4& m)
{
    this->Use();
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const std::string& name, const mat3& m)
{
    this->Use();
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}
void Shader::setUniform(const std::string& name, GLuint value)
{
    this->Use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::GenerateBuffers()
{
    glGenVertexArrays(1, &VaoID);
    glBindVertexArray(VaoID);
    VboID = new GLuint[5];
    glGenBuffers(5, VboID);
    glBindVertexArray(0);

    // Definindo as cores dos vértices.
    for (int i = 0; i <= Indexes.size(); i++)
    {
        Colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    glBindVertexArray(VaoID);

    //Habilita transparência
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
    glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
    glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(vec2), (GLvoid*)&Uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(1);  // UVs -> layout 1 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, VboID[2]);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(vec3), (GLvoid*)&Normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(2);  // Normals -> layout 2 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, VboID[3]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(vec4), (GLvoid*)&Colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(3);  // Colors -> layout 3 in the VS

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Shader::GenerateMouseBuffers()
{
    glGenVertexArrays(1, &VaoID);
    glBindVertexArray(VaoID);
    VboID = new GLuint[3];
    glGenBuffers(3, VboID);
    glBindVertexArray(0);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //Os dois por height por que quero um quadrado
    vec2 screenDelta = vec2(48.0f / InputManager::GetInstance()->GetWindowSize().x, 48.0f / InputManager::GetInstance()->GetWindowSize().y);


    Vertexes.clear();
    Uvs.clear();
    Indexes.clear();

    Vertexes.push_back(vec3(-screenDelta.x, -screenDelta.y, 0.0f));
    Vertexes.push_back(vec3(-screenDelta.x, screenDelta.y, 0.0f));
    Vertexes.push_back(vec3(screenDelta.x, -screenDelta.y, 0.0f));
    Vertexes.push_back(vec3(screenDelta.x, screenDelta.y, 0.0f));

    Uvs.push_back(vec2(0.0, 0.0));
    Uvs.push_back(vec2(0.0, 1.0));
    Uvs.push_back(vec2(1.0, 0.0));
    Uvs.push_back(vec2(1.0, 1.0));

    Indexes.push_back(0);
    Indexes.push_back(1);
    Indexes.push_back(2);
    Indexes.push_back(3);

    glBindVertexArray(VaoID);

    //Habilita transparência
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
    glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
    glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(vec2), (GLvoid*)&Uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(1);  // TexCoord -> layout 2 in the VS

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    this->setUniform("TextureSampler", 0);
}

void Shader::Use()
{
    //Verificações
    glUseProgram(ID);
}

void Shader::Render()
{
    this->Use();
    glBindVertexArray(VaoID);
    glDrawArrays(GL_TRIANGLES, 0, Indexes.size());
    glBindVertexArray(0);
    glUseProgram(0);
}

void Shader::RenderMouse() // Classe Própria ShaderMouse
{
    this->Use();
    glBindVertexArray(VaoID);
    glDrawElements(GL_TRIANGLE_STRIP, Indexes.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
    glBindVertexArray(0);
}

void Shader::RenderColliderDebug()
{
    this->Use();
    glBindVertexArray(VaoID);
    glDrawElements(GL_LINES, Indexes.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Shader::RenderParticles(GLuint id, int size)
{
    this->Use();
    glBindVertexArray(id);
    glDrawArrays(GL_POINTS, 0, size);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Shader::GenerateSkillBuffers()
{
    glGenVertexArrays(1, &VaoID);
    glBindVertexArray(VaoID);
    VboID = new GLuint[3];
    glGenBuffers(3, VboID);
    glBindVertexArray(0);

    // Definindo as cores dos vértices.
    for (int i = 0; i <= Indexes.size(); i++)
    {
        Colors.push_back(GlobalColors::DEVOURING_SINGULARITY_COLOR);
    }

    glBindVertexArray(VaoID);

    //Habilita transparência
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
    glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(vec4), (GLvoid*)&Colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(1);  // Colors -> layout 3 in the VS

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Shader::GenerateColliderBuffers()
{
    int i;
    for (i = 0; i < Vertexes.size(); i++)
    {
        Vertexes.push_back(Vertexes.at(i));
        Colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    for (i = 0; i < Edges.size(); i++)
    {
        std::pair<unsigned int, unsigned int> pair = Edges.at(i);
        Edges.push_back(pair);
        Indexes.push_back(pair.first - 1);
        Indexes.push_back(pair.second - 1);
    }
    Colors.push_back(vec4(1.0f, 0.0f, 1.0f, 1.0f));

    this->Use();
    glGenVertexArrays(1, &VaoID);
    glBindVertexArray(VaoID);

    unsigned int handle[3];
    glGenBuffers(3, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(vec4), (GLvoid*)&Colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(1);  // Colors -> layout 0 in the VS

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

    //Remove a referência atual VAO 
    glBindVertexArray(0);
}
