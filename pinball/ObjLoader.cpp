#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pbge/pbge.h"
#include "ObjLoader.h"

#define ObjLoaderCheck(pointer) if(pointer==NULL){printf(OBJECT_ERROR_2); exit(-1);}

/*Função de uso interno. Recebe um linha de texto apontada por buffer e um ponteiro para uma Object3D e faz um parse da linha armazenando as informações na Object3D. Devolve o ponteiro para a Object3D atualizado*/ 
Object3d * readVertexData(char * buffer, Object3d * obj, int * index){
    float x, y , z;
    sscanf(buffer, "v %f %f %f", &x, &y, &z);
    obj->vertex[*index].x = x;
    obj->vertex[*index].y = y;
    obj->vertex[*index].z = z;
    (*index)++;
    return obj;
}

/* Lê a partir do buffer as informações sobre os indices das normais, vertices e coordenadas de textura da face*/
Object3d * readFaceData(char * buffer, Object3d * obj){
    int coord1, norm1, coord2, norm2, coord3, norm3, tex1, tex2, tex3;
    sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d",&coord1, &tex1, &norm1, &coord2, &tex2, &norm2, &coord3, &tex3, &norm3);
    obj->vertexIndex.push_back(coord1 - 1);
    obj->vertexIndex.push_back(coord2 - 1);
    obj->vertexIndex.push_back(coord3 - 1);
    obj->normalIndex.push_back(norm1 - 1);
    obj->normalIndex.push_back(norm2 - 1);
    obj->normalIndex.push_back(norm3 - 1);
    obj->texCoordIndex.push_back(tex1 - 1);
    obj->texCoordIndex.push_back(tex2 - 1);
    obj->texCoordIndex.push_back(tex3 - 1);
    return obj;
}

/*Análogo ao readVertexData*/
Object3d * readNormalData(char * buffer, Object3d * obj, int * index){
    float xn, yn, zn;
    sscanf(buffer, "vn %f %f %f", &xn, &yn, &zn);
    obj->normals[*index].x = xn;
    obj->normals[*index].y = yn;
    obj->normals[*index].z = zn;
    (*index)++;
    return obj;
}

/*Funciona de forma semelhante ao readVertexData porém lê somente as duas primeiras coordenadas da textura.*/
Object3d * readTextureData(char * buffer, Object3d * obj, int * index){
    GLfloat s, t;
    /*tratamento de texturas 2D*/
    sscanf(buffer, "vt %f %f", &s, &t);
    obj->texCoord[*index].s = s;
    obj->texCoord[*index].t = t;
    (*index)++;
    return obj;
}

/* Abre um arquivo de imagem e o carrega como uma textura utilizável pelo openGL*/
GLuint loadTexture(char * fileName){
/*    GLuint texture;
    SDL_Surface * texSurf;
    GLenum format; 
    GLint nColors;
    texSurf = SDL_LoadBMP(fileName);
    if(texSurf == NULL){
    	printf("nao foi possivel carregar a textura %s\n", fileName);
    	exit(-1);
   	}
    nColors = texSurf->format->BytesPerPixel;
    if(nColors == 4)
        format = GL_RGBA;
    else if(nColors == 3)
        format = GL_BGR;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_LockSurface(texSurf);
    glTexImage2D(GL_TEXTURE_2D, 0, nColors, texSurf->w, texSurf->h, 0, format, GL_UNSIGNED_BYTE, texSurf->pixels);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    SDL_UnlockSurface(texSurf);
    SDL_FreeSurface(texSurf);
    return texture;*/
    return NULL;
}

pbge::ModelInstance * createInstance(Object3d * o3dnew, pbge::GraphicAPI * gfx, int vertexIndex, int normalIndex, int texCoordIndex) {
    pbge::VertexBufferBuilder builder(vertexIndex);
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    pbge::VertexAttribBuilder normal = builder.addAttrib(3, pbge::VertexAttrib::NORMAL);
    pbge::VertexAttribBuilder texture = builder.addAttrib(2, pbge::VertexAttrib::CUSTOM_ATTRIB);
    for(int i = 0; i < vertexIndex; i++) {
        builder.pushValue(vertex, o3dnew->vertex[i].x, o3dnew->vertex[i].y, o3dnew->vertex[i].z);
    }
    for(int i = 0; i < normalIndex; i++) {
        builder.pushValue(normal, o3dnew->normals[i].x, o3dnew->normals[i].y, o3dnew->normals[i].z);
    }
    for(int i = 0; i < texCoordIndex; i++) {
        builder.pushValue(texture, o3dnew->texCoord[i].s, o3dnew->texCoord[i].t);
    }
    builder.setAttribIndex(vertex, o3dnew->vertexIndex);
    builder.setAttribIndex(normal, o3dnew->normalIndex);
    builder.setAttribIndex(texture, o3dnew->texCoordIndex);
    pbge::VertexBuffer * vbo = builder.done(pbge::Buffer::STATIC_DRAW, gfx);
    pbge::ModelInstance * instance = new pbge::ModelInstance(new pbge::VBOModel(vbo, GL_TRIANGLES));
    return instance;
}

