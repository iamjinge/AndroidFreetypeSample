#include <jni.h>

#include <ftglyph.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

#include "fthelper.h"
#include "path-extractor.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong
Java_com_iamjinge_freetype_PathExtractor_nInit(JNIEnv *env, jobject, jstring filePath)
{
    PathExtractor *pathExtractor = new PathExtractor(env->GetStringUTFChars(filePath, JNI_FALSE));
    if (pathExtractor->isValid())
    {
        return reinterpret_cast<jlong>(pathExtractor);
    } else
    {
        delete pathExtractor;
        return 0;
    }
}

JNIEXPORT void
Java_com_iamjinge_freetype_PathExtractor_nFinalize(JNIEnv *env, jobject, jlong nPointer)
{
    if (nPointer)
    {
        PathExtractor *pathExtractor = reinterpret_cast<PathExtractor *>(nPointer);
        delete pathExtractor;
    }
}

JNIEXPORT void
Java_com_iamjinge_freetype_PathExtractor_nSetTextSize(
        JNIEnv *env, jobject, jlong nPointer, jint textSize)
{
    if (nPointer)
    {
        PathExtractor *pathExtractor = reinterpret_cast<PathExtractor *>(nPointer);
        pathExtractor->setTextSize(textSize);
    }
}

JNIEXPORT void
Java_com_iamjinge_freetype_PathExtractor_nGetMetrics(
        JNIEnv *env, jobject, jlong nPointer, jobject jMetrics)
{
    if (nPointer)
    {
        PathExtractor *pathExtractor = reinterpret_cast<PathExtractor *>(nPointer);
        FT_Size_Metrics metrics = pathExtractor->getMetrics();
        jclass metricsClass = env->GetObjectClass(jMetrics);
        env->SetFloatField(jMetrics, env->GetFieldID(metricsClass, "height", "F"), metrics.height);
        env->SetFloatField(jMetrics, env->GetFieldID(metricsClass, "ascender", "F"),
                metrics.ascender);
        env->SetFloatField(jMetrics, env->GetFieldID(metricsClass, "descender", "F"),
                metrics.descender);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_iamjinge_freetype_PathExtractor_nExtractPath(
        JNIEnv *env, jobject, jlong nPointer,
        jchar content, jobject path, jintArray boxArray)
{
    if (!nPointer)
    {
        return JNI_FALSE;
    }
    PathExtractor *pathExtractor = reinterpret_cast<PathExtractor *>(nPointer);
    wchar_t wChar = content;

    JPath jPath;
    jPath.env = env;
    jPath.path = path;

    FT_BBox box;

    if (pathExtractor->extractPath(wChar, &jPath, &box))
    {
        LOGI("find %ld, %ld, %ld, %ld", box.xMin, box.xMax, box.yMin, box.yMax);
        env->SetIntArrayRegion(boxArray, 0, 4, reinterpret_cast<const jint *>(&box));
        return JNI_TRUE;
    } else
    {
        LOGI("not find");
        return JNI_FALSE;
    }
}


#ifdef __cplusplus
}
#endif