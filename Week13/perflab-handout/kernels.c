/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Wan",              /* name */

    "2019007901",     /* student ID */
    "aus.baik@gmail.com",  /* student email */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}


/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj;
    int cache_limit = 16;
    for ( i = 0; i < dim; i += cache_limit ) {
	for ( j = 0; j < dim; j += cache_limit ) {
            int jj_limit = j + cache_limit;
		for ( jj = j; jj < jj_limit; jj += 1 ) {
		    int ii_limit = i + cache_limit;
			for ( ii = i; ii < ii_limit; ii += 1 ) {
			    dst[RIDX(dim-1-jj, ii, dim)] = src[RIDX(ii, jj, dim)];
			}
		}
	}
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
   //PROCESSING 4 CORNERS (PIXELS WITH 4 SURROUNDING)
   // top left pixel
   int top_left_pos = 0;
   dst[top_left_pos].red   = (src[top_left_pos].red + src[top_left_pos + 1].red + src[dim].red + src[dim + 1].red)/4;
   dst[top_left_pos].green = (src[top_left_pos].green + src[top_left_pos + 1].green + src[dim].green + src[dim + 1].green)/4;
   dst[top_left_pos].blue  = (src[top_left_pos].blue + src[top_left_pos + 1].blue + src[dim].blue + src[dim + 1].blue)/4;

   //top right pixel
   int top_right_pos = dim - 1;
   dst[top_right_pos].red   = (src[top_right_pos].red + src[top_right_pos - 1].red + src[top_right_pos + dim - 1].red + src[top_right_pos + dim].red) /4;
   dst[top_right_pos].green = (src[top_right_pos].green + src[top_right_pos - 1].green + src[top_right_pos + dim - 1].green + src[top_right_pos + dim].green)/4;
   dst[top_right_pos].blue  = (src[top_right_pos].blue + src[top_right_pos - 1].blue + src[top_right_pos + dim - 1].blue + src[top_right_pos + dim].blue)/4;

   // left bottom pixel
   int bottom_left_pos = dim*dim-dim;
   dst[bottom_left_pos].red   = (src[bottom_left_pos].red + src[bottom_left_pos + 1].red + src[bottom_left_pos - dim].red + src[bottom_left_pos - dim + 1].red)/4;
   dst[bottom_left_pos].green = (src[bottom_left_pos].green + src[bottom_left_pos + 1].green + src[bottom_left_pos - dim].green + src[bottom_left_pos - dim + 1].green)/4;
   dst[bottom_left_pos].blue  = (src[bottom_left_pos].blue + src[bottom_left_pos + 1].blue + src[bottom_left_pos - dim].blue + src[bottom_left_pos - dim + 1].blue)/4;

   // right bottom pixel
   int bottom_right_pos = dim*dim-1;
   dst[bottom_right_pos].red   = (src[bottom_right_pos].red + src[bottom_right_pos - 1].red + src[bottom_right_pos - dim].red + src[bottom_right_pos - dim - 1].red)/4;
   dst[bottom_right_pos].green = (src[bottom_right_pos].green + src[bottom_right_pos - 1].green + src[bottom_right_pos - dim].green + src[bottom_right_pos - dim - 1].green)/4;
   dst[bottom_right_pos].blue  = (src[bottom_right_pos].blue + src[bottom_right_pos - 1].blue + src[bottom_right_pos - dim].blue + src[bottom_right_pos - dim - 1].blue)/4;

   
   //PROCESSING EDGE PIXELS (PIXELS WITH 6 SURROUNDING)
   int i;

   // top edge 
   for (i = 1; i < top_right_pos; i++){
     dst[i].red   = (src[i].red + src[i - 1].red + src[i + 1].red + src[i + dim].red + src[i + dim - 1].red + src[i + dim + 1].red) / 6;
     dst[i].green = (src[i].green + src[i - 1].green + src[i + 1].green + src[i + dim].green + src[i + dim - 1].green + src[i + dim + 1].green) / 6;
     dst[i].blue  = (src[i].blue + src[i - 1].blue + src[i + 1].blue + src[i + dim].blue + src[i + dim - 1].blue + src[i + dim + 1].blue) / 6;
   }
   
   // left edge
   for (i = dim; i < bottom_left_pos; i += dim){
     dst[i].red = (src[i].red + src[i + 1].red + src[i - dim].red + src[i - dim + 1].red + src[i + dim].red + src[i + dim + 1].red) / 6;
     dst[i].green = (src[i].green + src[i + 1].green + src[i - dim].green+ src[i - dim + 1].green + src[i + dim].green + src[i + dim + 1].green) / 6;
     dst[i].blue = (src[i].blue + src[i + 1].blue + src[i - dim].blue + src[i - dim + 1].blue + src[i + dim].blue + src[i + dim + 1].blue) / 6;
   }
   
   // right edge
   for (i = top_right_pos+dim ; i < bottom_right_pos ; i += dim){
     dst[i].red = (src[i].red + src[i - 1].red + src[i - dim].red + src[i - dim - 1].red + src[i + dim].red + src[i + dim - 1].red) / 6;
     dst[i].green = (src[i].green + src[i - 1].green + src[i - dim].green + src[i - dim - 1].green + src[i + dim].green + src[i + dim - 1].green) / 6;
     dst[i].blue = (src[i].blue + src[i - 1].blue + src[i - dim].blue + src[i - dim - 1].blue + src[i + dim].blue + src[i + dim - 1].blue) / 6;
   }

   // bottom edge
   for (i = bottom_left_pos+1 ; i < bottom_right_pos; i++){
     dst[i].red   = (src[i].red + src[i - 1].red + src[i + 1].red + src[i - dim].red + src[i - dim - 1].red + src[i - dim + 1].red) / 6;
     dst[i].green = (src[i].green + src[i - 1].green + src[i + 1].green + src[i - dim].green + src[i - dim - 1].green + src[i - dim + 1].green) / 6;
     dst[i].blue  = (src[i].blue + src[i - 1].blue + src[i + 1].blue + src[i - dim].blue + src[i - dim - 1].blue + src[i - dim + 1].blue) / 6;
   }

   
   // PROCESSING REMAINING PIXELS (PIXELS WITH 9 SURROUNDING)
   int pos;
   for (i = 1 ; i < dim - 1 ; i++) {
     for (int j = 1 ; j < dim - 1 ; j++) {
       pos = i*dim+j;
       dst[pos].red = (src[pos].red + src[pos - 1].red + src[pos + 1].red + src[pos - dim].red + src[pos - dim - 1].red + src[pos - dim + 1].red + src[pos + dim].red + src[pos + dim - 1].red + src[pos + dim + 1].red) / 9;
       dst[pos].green = (src[pos].green + src[pos - 1].green + src[pos + 1].green + src[pos - dim].green + src[pos - dim - 1].green + src[pos - dim + 1].green + src[pos + dim].green + src[pos + dim - 1].green + src[pos + dim + 1].green) / 9;
       dst[pos].blue = (src[pos].blue + src[pos - 1].blue + src[pos + 1].blue + src[pos - dim].blue + src[pos - dim - 1].blue + src[pos - dim + 1].blue + src[pos + dim].blue + src[pos + dim - 1].blue + src[pos + dim + 1].blue) / 9;
     }
   }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