pbge::Node * LoadObject(char * filename, pbge::GraphicAPI * gfx){
    pbge::Node * parent = pbge::TransformationNode::translation(0,0,0);

    int numVertex = 0, numFaces = 0, numNormal = 0, numTexCoord = 0, numTextures = 0;
    char buffer[100];
    char textureFilename[100];
    int vertexIndex = 0, faceIndex = 0, normalIndex = 0, texCoordIndex = 0, textureIndex = -1;
    Object3d * O3dnew;
    FILE * objectFile = fopen(filename, "r");
    if(objectFile == NULL){
        printf(OBJECT_ERROR_1, filename);
        return NULL;
    }
    O3dnew = (Object3d *) malloc(sizeof(Object3d));
    ObjLoaderCheck(O3dnew);
    /*conta os números de texturas, vértices, normais, faces e coordenadas de textura utilizadas pelo modelo*/
    while(fgets(buffer, 100, objectFile) != NULL){
        if(strncmp(buffer, "v ", 2) == 0)
            numVertex++;
        else if(strncmp(buffer, "vn ", 3) == 0)
            numNormal++;
        else if(strncmp(buffer, "f ", 2) == 0)
            numFaces++;
        else if(strncmp(buffer, "vt ", 3) == 0)
            numTexCoord++;
        else if(strncmp(buffer, "usemat ", 7) == 0)
            numTextures++;
    }
    rewind(objectFile);
    /*aloca espaço para o modelo*/
    O3dnew->vertex = (VERTEX3D) malloc (sizeof(struct _VERTEX3D) * numVertex);
    ObjLoaderCheck(O3dnew->vertex);
    O3dnew->normals = (VERTEX3D) malloc (sizeof(struct _VERTEX3D) * numNormal);
    ObjLoaderCheck(O3dnew->normals);
    O3dnew->texCoord = (TEXCOORD) malloc (sizeof(struct _TEXCOORD) * numTexCoord);
    ObjLoaderCheck(O3dnew->texCoord);
    O3dnew->textures = (GLuint *) malloc (sizeof(GLuint) * numTextures);
    ObjLoaderCheck(O3dnew->textures);
    while(fgets(buffer, 100, objectFile) != NULL){
        if(strncmp(buffer, "v ", 2) == 0)
            O3dnew = readVertexData(buffer, O3dnew, &vertexIndex);
        else if(strncmp(buffer, "f ", 2) == 0){
            O3dnew = readFaceData(buffer, O3dnew);
        }
        else if(strncmp(buffer, "vn ", 3) == 0)
            O3dnew = readNormalData(buffer, O3dnew, &normalIndex);
        else if(strncmp(buffer, "vt ", 3) == 0)
            O3dnew = readTextureData(buffer, O3dnew, &texCoordIndex);
        else if(strncmp(buffer, "usemat ", 7) == 0){
            pbge::ModelInstance * instance = createInstance(O3dnew, gfx, vertexIndex, normalIndex, texCoordIndex);
            parent->addChild(instance);
    
            sscanf(buffer, "usemat %s", textureFilename);
            textureIndex++;
            O3dnew->textures[textureIndex] = loadTexture(textureFilename);
        }
    }
    fclose(objectFile);
    return parent;
}

void DrawObject(Object3d * obj){
    /*int i, j;
    for(i = 0; i < obj->iNFaces; i++){
        glBindTexture(GL_TEXTURE_2D, obj->textures[obj->faces[i].texIndex]);
        glBegin(GL_TRIANGLES);
        for(j = 0; j < obj->faces[i].iNVert; j++){
            glTexCoord2f(obj->texCoord[obj->faces[i].texCoordIndex[j]].s,
                         obj->texCoord[obj->faces[i].texCoordIndex[j]].t);
            glNormal3f(obj->normals[obj->faces[i].normalIndex[j]].x,
                       obj->normals[obj->faces[i].normalIndex[j]].y,
                       obj->normals[obj->faces[i].normalIndex[j]].z);
            glVertex3f(obj->vertex[obj->faces[i].vertexIndex[j]].x,
                       obj->vertex[obj->faces[i].vertexIndex[j]].y,
                       obj->vertex[obj->faces[i].vertexIndex[j]].z);
        }
        glEnd();
    }*/
}

void DrawWireObject(Object3d * obj){
    /*int i, j;
    for(i = 0; i < obj->iNFaces; i++){
        glBegin(GL_LINE_STRIP);
        for(j = 0; j < obj->faces[i].iNVert; j++){
            glVertex3f(obj->vertex[obj->faces[i].vertexIndex[j]].x,
                       obj->vertex[obj->faces[i].vertexIndex[j]].y,
                       obj->vertex[obj->faces[i].vertexIndex[j]].z);
        }
        glEnd();
    }*/
}

void discardObject(Object3D * obj){
    int nf;
    int i;
    nf = obj->iNFaces;
    for(i = 0; i < obj->iNTextures; i++)
        glDeleteTextures(1, &(obj->textures[i]));
    free(obj->textures);
    free(obj->texCoord);
    free(obj->vertex);
    free(obj->normals);
    free(obj);
}
