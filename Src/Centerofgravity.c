
#include <stdio.h>
#include "ALLOCATION.h"
#include "TIME_PRINT.h"
#include "READ_INPUT.h"
#include "IO.h"
#include "STR.h"

/*--------------------------------------------------------------------------------*/

    /*######################################################################
     
      revision log:

        27 Nov. 2024
          --- Update: remove the redundance  (Hao Li)
          
        28 Jun. 2024
          --- Initial commit (Hao Li)
     
     ######################################################################*/

/*--------------------------------------------------------------------------------*/

#define Input_Path "./input"

/*--------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
  
/*--------------------------------------------------------------------------------*/

    /*######################################################################
      Purpose:
        Compute the line of sight velocity via center of gravity method.
    ######################################################################*/

/*--------------------------------------------------------------------------------*/

    Time_Print();
    STRUCT_INPUT *Input = (STRUCT_INPUT *)malloc(sizeof(STRUCT_INPUT));

    char *Filename = Input_Path;
    if(argc>=2){
      Filename = argv[1];
    }

    if(!FILE_EXIST(Filename)) exit(1);
    
    // read the input configuration.
    RDINPUT(Filename, Input);

    STRUCT_STK *Stk = (STRUCT_STK *)malloc(sizeof(STRUCT_STK));

    // read the wavelength points. 
    rprofile(Input, Stk);
 
    int ix, iy, status;

    float **vlos = (float **)MATRIX(0, Input->ny-1, 0, \
        Input->nx-1, enum_flt, false);

    for(ix=0;ix<Input->nx;ix++){
      for(iy=0;iy<Input->ny;iy++){
        vlos[iy][ix] = COG(Stk, Stk->prof[iy][ix], Input);
      }
    }

    // pointer to the fits file.
    fitsfile *fptr_res;
    char filename[Max_Line_Length+1];
    sprintf(filename, "!%s",Input->Result_Path);
    fprintf(stderr, "%s\n",Input->Result_Path);
    fits_create_file(&(fptr_res), filename, &status);

    int naxis = 2;
    long naxes[2], fpixel[2];
    naxes[0] = Input->nx;
    naxes[1] = Input->ny;
    fpixel[0] = 1;
    fpixel[1] = 1;

    fits_create_img(fptr_res, FLOAT_IMG, naxis, naxes, &status);
        
    fits_write_pix(fptr_res, TFLOAT, fpixel, Input->nx*Input->ny, \
              vlos[0], &status);
 
    fits_close_file(fptr_res, &status);

    Time_Print();

    return 0;
    
}

/*--------------------------------------------------------------------------------*/
