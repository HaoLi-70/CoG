
#include "READ_INPUT.h"

/*--------------------------------------------------------------------------------*/

    /*######################################################################
     
      revision log:

        26 Nov. 2024
          --- Update: remove the redundance  (Hao Li)

        28 Jun. 2024
          --- Initial commit (Hao Li)
     
     ######################################################################*/

/*--------------------------------------------------------------------------------*/

static int Get_Keys(STRUCT_KEYS Keywords[], STRUCT_INPUT *Input);

/*--------------------------------------------------------------------------------*/

static int Get_Keys(STRUCT_KEYS Keywords[], STRUCT_INPUT *Input){

/*--------------------------------------------------------------------------------*/

    /*######################################################################
      Purpose:
        Convert the keywords to inversion configuration.
      Record of revisions:
        26 Nov. 2024 (Hao Li)
      Input parameters:
        Keywords, structure with the input configuration.
      Output parameters:
        Input, structure with the input information.
    ######################################################################*/

/*--------------------------------------------------------------------------------*/

    const char *rname = "Get_Keys";
    char *token, parameter[Key_Length], *p;
    int indx, nread, len, i, itmp;
    long len_tot;
    
    indx = 0;
    String_Copy(Input->Data_Path, Keywords[indx].line, \
        strlen(Keywords[indx].line), true);
    fprintf(stderr, "\n path to the data file : %s", \
        Input->Data_Path);

    indx = 1;
    String_Copy(Input->Wav_Path, Keywords[indx].line, \
        strlen(Keywords[indx].line), true);
    fprintf(stderr, "\n path to the wavelength file : %s", \
        Input->Wav_Path);
      
    indx = 2;
    sprintf(Input->Result_Path, Keywords[indx].line);
    fprintf(stderr, "\n path to the result file : %s", \
          Input->Result_Path);

    indx = 3;
    Input->Lambda0 = atof(Keywords[indx].line);
    fprintf(stderr,"\n lambda = %e", Input->Lambda0);
  
    indx = 4;
    Input->Icriteria = atof(Keywords[indx].line);
    fprintf(stderr,"\n criteria = %e", Input->Icriteria);

    return 0;

}

/*--------------------------------------------------------------------------------*/

extern int RDINPUT(char Filename[], STRUCT_INPUT *Input){
  
/*--------------------------------------------------------------------------------*/

    /*######################################################################
      Purpose:
        Read the input.
      Record of revisions:
        26 Nov. 2024 (Hao Li)  
      Input parameters:
        Filename, the path to the input file.
      Output parameters:
        Input, structure with the input information.
    ######################################################################*/

/*--------------------------------------------------------------------------------*/

    const char *rname = "RDINPUT";

    FILE *Fa = fopen(Filename, "r");
    int nKeywords;

    STRUCT_KEYS Keywords[] = {
      {"data_path", "", false, true}, //0
      {"wavelength_path", "", false, true}, //1
      {"result_path", "./res.fits", false, false}, //2
      {"lambda", "", false, true}, //3
      {"Icriteria", "1000.", false, false}, //4
    };

    nKeywords = sizeof(Keywords)/sizeof(STRUCT_KEYS);
    
    char lines[Max_Line_Length], key[Key_Length], *p;
    int len, i;
    long len_tot;
    bool neglect = true;

    while(Read_line(lines, Fa) > 0 ){
      len_tot = strlen(lines);
      len = Indx_Char(lines, '=', 1);
      if(len > 1){
        len_tot -= len;
        p = lines+len;
        String_Copy(key, lines, len-1, 1);
        Trim(key, 3);
        
        neglect = true;
   
        for(i=0; i<nKeywords; i++){

          if(strcmp(key,Keywords[i].keyword)==0){

            String_Copy(Keywords[i].line, p, len_tot, true);
            Trim(Keywords[i].line, 3);
            Keywords[i].Set = true;

            neglect = false;
            break;
          }
        }
      }
    }

    Get_Keys(Keywords, Input);
   
    fclose(Fa);

    return 0;

}

/*--------------------------------------------------------------------------------*/

extern float COG(STRUCT_STK *Stk, float *prof, STRUCT_INPUT *Input){
  
/*--------------------------------------------------------------------------------*/

    /*######################################################################
      Purpose:
        Estimating the Vlos with the center of gravity method
      Record of revisions:
        26 Nov. 2024
      Input parameters:
        Stk, structure with Stokes profiles.
        Input, the input configuration.
    ######################################################################*/

/*--------------------------------------------------------------------------------*/  


    double IMax = prof[0], sumL = 0.0, weight = 0.0, dtmp = 0.0;
    double IMin = IMax;

    int i;

    for(i=1; i<Stk->nl; i++){
      if(IMax < prof[i]){ 
        IMax = prof[i];
      }else if(IMin > prof[i]){
        IMin = prof[i];
      } 
    }

    // if on disk e.g. the intensity is larger than a specified value.
    if(IMax>Input->Icriteria){

      for(i=0; i<Stk->nl; i++){
        sumL += Stk->Lambda[i]*(IMax-prof[i]);
        weight += IMax-prof[i];
      }
      dtmp = sumL/weight;

      return (float)((dtmp-Input->Lambda0) \
          /Input->Lambda0*Par_C/1e3);
    }else{
      return 0.0;
    }

}

/*--------------------------------------------------------------------------------*/  

extern bool FILE_EXIST(char *Filename){

/*--------------------------------------------------------------------------------*/
      
    /*######################################################################
      Purpose:
        check if a file exists.
      Record of revisions:
        23 Apr. 2024.
      Input parameters:
        Filename, name of the file
      Return:
        true or false
    ######################################################################*/

/*--------------------------------------------------------------------------------*/

    FILE *Fa = fopen(Filename, "r");
    
    if(Fa){

      fclose(Fa);

      return true;
    }

    return false;
}
