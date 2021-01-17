/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Team Cinayat",                     /* Team name */

    "Ozan Sazak",             /* First member full name */
    "e2310456",                 /* First member id */

    "Osman Ufuk Yağmur",                         /* Second member full name (leave blank if none) */
    "e2310597",                         /* Second member id (leave blank if none) */

    "",                         /* Third member full name (leave blank if none) */
    ""                          /* Third member id (leave blank if none) */
};

/****************
 * BOKEH KERNEL *
 ****************/

/****************************************************************
 * Various typedefs and helper functions for the bokeh function
 * You may modify these any way you like.                       
 ****************************************************************/

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

static pixel avg(int i, int j, int dim, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
            // if(i == 0 && j == 1){
            //     printf("naivedekiler ==>  %d", (int) src[RIDX(ii,jj,dim)].red);
            // }
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
        }
        
    }

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/*******************************************************
 * Your different versions of the bokeh kernel go here 
 *******************************************************/

/* 
 * naive_bokeh - The naive baseline version of bokeh effect with filter
 */
char naive_bokeh_descr[] = "naive_bokeh: Naive baseline bokeh with filter";
void naive_bokeh(int dim, pixel *src, short *flt, pixel *dst) {
  
    int i, j;

    for(i = 0; i < dim; i++) {
        for(j = 0; j < dim; j++) {
            if ( !flt[RIDX(i, j, dim)] )
                dst[RIDX(i, j, dim)] = avg(i, j, dim, src);
            else
                dst[RIDX(i, j, dim)] = src[RIDX(i, j, dim)];
        }
    }
}

//#define EQUALIZE_PX(dst, src) {dst.red = src.red; dst.green = src.green; dst.blue = src.blue;}
#define EQUALIZE_PX(dst, src) {dst = src;}

