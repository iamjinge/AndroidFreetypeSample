//
// Created by eltonjiang on 2018/6/14.
//

#ifndef FONTDECODE_PATH_EXTRACTOR_H
#define FONTDECODE_PATH_EXTRACTOR_H

#include <jni.h>
#include "common.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

struct FreeTypeLibrary
{
    FreeTypeLibrary();

    ~FreeTypeLibrary();

    FT_Library m_ftLibrary;

    FT_Error error;
};


struct FreeTypeFace
{
    FreeTypeFace(const FreeTypeLibrary &library, const char *filename);

    ~FreeTypeFace();

    FT_Face m_ftFace;

    FT_Error error;
};


typedef struct
{
    JNIEnv *env;
    jobject path;
} JPath;


class PathExtractor
{

public:
    PathExtractor(const char *filename);

    bool isValid();

    void setTextSize(int textSize);

    FT_Size_Metrics getMetrics();

    bool extractPath(wchar_t wChar, JPath *jPath, FT_BBox *box);

    static int MoveToFunction(const FT_Vector *to, void *user);

    static int LineToFunction(const FT_Vector *to, void *user);

    static int ConicToFunction(const FT_Vector *control, const FT_Vector *to, void *user);

    static int CubicToFunction(const FT_Vector *controlOne, const FT_Vector *controlTwo,
                               const FT_Vector *to, void *user);

private:
    bool loadGlyph(wchar_t wChar);

    bool checkOutline();

    void flipOutline();

    bool decomposeOutline(JPath *jPath, FT_BBox *box);

private:
    FreeTypeLibrary m_library;
    FreeTypeFace m_face;
};

#endif //FONTDECODE_PATH_EXTRACTOR_H
