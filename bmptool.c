#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include"bmplib.h"
/* Humberto Perez PID: 3747565
 * I affirm that I wrote this program myself without the help of others or the internet
 * Description: This program is to read an image and enlarge, flip horizontally, vertically flip and rotate by a factor of 90 degrees 
 */

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation, PIXEL** new, int* newrows, int* newcols)
{
   int row, col;
   if((rows <= 0) || (cols <= 0)) return -1;
   if(rotation == -90 || rotation == 270)
   {
	*newrows = cols;
	*newcols = rows;
	*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
	
	for(row = 0; row < rows; row++)
   	  for(col = 0; col < cols; col++) {
	   PIXEL* o = original + row*cols + col;
	   PIXEL* n = (*new) + (cols-1-col)*rows + row;
	   *n = *o;	
           }
   } else if(rotation == -270 || rotation == 90) 
	  {
	  *newrows = cols;
	  *newcols = rows;
   	  *new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));

	  for(row = 0; row < rows; row++)
	    for(col = 0; col < cols; col++) {
	     PIXEL* o = original + row*cols + col;
	     PIXEL* n = (*new) + col*rows + (rows-1-row);
	     *n = *o;
	     }
          }
	  else if(rotation == 180 || rotation == -180) 
	       {
		*newrows = rows;
		*newcols = cols;
		*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
		
		for(row = 0; row < rows; row++)
	 	  for(col = 0; col < cols; col++) {
		   PIXEL* o = original + row*cols + col;
		   PIXEL* n = (*new)+(rows-row)*cols - (col+1);
		   *n = *o;	
		   }
	        } 
		else  if(rotation == 360 || rotation == 0) 
		      {
		       *newrows = rows;
		       *newcols = cols;
		       *new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
		
	                for(row = 0; row < rows; row++)
		          for(col = 0; col < cols; col++) {
			   PIXEL* o = original + row*cols + col;
			   PIXEL* n = (*new) + row*cols + col;		
		           *n = *o;
			}
		      }
		else printf("Rotation number should be multiple of 90 up to 360\n"); 
   return 0;
}
/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scaler    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scaler=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */	   
int scale(PIXEL* original, int rows, int cols, int scaler, PIXEL** new, int* newrows, int* newcols)
{
   int row, col;
   if((rows <= 0) || (cols <= 0)) return -1;

   (*newrows) = (int)((int)rows*scaler);
   (*newcols) = (int)((int)cols*scaler);

   *new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));

   for (row = 0; row < (*newrows); row++)
      for (col = 0; col < (*newcols); col++) {
       	 (*new)[row * (*newcols) + col] = original[(row * rows / (*newrows)) * cols + col * cols / (*newcols)];  
      }
  
   return 0;
}
/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int vflip (PIXEL *original, PIXEL **new, int rows, int cols)
{
	int row, col;
	
	if((rows <= 0) || (cols <= 0)) return -1;
		
	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	for (row = 0; row < rows; row++)
	  for (col = 0; col < cols; col++) {
	    PIXEL* o = original + row*cols + col;
	    PIXEL* n = (*new) + (rows-1-row)*cols + col;
	    *n = *o;		
	   } 
	return 0;	   	 	
}
/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
   int row, col;

   if((rows <= 0) || (cols <= 0)) return -1;

   *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));
   
    for (row = 0; row < rows; row++)
      for (col = 0; col < cols; col++) {
	PIXEL* o = original + row*cols + col;
	PIXEL* n = (*new) + row*cols + (cols-1-col);
	*n = *o;	
       }
  return 0;
}

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int row, col, nrow, ncol;
	PIXEL *map, *n;
	char *out;
	char *data = NULL;
	int g, s, r;
	int sflag = 0, rflag = 0, fflag = 0, vflag = 0, oflag = 0;
	static char usage[] = "Usage: %./bmptool [-s scale | -r degree | -f | -v] [-o outputfile] [input file]";

	while((g = getopt(argc, argv, "s:r:fvo:")) != -1)
		switch(g)
		{
		  case 'r':
		  rflag = 1;
		  r = atoi(optarg);
		  break;
	
		  case 's':
		  sflag = 1;
		  s = atoi(optarg);
		  break;
		  
		  case 'f':
		  fflag = 1;
		  break;
		
		  case 'v':
		  vflag = 1;
		  break;

		  case 'o':
		  oflag = 1;
		  out = optarg;
		  break;
		}
		if((optind + 1) > argc)	
		{ 
	   	  fprintf(stderr, usage, argv[0]);
		  exit(1);
		  return 1;
		}
		if(oflag == 0)
		{
		  fprintf(stdout, "%s", out);
		}
		else data = argv[optind];
			
	        if(sflag >= 2 || rflag >= 2 || vflag >= 2)
		{
		  printf("Duplicate options not permitted");
		  fprintf(stderr, usage, argv[0]);
	          return 1;
		}
		if(sflag == 1 && rflag == 1 && vflag == 1 && fflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  readFile(out, &nrow, &ncol, &map);
		  rotate(map, nrow, ncol, r, &n, &row, &col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
	          readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
    		}
		if(sflag == 1 && rflag == 1 && fflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  readFile(out, &nrow, &ncol, &map);
		  rotate(map, nrow, ncol, r, &n, &row, &col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);
	
		  free(map);
		  free(n);
		  return 0;
		}
		if(sflag == 1 && rflag == 1 && vflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  readFile(out, &nrow, &ncol, &map);
		  rotate(map, nrow, ncol, r, &n, &row, &col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n); 
		  return 0;
		}
		if(rflag == 1 && vflag == 1 && fflag == 1)
		{
	 	  readFile(data, &row, &col, &map);
		  rotate(map, row, col, r, &n, &nrow, &ncol);
		  row = nrow;
		  col = ncol;
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
		}
                if(sflag == 1 && vflag == 1 && fflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  row = nrow;
		  col = ncol;
		  readFile(out, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
		}
		if(sflag == 1 && rflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  rotate(map, row, col, r, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  readFile(out, &nrow, &ncol, &map);
		  scale(map, nrow, ncol, s, &n, &row, &col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
		}
		if(sflag == 1 && fflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);
		  row = nrow;
		  col = ncol;
		  readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
		}
	 	if(sflag == 1 && vflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  scale(map, row, col, s, &n, &nrow, &ncol);
		  writeFile(out, nrow, ncol, n);

		  free(map);
		  free(n);
		  return 0;		 
		}
		if(rflag == 1 && vflag == 1)
                {
		  readFile(data, &row, &col, &map);
		  rotate(map, row, col, r, &n, &nrow, &ncol);
		  row = nrow;
		  col = ncol;
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
		
	          free(map);
		  free(n);
		  return 0;	
		}
		if(vflag == 1 && fflag == 1)
		{
		  readFile(data, &row, & col, &map);
		  vflip(map, &n, row, col);
		  writeFile(out, row, col, n);
		  readFile(out, &row, &col, &map);
		  flip(map, &n, row, col);
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;
		}
                if(rflag == 1 && fflag == 1)
		{
		   readFile(data, &row, &col, &map);
		   rotate(map, row, col, r, &n, &nrow, &ncol);
		   row = nrow;
		   col = ncol;
		   writeFile(out, row, col, n);
		   readFile(out, &row, &col, &map);
		   flip(map, &n, row, col);
		   writeFile(out, row, col, n);
		   
		   free(map);
		   free(n);
		   return 0;
		}
		if(sflag == 1)
		{
		  readFile(data, &row, &col, &map);  
	          scale(map, row, col, s, &n, &nrow, &ncol);
		  row = nrow;
		  col = ncol;
		  writeFile(out, row, col, n);

		  free(map);
		  free(n);
		  return 0;		 
		}
		if(rflag == 1)
		{
		  readFile(data, &row, &col, &map);
		  rotate(map, row, col, r, &n, &nrow, &ncol);
		  nrow = row;
		  ncol = col;
		  writeFile(out, row, col, n);

	          free(map);
		  free(n);
		  return 0;	
		}
		if(fflag == 1)
		{
		 readFile(data, &row, &col, &map); 
		 flip(map, &n, row, col);
	 	 writeFile(out, row, col, n);
		
	  	 free(map);
		 free(n);
		 return 0;
		}
	        if(vflag == 1)
		{
		 readFile(data, &row, &col, &map); 
		 vflip(map, &n, row, col);
		 writeFile(out, row, col, n);
		
		 free(map);
		 free(n);
		 return 0;
		}
return 0;		  
}
