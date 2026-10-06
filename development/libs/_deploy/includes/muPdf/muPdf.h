#ifndef muPdf_H
#define muPdf_H


#if defined DLL_EXPORT
#define dll __declspec(dllexport)
#else
#define dll __declspec(dllimport)
#endif
//typedef struct fz_alloc_context fz_alloc_context;
extern "C"
{
	dll int add(int a,int b);
	dll int getNumberOfPagesInFile(char *fileName);
	dll unsigned char *getPixelsInPage(char *fileName,int pageNumber,float zoom,float rotate,int *width,int *height);
}
#endif
