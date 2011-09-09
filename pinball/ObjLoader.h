/** \file ObjLoader.h
 * \brief Module responsible for rendering .obj models
 */

#include <vector>

#include "pbge/pbge.h"

#ifndef _objloader_h
#define _objloader_h
#define OBJECT_ERROR_1  "ERROR could not open file %s\n"
#define OBJECT_ERROR_2  "ERROR could not alloc memory\n"


/*! \struct _VERTEX3D
 * \brief Armazena a coordenada de uma normal ou de um vértice 3D
 */
struct _VERTEX3D{
    float x, y, z;
};

/*! \struct _TEXCOORD
 * \brief armazena as coordenadas de textura 2D
 */
struct _TEXCOORD{
    GLfloat s, t;
};

typedef struct _TEXCOORD * TEXCOORD;
typedef struct _VERTEX3D * VERTEX3D;

/*! \struct _Object3d
 * \brief armazena os dados necessários para se construir um Objeto Tridimensional
 */
typedef struct _Object3d{
    int      iNFaces;
    int      iNVertex;
    int      iNNormals;
    int      iNTextures;
    int      iNTexCoords;
    std::vector<int> vertexIndex;
    std::vector<int> normalIndex;
    std::vector<int> texCoordIndex;
    VERTEX3D vertex;
    VERTEX3D normals;
    TEXCOORD texCoord;
    GLuint   *textures;
}Object3d;

typedef Object3d Object3D;
/*! Carrega um objeto 3d armazenado no arquivo fileName e devolve um ponteiro para uma estrutura Object3D que contém os dados do objeto lido.
 * Suposições feitas:
 * O objeto est� triangularizado, normalizado e texturizado.
 * \param fileName Arquivo .obj alvo
 */
pbge::Node *LoadObject(char *fileName);

/*! Desenha um Objeto carregado por LoadObject
 * \param obj Objeto-alvo
 */
void DrawObject(Object3D *obj);

/*! Desenha um Objeto carregado em wireframe por LoadObject
 * \param obj Objeto-alvo
 */
void DrawWireObject(Object3d * obj);

/*! Recebe um ponteiro para uma estrutura Object3D e desaloca os recursos utilizados por essa estrutura
 * \param obj Objeto-alvo
 */
void discardObject(Object3D * obj);

#endif


