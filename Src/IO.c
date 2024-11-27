
#include "IO.h"

/*--------------------------------------------------------------------------------*/

    /*######################################################################
     
      revision log:

        27 Nov. 2024
          --- Update: remove the redundance  (Hao Li)

        28 Jun. 2024
          --- Initial commit (Hao Li)

     ######################################################################*/

/*--------------------------------------------------------------------------------*/

extern int rprofile(STRUCT_INPUT *Input, STRUCT_STK *Stk){

/*--------------------------------------------------------------------------------*/

    /*######################################################################
      Purpose:
        read the wavelength.
      Record of revisions:
        27 Nov. 2024 (Hao Li)
      Input parameters:
        Input, the input configuration.
        Stk, structure with Stokes profiles.
      Output parameters:
        Stk, structure with Stokes profiles.
    ######################################################################*/

/*--------------------------------------------------------------------------------*/

    const char *rname = "rWavelength";

    // CFITSIO status value MUST be initialized to zero!
    int status = 0;

    // he bit of each pixel, the number of axes
    int bitpix, naxis;
    
    // the length of each axis, the first pixel
    long naxes[4], fpixel[4];

    fpixel[0] = 1;
    fpixel[1] = 1;
    fpixel[2] = 1;
    fpixel[3] = 1;
    
    // pointer to the fits file.
    fitsfile *fptr_data, *fptr_wav;

    fits_open_file(&(fptr_data), Input->Data_Path, \
        READONLY, &status);

    fits_open_file(&(fptr_wav), Input->Wav_Path, \
        READONLY, &status);

    // get the dimension of the data hdu
    fits_get_img_dim(fptr_data, &(naxis), &status);

    if(naxis!=4){ 
      fprintf(stderr,"dimension error in the data file. %d\n", \
          naxis);
      exit(1);      
    }

    // get the size of each dimension 
    fits_get_img_size(fptr_data, naxis, naxes, &status);

    if(naxes[1]!=4){
      fprintf(stderr,"size error in the data file. %d\n", \
          naxes[1]);
      exit(1);              
    }

    Stk->nl = naxes[0];
    Input->nx = naxes[2];
    Input->ny = naxes[3];
 
    // get the type of the hdu
    fits_get_img_type(fptr_data, &bitpix, &status);

    Stk->prof = (float ***)TENSOR_FLT(0, Input->ny-1, 
        0, Input->nx-1, 0, 4*Stk->nl-1, false);

    if(bitpix==-32){
      fits_read_pix(fptr_data, TFLOAT, fpixel, \
          Stk->nl*4*Input->nx*Input->ny, NULL, Stk->prof[0][0], \
          NULL, &status); 
          
    }else{
      exit(1);   
    }
      
    // get the dimension of the wavlength hdu
    fits_get_img_dim(fptr_wav, &naxis, &status);

    if(status!=0) exit(1);        

    if(naxis!=1) exit(1);   

    // get the size of the wavelength 
    fits_get_img_size(fptr_wav, naxis, naxes, &status);
    if(status!=0) exit(1);        

    if(Stk->nl != naxes[0]) exit(1);              

    // get the type of the hdu
    fits_get_img_type(fptr_wav, &bitpix, &status);

    if(status!=0){
      exit(1);        
    }

    Stk->Lambda = (double *)malloc(Stk->nl*sizeof(double));

    if(bitpix==-64){
      fits_read_pix(fptr_wav, TDOUBLE, fpixel, Stk->nl, NULL, \
          Stk->Lambda, NULL, &status);        
    }else{
      exit(1); 
    }

    if(status != 0) exit(1);        

    fits_close_file(fptr_data, &status);

    if(status != 0) exit(1);        

    fits_close_file(fptr_wav, &status);

    if(status != 0) exit(1);        

    return 0;
}

/*--------------------------------------------------------------------------------*/