char bokeh_desc[] = "bokeh4: sexy + unrolling";
void bokeh(int dim, pixel *src, short *flt, pixel *dst) {
    unsigned int t, j, rij;

    if (!flt[0]) {
        dst->red = (unsigned short)(((unsigned int)src->red +(unsigned int)(src+1)->red +(unsigned int)(src+dim)->red +(unsigned int)(src+(dim +1))->red)>>2);
        dst->blue = (unsigned short)(((unsigned int) src->blue+(unsigned int) (src+1)->blue+ (unsigned int) (src+dim)->blue+ (unsigned int) (src+(dim +1))->blue)>>2);
        dst->green = (unsigned short)(( (unsigned int) src->green+ (unsigned int) (src+1)->green+ (unsigned int) (src+dim)->green+ (unsigned int) (src+(dim +1))->green)>>2);
    } else EQUALIZE_PX(dst[0],  src[0])
   
    if (!flt[(dim  - 1 )]) {
        (dst+(dim  - 1 ))->red = (unsigned short)(( (unsigned int) (src+(dim  - 1 ))->red + (unsigned int) (src+dim-2)->red + (unsigned int) (src+2 * dim -1)->red + (unsigned int) (src+2 * dim -2)->red)>>2);
        (dst+(dim  - 1 ))->blue = (unsigned short)(( (unsigned int) (src+(dim  - 1 ))->blue+ (unsigned int) (src+dim-2)->blue+ (unsigned int) (src+2 * dim -1)->blue+ (unsigned int) (src+2 * dim -2)->blue)>>2);
        (dst+(dim  - 1 ))->green = (unsigned short)(( (unsigned int) (src+(dim  - 1 ))->green+ (unsigned int) (src+dim-2)->green+ (unsigned int) (src+2 * dim -1)->green+ (unsigned int) (src+2 * dim -2)->green)>>2);
    } else EQUALIZE_PX(dst[dim  - 1 ],  src[dim  - 1 ])

    for (t = dim - 2, j = dim; t--; j += dim) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(((unsigned int) (src+j)->red + (unsigned int) (src+j-dim)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j+dim)->red + (unsigned int) (src+j+(dim +1))->red + (unsigned int) (src+j-(dim  - 1 ))->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-dim)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j+dim)->green+ (unsigned int) (src+j+(dim +1))->green+ (unsigned int) (src+j-(dim  - 1 ))->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-dim)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j+dim)->blue+ (unsigned int) (src+j+(dim +1))->blue+ (unsigned int) (src+j-(dim  - 1 ))->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    if (!flt[(dim  * dim - dim)]) {
        (dst+(dim  * dim - dim))->red = (unsigned short)(( (unsigned int) (src+(dim  * dim - dim))->red + (unsigned int) (src+(dim  * dim - dim)+1)->red + (unsigned int) (src+(dim * dim - 2 * dim))->red + (unsigned int) (src+(dim * dim - 2 * dim)+1)->red)>>2);
        (dst+(dim  * dim - dim))->blue = (unsigned short)(( (unsigned int) (src+(dim  * dim - dim))->blue+ (unsigned int) (src+(dim  * dim - dim)+1)->blue+ (unsigned int) (src+(dim * dim - 2 * dim))->blue+ (unsigned int) (src+(dim * dim - 2 * dim)+1)->blue)>>2);
        (dst+(dim  * dim - dim))->green = (unsigned short)(( (unsigned int) (src+(dim  * dim - dim))->green+ (unsigned int) (src+(dim  * dim - dim)+1)->green+ (unsigned int) (src+(dim * dim - 2 * dim))->green+ (unsigned int) (src+(dim * dim - 2 * dim)+1)->green)>>2);
    } else EQUALIZE_PX(dst[(dim  * dim - dim)], src[(dim  * dim - dim)])

    if (!flt[(dim  * dim - 1)]) {
        (dst+dim  * dim -1)->red = (unsigned short)(( (unsigned int) (src+dim  * dim -1)->red + (unsigned int) (src+dim  * dim -2)->red + (unsigned int) (src+(dim  * dim - dim)-1)->red + (unsigned int) (src+(dim  * dim - dim)-2)->red)>>2);
        (dst+dim  * dim -1)->blue = (unsigned short)(( (unsigned int) (src+dim  * dim -1)->blue+ (unsigned int) (src+dim  * dim -2)->blue+ (unsigned int) (src+(dim  * dim - dim)-1)->blue+ (unsigned int) (src+(dim  * dim - dim)-2)->blue)>>2);
        (dst+dim  * dim -1)->green = (unsigned short)(( (unsigned int) (src+dim  * dim -1)->green+ (unsigned int) (src+dim  * dim -2)->green+ (unsigned int) (src+(dim  * dim - dim)-1)->green+ (unsigned int) (src+(dim  * dim - dim)-2)->green)>>2);
    } else EQUALIZE_PX(dst[(dim  * dim - 1)],  src[(dim  * dim - 1)])

    for (t = dim - 2, j = (dim  * dim - dim) + 1; t--; j++) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(( (unsigned int) (src+j)->red + (unsigned int) (src+j-1)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j-dim)->red + (unsigned int) (src+j-(dim  - 1 ))->red + (unsigned int) (src+j-(dim +1))->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-1)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j-dim)->green+ (unsigned int) (src+j-(dim  - 1 ))->green+ (unsigned int) (src+j-(dim +1))->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-1)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j-dim)->blue+ (unsigned int) (src+j-(dim  - 1 ))->blue+ (unsigned int) (src+j-(dim +1))->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (t = (dim - 2), j = 2 * dim - 1; t--; j += dim) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short) ((unsigned int) ((src+j)->red + (unsigned int)(src+j-1)->red + (unsigned int)(src+j-dim)->red +(unsigned int)(src+j+dim)->red + (unsigned int)(src+j-(dim +1))->red + (unsigned int) (src+j+(dim  - 1 ))->red)/6);
            (dst+j)->green =  (unsigned short) ((unsigned int) ((src+j)->green + (unsigned int) (src+j-1)->green + (unsigned int) (src+j-dim)->green + (unsigned int) (src+j+dim)->green + (unsigned int) (src+j-(dim +1))->green + (unsigned int) (src+j+(dim  - 1 ))->green)/6);
            (dst+j)->blue =  (unsigned short) ((unsigned int) ((src+j)->blue + (unsigned int) (src+j-1)->blue + (unsigned int)(src+j-dim)->blue + (unsigned int)(src+j+dim)->blue + (unsigned int)(src+j-(dim +1))->blue + (unsigned int)(src+j+(dim  - 1 ))->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    const unsigned int count1 = (dim - 2) / 6;
    const unsigned int remainder = (dim - 2) % 6;

    rij = j = dim - 1;

    while (--j) {
        rij += 2;

        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(( (unsigned int) (src+j)->red + (unsigned int) (src+j-1)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j+dim)->red + (unsigned int) (src+j+(dim +1))->red + (unsigned int) (src+j+(dim  - 1 ))->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-1)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j+dim)->green+ (unsigned int) (src+j+(dim +1))->green+ (unsigned int) (src+j+(dim  - 1 ))->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-1)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j+dim)->blue+ (unsigned int) (src+j+(dim +1))->blue+ (unsigned int) (src+j+(dim  - 1 ))->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])

        int fake_count = count1;
        while(fake_count--){
            if (!flt[rij]) {
                (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij], src[rij])
           
            if (!flt[rij+1]) {
                (dst+rij+1)->red = (unsigned short) (((unsigned int) (src+rij+1)->red + (unsigned int)(src+rij+1-1)->red + (unsigned int) (src+rij+1+1)->red + (unsigned int)(src+rij+1-dim)->red + (unsigned int)(src+rij+1-(dim +1))->red + (unsigned int)(src+rij+1-(dim  - 1 ))->red + (unsigned int)(src+rij+1+dim)->red + (unsigned int)(src+rij+1+(dim +1))->red + (unsigned int)(src+rij+1+(dim  - 1 ))->red)/9);
                (dst+rij+1)->green = (unsigned short) (((unsigned int) (src+rij+1)->green + (unsigned int) (src+rij+1-1)->green + (unsigned int) (src+rij+1+1)->green + (unsigned int) (src+rij+1-dim)->green  + (unsigned int)(src+rij+1-(dim +1))->green + (unsigned int) (src+rij+1-(dim  - 1 ))->green + (unsigned int)(src+rij+1+dim)->green + (unsigned int) (src+rij+1+(dim +1))->green + (unsigned int)(src+rij+1+(dim  - 1 ))->green)/9);
                (dst+rij+1)->blue = (unsigned short) (((unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij+1-1)->blue + (unsigned int) (src+rij+1+1)->blue+ (unsigned int) (src+rij+1-dim)->blue + (unsigned int) (src+rij+1-(dim +1))->blue + (unsigned int) (src+rij+1-(dim  - 1 ))->blue + (unsigned int) (src+rij+1+dim)->blue + (unsigned int) (src+rij+1+(dim +1))->blue+ (unsigned int) (src+rij+1+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij+1], src[rij+1])
           
             if (!flt[rij+2]) {
                (dst+rij+2)->red = (unsigned short) (((unsigned int) (src+rij+2)->red + (unsigned int)(src+rij+2-1)->red + (unsigned int) (src+rij+2+1)->red + (unsigned int)(src+rij+2-dim)->red + (unsigned int)(src+rij+2-(dim +1))->red + (unsigned int)(src+rij+2-(dim  - 1 ))->red + (unsigned int)(src+rij+2+dim)->red + (unsigned int)(src+rij+2+(dim +1))->red + (unsigned int)(src+rij+2+(dim  - 1 ))->red)/9);
                (dst+rij+2)->green = (unsigned short) (((unsigned int) (src+rij+2)->green + (unsigned int) (src+rij+2-1)->green + (unsigned int) (src+rij+2+1)->green + (unsigned int) (src+rij+2-dim)->green  + (unsigned int)(src+rij+2-(dim +1))->green + (unsigned int) (src+rij+2-(dim  - 1 ))->green + (unsigned int)(src+rij+2+dim)->green + (unsigned int) (src+rij+2+(dim +1))->green + (unsigned int)(src+rij+2+(dim  - 1 ))->green)/9);
                (dst+rij+2)->blue = (unsigned short) (((unsigned int) (src+rij+2)->blue+ (unsigned int) (src+rij+2-1)->blue + (unsigned int) (src+rij+2+1)->blue+ (unsigned int) (src+rij+2-dim)->blue + (unsigned int) (src+rij+2-(dim +1))->blue + (unsigned int) (src+rij+2-(dim  - 1 ))->blue + (unsigned int) (src+rij+2+dim)->blue + (unsigned int) (src+rij+2+(dim +1))->blue+ (unsigned int) (src+rij+2+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij+2], src[rij+2])

            if (!flt[rij+3]) {
                (dst+rij+3)->red = (unsigned short) (((unsigned int) (src+rij+3)->red + (unsigned int)(src+rij+3-1)->red + (unsigned int) (src+rij+3+1)->red + (unsigned int)(src+rij+3-dim)->red + (unsigned int)(src+rij+3-(dim +1))->red + (unsigned int)(src+rij+3-(dim  - 1 ))->red + (unsigned int)(src+rij+3+dim)->red + (unsigned int)(src+rij+3+(dim +1))->red + (unsigned int)(src+rij+3+(dim  - 1 ))->red)/9);
                (dst+rij+3)->green = (unsigned short) (((unsigned int) (src+rij+3)->green + (unsigned int) (src+rij+3-1)->green + (unsigned int) (src+rij+3+1)->green + (unsigned int) (src+rij+3-dim)->green  + (unsigned int)(src+rij+3-(dim +1))->green + (unsigned int) (src+rij+3-(dim  - 1 ))->green + (unsigned int)(src+rij+3+dim)->green + (unsigned int) (src+rij+3+(dim +1))->green + (unsigned int)(src+rij+3+(dim  - 1 ))->green)/9);
                (dst+rij+3)->blue = (unsigned short) (((unsigned int) (src+rij+3)->blue+ (unsigned int) (src+rij+3-1)->blue + (unsigned int) (src+rij+3+1)->blue+ (unsigned int) (src+rij+3-dim)->blue + (unsigned int) (src+rij+3-(dim +1))->blue + (unsigned int) (src+rij+3-(dim  - 1 ))->blue + (unsigned int) (src+rij+3+dim)->blue + (unsigned int) (src+rij+3+(dim +1))->blue+ (unsigned int) (src+rij+3+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij+3], src[rij+3])
           
             if (!flt[rij+4]) {
                (dst+rij+4)->red = (unsigned short) (((unsigned int) (src+rij+4)->red + (unsigned int)(src+rij+4-1)->red + (unsigned int) (src+rij+4+1)->red + (unsigned int)(src+rij+4-dim)->red + (unsigned int)(src+rij+4-(dim +1))->red + (unsigned int)(src+rij+4-(dim  - 1 ))->red + (unsigned int)(src+rij+4+dim)->red + (unsigned int)(src+rij+4+(dim +1))->red + (unsigned int)(src+rij+4+(dim  - 1 ))->red)/9);
                (dst+rij+4)->green = (unsigned short) (((unsigned int) (src+rij+4)->green + (unsigned int) (src+rij+4-1)->green + (unsigned int) (src+rij+4+1)->green + (unsigned int) (src+rij+4-dim)->green  + (unsigned int)(src+rij+4-(dim +1))->green + (unsigned int) (src+rij+4-(dim  - 1 ))->green + (unsigned int)(src+rij+4+dim)->green + (unsigned int) (src+rij+4+(dim +1))->green + (unsigned int)(src+rij+4+(dim  - 1 ))->green)/9);
                (dst+rij+4)->blue = (unsigned short) (((unsigned int) (src+rij+4)->blue+ (unsigned int) (src+rij+4-1)->blue + (unsigned int) (src+rij+4+1)->blue+ (unsigned int) (src+rij+4-dim)->blue + (unsigned int) (src+rij+4-(dim +1))->blue + (unsigned int) (src+rij+4-(dim  - 1 ))->blue + (unsigned int) (src+rij+4+dim)->blue + (unsigned int) (src+rij+4+(dim +1))->blue+ (unsigned int) (src+rij+4+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij+4], src[rij+4])
            
            if (!flt[rij+5]) {
                (dst+rij+5)->red = (unsigned short) (((unsigned int) (src+rij+5)->red + (unsigned int)(src+rij+5-1)->red + (unsigned int) (src+rij+5+1)->red + (unsigned int)(src+rij+5-dim)->red + (unsigned int)(src+rij+5-(dim +1))->red + (unsigned int)(src+rij+5-(dim  - 1 ))->red + (unsigned int)(src+rij+5+dim)->red + (unsigned int)(src+rij+5+(dim +1))->red + (unsigned int)(src+rij+5+(dim  - 1 ))->red)/9);
                (dst+rij+5)->green = (unsigned short) (((unsigned int) (src+rij+5)->green + (unsigned int) (src+rij+5-1)->green + (unsigned int) (src+rij+5+1)->green + (unsigned int) (src+rij+5-dim)->green  + (unsigned int)(src+rij+5-(dim +1))->green + (unsigned int) (src+rij+5-(dim  - 1 ))->green + (unsigned int)(src+rij+5+dim)->green + (unsigned int) (src+rij+5+(dim +1))->green + (unsigned int)(src+rij+5+(dim  - 1 ))->green)/9);
                (dst+rij+5)->blue = (unsigned short) (((unsigned int) (src+rij+5)->blue+ (unsigned int) (src+rij+5-1)->blue + (unsigned int) (src+rij+5+1)->blue+ (unsigned int) (src+rij+5-dim)->blue + (unsigned int) (src+rij+5-(dim +1))->blue + (unsigned int) (src+rij+5-(dim  - 1 ))->blue + (unsigned int) (src+rij+5+dim)->blue + (unsigned int) (src+rij+5+(dim +1))->blue+ (unsigned int) (src+rij+5+(dim  - 1 ))->blue)/9);
            } else EQUALIZE_PX(dst[rij+5], src[rij+5])

            rij+=6;
        }
        switch(remainder){
            case 4: 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++; 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;  
                break;     
            case 2: 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-(dim +1))->red + (unsigned int)(src+rij-(dim  - 1 ))->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+(dim +1))->red + (unsigned int)(src+rij+(dim  - 1 ))->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-(dim +1))->green + (unsigned int) (src+rij-(dim  - 1 ))->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+(dim +1))->green + (unsigned int)(src+rij+(dim  - 1 ))->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-(dim +1))->blue + (unsigned int) (src+rij-(dim  - 1 ))->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+(dim +1))->blue+ (unsigned int) (src+rij+(dim  - 1 ))->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])

                rij++;
                break;
        }
    }
}


