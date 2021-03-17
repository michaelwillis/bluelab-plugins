#include "BLUtilsFile.h"

char *
BLUtilsFile::GetFileExtension(const char *fileName)
{
	char *ext = (char *)strrchr(fileName, '.');
    
    // Here, we have for example ".wav"
    if (ext != NULL)
    {
        if (strlen(ext) > 0)
            // Skip the dot
            ext = &ext[1];
    }
        
    return ext;
}

char *
BLUtilsFile::GetFileName(const char *path)
{
	char *fileName = (char *)strrchr(path, '/');

	// Here, we have for example "/file.wav"
	if (fileName != NULL)
	{
		if (strlen(fileName) > 0)
			// Skip the dot
			fileName = &fileName[1];
	}
	else
	{
		// There were no "/" in the path,
		// we already had the correct file name
		return (char *)path;
	}

	return fileName;
}

template <typename FLOAT_TYPE>
void
BLUtilsFile::AppendValuesFile(const char *fileName,
                              const WDL_TypedBuf<FLOAT_TYPE> &values, char delim)
{
    // Compute the file size
    FILE *fileSz = fopen(fileName, "a+");
    if (fileSz == NULL)
        return;
    
    fseek(fileSz, 0L, SEEK_END);
    long size = ftell(fileSz);
    
    fseek(fileSz, 0L, SEEK_SET);
    fclose(fileSz);
    
    // Write
    FILE *file = fopen(fileName, "a+");
    
    for (int i = 0; i < values.GetSize(); i++)
    {
        if ((i == 0) && (size == 0))
            fprintf(file, "%g", values.Get()[i]);
        else
            fprintf(file, "%c%g", delim, values.Get()[i]);
    }
    
    //fprintf(file, "\n");
           
    fclose(file);
}
template void BLUtilsFile::AppendValuesFile(const char *fileName,
                                            const WDL_TypedBuf<float> &values,
                                            char delim);
template void BLUtilsFile::AppendValuesFile(const char *fileName,
                                            const WDL_TypedBuf<double> &values,
                                            char delim);

template <typename FLOAT_TYPE>
void
BLUtilsFile::AppendValuesFileBin(const char *fileName,
                                 const WDL_TypedBuf<FLOAT_TYPE> &values)
{
    // Write
    FILE *file = fopen(fileName, "ab+");
    
    fwrite(values.Get(), sizeof(FLOAT_TYPE), values.GetSize(), file);
    
    fclose(file);
}
template void BLUtilsFile::AppendValuesFileBin(const char *fileName,
                                               const WDL_TypedBuf<float> &values);
template void BLUtilsFile::AppendValuesFileBin(const char *fileName,
                                               const WDL_TypedBuf<double> &values);

template <typename FLOAT_TYPE>
void
BLUtilsFile::AppendValuesFileBinFloat(const char *fileName,
                                      const WDL_TypedBuf<FLOAT_TYPE> &values)
{
    WDL_TypedBuf<float> floatBuf;
    floatBuf.Resize(values.GetSize());
    for (int i = 0; i < floatBuf.GetSize(); i++)
    {
        FLOAT_TYPE val = values.Get()[i];
        floatBuf.Get()[i] = val;
    }
    
    // Write
    FILE *file = fopen(fileName, "ab+");
    
    fwrite(floatBuf.Get(), sizeof(float), floatBuf.GetSize(), file);
    
    fclose(file);
}
template void
BLUtilsFile::AppendValuesFileBinFloat(const char *fileName,
                                      const WDL_TypedBuf<float> &values);
template void
BLUtilsFile::AppendValuesFileBinFloat(const char *fileName,
                                      const WDL_TypedBuf<double> &values);

void *
BLUtilsFile::AppendValuesFileBinFloatInit(const char *fileName)
{
    // Write
    FILE *file = fopen(fileName, "ab+");
 
    return file;
}

template <typename FLOAT_TYPE>
void
BLUtilsFile::AppendValuesFileBinFloat(void *cookie,
                                      const WDL_TypedBuf<FLOAT_TYPE> &values)
{
    // Write
    FILE *file = (FILE *)cookie;
    
    WDL_TypedBuf<float> floatBuf;
    floatBuf.Resize(values.GetSize());
    for (int i = 0; i < floatBuf.GetSize(); i++)
    {
        FLOAT_TYPE val = values.Get()[i];
        floatBuf.Get()[i] = val;
    }
    
    fwrite(floatBuf.Get(), sizeof(float), floatBuf.GetSize(), file);
    
    // TEST
    fflush(file);
}
template void
BLUtilsFile::AppendValuesFileBinFloat(void *cookie,
                                      const WDL_TypedBuf<float> &values);
template void
BLUtilsFile::AppendValuesFileBinFloat(void *cookie,
                                      const WDL_TypedBuf<double> &values);

void
BLUtilsFile::AppendValuesFileBinFloatShutdown(void *cookie)
{
    fclose((FILE *)cookie);
}