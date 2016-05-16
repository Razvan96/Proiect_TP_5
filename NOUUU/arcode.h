
#ifndef _ARCODE_H_
#define _ARCODE_H_

#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

 /* encode inFile */
int ArEncodeFile(FILE *inFile, FILE *outFile, char staticModel);

/* decode inFile*/
int ArDecodeFile(FILE *inFile, FILE *outFile, char staticModel);

#endif  /* ndef _ARCODE_H_ */
