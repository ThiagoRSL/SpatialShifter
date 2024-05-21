
#include "InterfaceManager.h"
#include "TextManager.h"
#include "GLUtils.h"
#include <iostream>

using std::cout;
using std::endl;

TextManager* TextManager::textManagerInstance = nullptr;


TextManager* TextManager::GetInstance()
{
    if (!textManagerInstance)
    {
        textManagerInstance = new TextManager();
    }


    return textManagerInstance;
}
TextManager::TextManager()
{
    this->InterfaceProjection = InterfaceManager::GetInstance()->GetInterfaceProjectionRef();
    TextShader = new Shader();
    printf("\n COMPILANDO SHADERS TEXT.");
    TextShader->Compile("shader/interface/text.vert", "shader/interface/text.frag");
    mat4 interfaceProj = *this->InterfaceProjection;
    TextShader->setUniform(string("InterfaceProjectionMatrix"), interfaceProj);

    this->Init();

    VboID = new GLuint[1];

    glGenVertexArrays(1, &VaoID);
    glBindVertexArray(VaoID);

    glGenBuffers(1, VboID);
    glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // Essa linha aqui
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //ShaderManager::GetInstance()->AddShader(1, ShaderType::SHADER_TYPE_TEXT, TextShader);
}

TextManager::~TextManager()
{

}

void TextManager::Init()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }
    std::cout << "FREE TYPE INICIADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "C://Windows//Fonts//L_10646.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    std::cout << "FONTE CARREGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    //The function sets the font's width and height parameters. 
    //Setting the width to 0 lets the face dynamically calculate the width based on the given height.
    FT_Set_Pixel_Sizes(face, 0, 36);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return;
    }
    std::cout << "FONTE CARREGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        //TODO: ver se resolveu.
        //TextureManager::Inst()->ReserveIndex();
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        TextCharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        TextCharacters.insert(std::pair<char, TextCharacter>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    //If we use a single byte to represent the colors of a texture we do need to take care of a restriction of OpenGL :
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //uniform mat4 ProjectionMatrix;
    //uniform sampler2D text;
    //uniform vec3 textColor;
    printf("\n\nGLIFOS CARREGADOS\n\n");
}

void TextManager::RenderText(const TextElement* te)
{

    //TextShader = ShaderManager::GetInstance()->GetShader(1, ShaderType::SHADER_TYPE_TEXT);
    float x = te->Position.x; 
    float y = te->Position.y;
    float lineSpacing = te->LineSpacing;
    float lineHeight = 40.0f;
    float scale = te->Scale;

    // activate corresponding render state	
    TextShader->Use();
    TextShader->setUniform(string("TextColor"), te->TextColor);
    glActiveTexture(GL_TEXTURE0);
    TextShader->setUniform(string("text"), 0);
    glBindVertexArray(VaoID);

    // iterate through all characters

    int lineCount = 0;
    float xpos = x + te->MarginLeft;
    float ypos = y + te->MarginTop;
    std::string::const_iterator c;
    std::string::const_iterator c_next;
    TextCharacter ch;
    TextCharacter ch_next;
    for (c = te->Text.begin(); c != te->Text.end(); c++)
    {
        ch = TextCharacters[*c];

        //Tamanho do caracter
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        if (c + 1 < te->Text.end())
        {
            c_next = c + 1;
            ch_next = TextCharacters[*c_next];
            //Se o próximo caracter estourar a linha, quebra ela.
            if ((xpos + w + ch_next.Bearing.x * scale) > x + te->LineWidthMax)
            {
                //Fazer tratamento para adiciona - em frases quebradas.
                //Fazer pro *c == '\n'
                lineCount += 1;
                xpos = x + te->MarginLeft;                            // Retoma pro inicio X
                ypos = y - (lineHeight * lineCount + te->MarginTop);  // Desloca o Y (próxima linha)
            }
        }

        //Altera os valores onde vai escrever (depende do caractere), o y pode ser deslocado um pouco baseado no tipo de letra
        //Letras como 'q'e 'p' tem um offset de altura.
        xpos = xpos + ch.Bearing.x * scale;
        ypos = ypos - (ch.Size.y - ch.Bearing.y) * scale;

        // Atualiza o VBO
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        //Devolve o offset que tirou (pra letras igual p e q)
        ypos += (ch.Size.y - ch.Bearing.y) * scale;
        //Define a textura do glyph (representante visual do caractere) à ser renderizado
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        //Atualiza o VBO na memória
        glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        xpos += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}