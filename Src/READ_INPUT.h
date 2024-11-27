
#ifndef READ_INPUT_h
#define READ_INPUT_h

#include <stdio.h>
#include <stdbool.h>
#include <fitsio.h>
#include "ALLOCATION.h"
#include "STR.h"

/*--------------------------------------------------------------------------------*/

enum keywordtype {KEYWORD_REQUIRED, KEYWORD_DEFAULT, KEYWORD_OPTIONAL};

/*--------------------------------------------------------------------------------*/

typedef struct Struct_Keywords{

    // the keyword.
    char keyword[Key_Length];

    // the input parameter for the keyword.
    char line[Max_Line_Length];

    // the keyword status.
    bool Set, Required;

}STRUCT_KEYS;


/*--------------------------------------------------------------------------------*/

typedef struct Struct_input{

    // Path to the data, cache, verbose, result files
    char Data_Path[Max_Line_Length], Wav_Path[Max_Line_Length], \
        Result_Path[Max_Line_Length];

    int nx, ny;

    double Lambda0, Icriteria;
  
}STRUCT_INPUT;

/*--------------------------------------------------------------------------------*/

typedef struct Struct_Stokes{

    // number of wavelength points
    int nl;
    // the wavelength
    double *Lambda;
    // input profiles
    float ***prof;
 
}STRUCT_STK;

/*--------------------------------------------------------------------------------*/

#define Par_C 299792458.0

extern float COG(STRUCT_STK *Stk, float *prof, STRUCT_INPUT *Input);

extern int RDINPUT(char Filename[], STRUCT_INPUT *Input);

extern bool FILE_EXIST(char *Filename);

/*--------------------------------------------------------------------------------*/

#endif /* READ_INPUT_h */
