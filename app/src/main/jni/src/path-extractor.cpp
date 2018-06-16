//
// Created by eltonjiang on 2018/6/14.
//

#include "path-extractor.h"
#include "fthelper.h"


FreeTypeLibrary::FreeTypeLibrary()
{
    error = FT_Init_FreeType(&m_ftLibrary);
    checkFT_Error(error, "init library");
}

FreeTypeLibrary::~FreeTypeLibrary()
{
    FT_Done_FreeType(m_ftLibrary);
}


FreeTypeFace::FreeTypeFace(const FreeTypeLibrary &library, const char *filename)
{
    error = FT_New_Face(library.m_ftLibrary, filename, 0, &m_ftFace);
    checkFT_Error(error, "new face");
}

FreeTypeFace::~FreeTypeFace()
{
    if (!error)
        FT_Done_Face(m_ftFace);
}


PathExtractor::PathExtractor(const char *filename)
        : m_face(m_library, filename)
{

}

bool PathExtractor::isValid()
{
    return m_face.error == 0 && m_library.error == 0;
}


void PathExtractor::setTextSize(int textSize)
{
    FT_Error error;
    error = FT_Set_Pixel_Sizes(m_face.m_ftFace, 0, static_cast<FT_UInt>(textSize));
    checkFT_Error(error, "set text size");
}

FT_Size_Metrics PathExtractor::getMetrics()
{
    return m_face.m_ftFace->size->metrics;
}

bool PathExtractor::extractPath(wchar_t wChar, JPath *jPath, FT_BBox *box)
{
    if (!loadGlyph(wChar))return false;

    if (!checkOutline())return false;

    flipOutline();

    return decomposeOutline(jPath, box);
}


bool PathExtractor::loadGlyph(wchar_t wChar)
{
    FT_Error error;

    FT_UInt glyph_index = FT_Get_Char_Index(m_face.m_ftFace, static_cast<FT_ULong>(wChar));
    error = FT_Load_Glyph(m_face.m_ftFace, glyph_index, FT_LOAD_DEFAULT);
    checkFT_Error(error, "load glyph");

    return error == 0;
}

bool PathExtractor::checkOutline()
{
    FT_Face face = m_face.m_ftFace;
    FT_GlyphSlot slot = face->glyph;
    FT_Outline &outline = slot->outline;

    if (slot->format != FT_GLYPH_FORMAT_OUTLINE)
        return false; // Should never happen.  Just an extra check.

    if (outline.n_contours <= 0 || outline.n_points <= 0)
        return false; // Can happen for some font files.

    FT_Error error = FT_Outline_Check(&outline);
    return error == 0;
}

void PathExtractor::flipOutline()
{
    const FT_Fixed multiplier = 65536L;

    FT_Matrix matrix;

    matrix.xx = 1L * multiplier;
    matrix.xy = 0L * multiplier;
    matrix.yx = 0L * multiplier;
    matrix.yy = -1L * multiplier;

    FT_Face face = m_face.m_ftFace;
    FT_GlyphSlot slot = face->glyph;
    FT_Outline &outline = slot->outline;

    FT_Outline_Transform(&outline, &matrix);
}

bool PathExtractor::decomposeOutline(JPath *jPath, FT_BBox *box)
{
    FT_Outline_Funcs callbacks;

    callbacks.move_to = MoveToFunction;
    callbacks.line_to = LineToFunction;
    callbacks.conic_to = ConicToFunction;
    callbacks.cubic_to = CubicToFunction;

    callbacks.shift = 0;
    callbacks.delta = 0;

    FT_Face face = m_face.m_ftFace;
    FT_GlyphSlot slot = face->glyph;
    FT_Outline &outline = slot->outline;

    FT_Error error = FT_Outline_Decompose(&outline, &callbacks, jPath);
    checkFT_Error(error, "decompose outline");
    FT_Outline_Get_BBox(&outline, box);
    return error == 0;
}


int PathExtractor::MoveToFunction(const FT_Vector *to, void *user)
{
    JPath *jPath = static_cast<JPath *>(user);

    FT_Pos x = to->x;
    FT_Pos y = to->y;

    jclass pathClass = jPath->env->GetObjectClass(jPath->path);
    jmethodID method = jPath->env->GetMethodID(pathClass, "moveTo", "(FF)V");
    jPath->env->CallVoidMethod(jPath->path, method, (float) x, (float) y);
    return 0;
}

int PathExtractor::LineToFunction(const FT_Vector *to, void *user)
{
    JPath *jPath = static_cast<JPath *>(user);

    FT_Pos x = to->x;
    FT_Pos y = to->y;

    jclass pathClass = jPath->env->GetObjectClass(jPath->path);
    jmethodID method = jPath->env->GetMethodID(pathClass, "lineTo", "(FF)V");
    jPath->env->CallVoidMethod(jPath->path, method, (float) x, (float) y);
    return 0;
}

int PathExtractor::ConicToFunction(const FT_Vector *control, const FT_Vector *to, void *user)
{
    JPath *jPath = static_cast<JPath *>(user);

    FT_Pos controlX = control->x;
    FT_Pos controlY = control->y;

    FT_Pos x = to->x;
    FT_Pos y = to->y;

    jclass pathClass = jPath->env->GetObjectClass(jPath->path);
    jmethodID method = jPath->env->GetMethodID(pathClass, "quadTo", "(FFFF)V");
    jPath->env->CallVoidMethod(jPath->path, method, (float) controlX, (float) controlY,
            (float) x, (float) y);
    return 0;
}

int PathExtractor::CubicToFunction(const FT_Vector *controlOne, const FT_Vector *controlTwo,
                                   const FT_Vector *to, void *user)
{
    JPath *jPath = static_cast<JPath *>(user);

    FT_Pos controlOneX = controlOne->x;
    FT_Pos controlOneY = controlOne->y;

    FT_Pos controlTwoX = controlTwo->x;
    FT_Pos controlTwoY = controlTwo->y;

    FT_Pos x = to->x;
    FT_Pos y = to->y;

    jclass pathClass = jPath->env->GetObjectClass(jPath->path);
    jmethodID method = jPath->env->GetMethodID(pathClass, "cubicTo", "(FFFFFF)V");
    jPath->env->CallVoidMethod(jPath->path, method, (float) controlOneX, (float) controlOneY,
            (float) controlTwoX, (float) controlTwoY, (float) x, (float) y);
    return 0;
}