/*********************************************************************asdfsa
 * register_bokeh_functions - Register all of your different versions
 *     of the bokeh kernel with the driver by calling the
 *     add_bokeh_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_bokeh_functions() 
{
    add_bokeh_function(&naive_bokeh, naive_bokeh_descr);   
    add_bokeh_function(&bokeh, bokeh_desc);    
    /* ... Register additional test functions here */
}

/***************************
 * HADAMARD PRODUCT KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the hadamard product functions go here
 ******************************************************/

/* 
 * naive_hadamard - The naive baseline version of hadamard product of two matrices
 */
char naive_hadamard_descr[] = "naive_hadamard The naive baseline version of hadamard product of two matrices";
void naive_hadamard(int dim, int *src1, int *src2, int *dst) {
  
    int i, j;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) 
            dst[RIDX(i, j, dim)] = src1[RIDX(i, j, dim)] * src2[RIDX(i, j, dim)];
}

/* 
 * hadamard - Your current working version of hadamard product
 * IMPORTANT: This is the version you will be graded on
 */
char hadamard_descr[] = "hadamard: Current working version";
void hadamard(int dim, int *src1, int *src2, int *dst) {
    int elem_count = dim * dim;
    int n = (elem_count + 53) >> 6;

    switch (elem_count & 63) {
        case  0: *dst++ = *src1++ * *src2++;
        case 63: *dst++ = *src1++ * *src2++;
        case 62: *dst++ = *src1++ * *src2++;
        case 61: *dst++ = *src1++ * *src2++;
        case 60: *dst++ = *src1++ * *src2++;
        case 59: *dst++ = *src1++ * *src2++;
        case 58: *dst++ = *src1++ * *src2++;
        case 57: *dst++ = *src1++ * *src2++;
        case 56: *dst++ = *src1++ * *src2++;
        case 55: *dst++ = *src1++ * *src2++;
        case 54: *dst++ = *src1++ * *src2++;
        case 53: *dst++ = *src1++ * *src2++;
        case 52: *dst++ = *src1++ * *src2++;
        case 51: *dst++ = *src1++ * *src2++;
        case 50: *dst++ = *src1++ * *src2++;
        case 49: *dst++ = *src1++ * *src2++;
        case 48: *dst++ = *src1++ * *src2++;
        case 47: *dst++ = *src1++ * *src2++;
        case 46: *dst++ = *src1++ * *src2++;
        case 45: *dst++ = *src1++ * *src2++;
        case 44: *dst++ = *src1++ * *src2++;
        case 43: *dst++ = *src1++ * *src2++;
        case 42: *dst++ = *src1++ * *src2++;
        case 41: *dst++ = *src1++ * *src2++;
        case 40: *dst++ = *src1++ * *src2++;
        case 39: *dst++ = *src1++ * *src2++;
        case 38: *dst++ = *src1++ * *src2++;
        case 37: *dst++ = *src1++ * *src2++;
        case 36: *dst++ = *src1++ * *src2++;
        case 35: *dst++ = *src1++ * *src2++;
        case 34: *dst++ = *src1++ * *src2++;
        case 33: *dst++ = *src1++ * *src2++;
        case 32: *dst++ = *src1++ * *src2++;
        case 31: *dst++ = *src1++ * *src2++;
        case 30: *dst++ = *src1++ * *src2++;
        case 29: *dst++ = *src1++ * *src2++;
        case 28: *dst++ = *src1++ * *src2++;
        case 27: *dst++ = *src1++ * *src2++;
        case 26: *dst++ = *src1++ * *src2++;
        case 25: *dst++ = *src1++ * *src2++;
        case 24: *dst++ = *src1++ * *src2++;
        case 23: *dst++ = *src1++ * *src2++;
        case 22: *dst++ = *src1++ * *src2++;
        case 21: *dst++ = *src1++ * *src2++;
        case 20: *dst++ = *src1++ * *src2++;
        case 19: *dst++ = *src1++ * *src2++;
        case 18: *dst++ = *src1++ * *src2++;
        case 17: *dst++ = *src1++ * *src2++;
        case 16: *dst++ = *src1++ * *src2++;
        case 15: *dst++ = *src1++ * *src2++;
        case 14: *dst++ = *src1++ * *src2++;
        case 13: *dst++ = *src1++ * *src2++;
        case 12: *dst++ = *src1++ * *src2++;
        case 11: *dst++ = *src1++ * *src2++;
        case 10: *dst++ = *src1++ * *src2++;
        case  9: *dst++ = *src1++ * *src2++;
        case  8: *dst++ = *src1++ * *src2++;
        case  7: *dst++ = *src1++ * *src2++;
        case  6: *dst++ = *src1++ * *src2++;
        case  5: *dst++ = *src1++ * *src2++;
        case  4: *dst++ = *src1++ * *src2++;
        case  3: *dst++ = *src1++ * *src2++;
        case  2: *dst++ = *src1++ * *src2++;
        case  1: *dst++ = *src1++ * *src2++;
    }

    while (--n) {
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
        *dst++ = *src1++ * *src2++;
    }
}

/*********************************************************************
 * register_hadamard_functions - Register all of your different versions
 *     of the hadamard kernel with the driver by calling the
 *     add_hadamard_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_hadamard_functions() 
{
    add_hadamard_function(&naive_hadamard, naive_hadamard_descr);   
    add_hadamard_function(&hadamard, hadamard_descr);   
    /* ... Register additional test functions here */
}

