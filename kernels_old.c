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


static pixel avg2(int i, int j, int dim, pixel *src) 
{
    // int ii, jj;
    // pixel_sum sum;
    // pixel current_pixel;

    //initialize_pixel_sum(&sum);
    // for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
    // for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
    //     accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    //assign_sum_to_pixel(&current_pixel, sum);
    // return current_pixel;

    pixel pix; 

    if(i == 0){
        if(j == 0) {
            pix.red = (unsigned short) (((int) src->red + (int) (src+1)->red + (int) (src+dim)->red + (int) (src+dim+1)->red) >> 2); 
            pix.green = (unsigned short) (((int) src->green + (int) (src+1)->green + (int) (src+dim)->green + (int) (src+dim+1)->green) >> 2); 
            pix.blue = (unsigned short) (((int) src->blue + (int) (src+1)->blue + (int) (src+dim)->blue + (int) (src+dim+1)->blue) >> 2) ; 
        } else if(j == (dim - 1)){
            pix.red = (unsigned short) (((int) (src-1)->red + (int) src->red + (int) (src+dim -1)->red + (int) (src+dim)->red) >> 2); 
            pix.green = (unsigned short) (((int) (src-1)->green + (int) src->green + (int) (src+dim -1)->green + (int) (src+dim)->green) >> 2); 
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) src->blue + (int) (src+dim -1)->blue + (int) (src+dim)->blue) >> 2); 
        } else {
            // printf("initial reds = %d %d %d %d %d %d  ", (int) (src-1)->red,(int) src->red, (int) (src+1)->red, (int) (src+dim -1)->red, (int) (src+dim)->red, (int) (src+dim+1)->red);
            pix.red = (unsigned short) (((int) (src-1)->red + (int) src->red + (int) (src+1)->red + (int) (src+dim -1)->red + (int) (src+dim)->red + (int) (src+dim+1)->red)/ 6); 
            pix.green = (unsigned short) (((int) (src-1)->green + (int) src->green + (int) (src+1)->green + (int) (src+dim -1)->green + (int) (src+dim)->green + (int) (src+dim+1)->green)/ 6); 
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) src->blue + (int) (src+1)->blue + (int) (src+dim -1)->blue + (int) (src+dim)->blue + (int) (src+dim+1)->blue)/ 6); 
        }
    }
    else if(i == (dim - 1)) { 
        if(j == 0) {
            pix.red = (unsigned short) (((int) src->red + (int) (src+1)->red + (int) (src-dim)->red + (int) (src-dim+1)->red) >> 2); 
            pix.green = (unsigned short) (((int) src->green + (int) (src+1)->green + (int) (src-dim)->green + (int) (src-dim+1)->green) >> 2); 
            pix.blue = (unsigned short) (((int) src->blue + (int) (src+1)->blue + (int) (src-dim)->blue + (int) (src-dim+1)->blue) >> 2); 
        } else if(j == (dim - 1)){
            pix.red = (unsigned short) (((int) (src-1)->red + (int) (src)->red + (int) (src-dim -1)->red + (int) (src-dim)->red) >> 2); 
            pix.green = (unsigned short) (((int) (src-1)->green + (int) (src)->green + (int) (src-dim -1)->green + (int) (src-dim)->green) >> 2); 
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) (src)->blue + (int) (src-dim -1)->blue + (int) (src-dim)->blue) >> 2); 
        } else {
            pix.red = (unsigned short) (((int) (src-1)->red + (int) (src)->red +  (int) (src+1)->red + (int) (src-dim -1)->red + (int) (src-dim)->red + (int) (src-dim+1)->red)/ 6); 
            pix.green = (unsigned short) (((int) (src-1)->green + (int) (src)->green +  (int) (src+1)->green + (int) (src-dim -1)->green + (int) (src-dim)->green + (int) (src-dim+1)->green)/ 6); 
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) (src)->blue +  (int) (src+1)->blue + (int) (src-dim -1)->blue + (int) (src-dim)->blue + (int) (src-dim+1)->blue)/ 6); 
        }
    }
    else {
        if(j == 0) { 
            pix.red = (unsigned short) (((int) (src)->red +  (int) (src+1)->red + (int) (src-dim)->red + (int) (src-dim+1)->red + (int) (src+dim)->red + (int) (src+dim+1)->red)/ 6);
            pix.green = (unsigned short) (((int) (src)->green +  (int) (src+1)->green + (int) (src-dim)->green + (int) (src-dim+1)->green + (int) (src+dim)->green + (int) (src+dim+1)->green)/ 6);
            pix.blue = (unsigned short) (((int) (src)->blue +  (int) (src+1)->blue + (int) (src-dim)->blue + (int) (src-dim+1)->blue + (int) (src+dim)->blue + (int) (src+dim+1)->blue)/ 6); 
        
         
        } else if(j == (dim - 1)){
            pix.red = (unsigned short) (((int) (src-1)->red + (int) (src)->red  + (int) (src-dim -1)->red + (int) (src-dim)->red + (int) (src+dim -1)->red + (int) (src+dim)->red)/ 6);
            pix.green = (unsigned short) (((int) (src-1)->green + (int) (src)->green +  (int) (src-dim -1)->green + (int) (src-dim)->green + (int) (src+dim -1)->green + (int) (src+dim)->green )/ 6);
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) (src)->blue + (int) (src-dim -1)->blue + (int) (src-dim)->blue + (int) (src+dim -1)->blue + (int) (src+dim)->blue)/ 6); 
        
        } else {
            pix.red = (unsigned short) (((int) (src-1)->red + (int) (src)->red +  (int) (src+1)->red + (int) (src-dim -1)->red + (int) (src-dim)->red + (int) (src-dim+1)->red +(int) (src+dim -1)->red + (int) (src+dim)->red + (int) (src+dim+1)->red)/ 9);
            pix.green = (unsigned short) (((int) (src-1)->green + (int) (src)->green +  (int) (src+1)->green + (int) (src-dim -1)->green + (int) (src-dim)->green + (int) (src-dim+1)->green +(int) (src+dim -1)->green + (int) (src+dim)->green + (int) (src+dim+1)->green)/ 9);
            pix.blue = (unsigned short) (((int) (src-1)->blue + (int) (src)->blue +  (int) (src+1)->blue + (int) (src-dim -1)->blue + (int) (src-dim)->blue + (int) (src-dim+1)->blue +(int) (src+dim -1)->blue + (int) (src+dim)->blue + (int) (src+dim+1)->blue)/ 9); 
        }
    }

    return pix;
        // count++;
        // pixel * p = src - dim - 1; 
        // sum->red += (int) p.red;
        // sum->green += (int) p.green;
        // sum->blue += (int) p.blue;
        // sum->num++;
    // current_pixel->red = (unsigned short) (sum.red/sum.num);
    // current_pixel->green = (unsigned short) (sum.green/sum.num);
    // current_pixel->blue = (unsigned short) (sum.blue/sum.num);
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

/* 
 * bokeh - Your current working version of bokeh
 * IMPORTANT: This is the version you will be graded on
 */
char bokeh_descr[] = "bokeh: Current working version";

static pixel rdix(int i, int j, int dim , pixel *src ){
    return src[i * dim + j];
}

void bokeh(int dim, pixel *src, short *flt, pixel *dst) 
{
    int i, j;

    // void avg_ptr (int) = &avg; 
    // void rdix_ptr (int) = &rdix;
    
    pixel (*p[2]) (int i, int j, int dim, pixel *src);
    p[0] = rdix; 
    p[1] = avg2;


    for(i = 0; i < dim; i++) {
        for(j = 0; j < dim; j++) {
            // if ( !flt[RIDX(i, j, dim)] )
            //     dst[RIDX(i, j, dim)] = avg(i, j dim, src);
            // else
            //     dst[RIDX(i, j, dim)] = src[RIDX(i, j, dim)];

            dst[RIDX(i, j, dim)] = (*p[!flt[RIDX(i,j,dim)]])(i, j, dim, src++);
    
        }
    }  
}

static void avg_9_2(int dim, pixel* src, pixel *dst){
    dst->red = (unsigned short) (((int) (src-1)->red + (int) (src)->red +  (int) (src+1)->red + (int) (src-dim -1)->red + (int) (src-dim)->red + (int) (src-dim+1)->red +(int) (src+dim -1)->red + (int) (src+dim)->red + (int) (src+dim+1)->red)/ 9);
    dst->green = (unsigned short) (((int) (src-1)->green + (int) (src)->green +  (int) (src+1)->green + (int) (src-dim -1)->green + (int) (src-dim)->green + (int) (src-dim+1)->green +(int) (src+dim -1)->green + (int) (src+dim)->green + (int) (src+dim+1)->green)/ 9);
    dst->blue = (unsigned short) (((int) (src-1)->blue + (int) (src)->blue +  (int) (src+1)->blue + (int) (src-dim -1)->blue + (int) (src-dim)->blue + (int) (src-dim+1)->blue +(int) (src+dim -1)->blue + (int) (src+dim)->blue + (int) (src+dim+1)->blue)/ 9); 
}

static void rdix_inside(int dim , pixel *src, pixel *dst){
    *dst = *src;
}

char bokeh2_descr[] = "bokeh2: A sexier one";
void bokeh2(int dim, pixel *src, short *flt, pixel *dst) {
    int i, j;
    pixel *src_orig = src;
    short *flt_orig = flt; 
    pixel *dst_orig = dst; 

    pixel (*p[2]) (int i, int j, int dim, pixel *src);
    p[0] = rdix; 
    p[1] = avg2;

    // i = 0
    for (j = 0; j < dim; j++) // top edge
        dst[j] = (*p[!flt[j]])(0, j, dim, src++);

    // i = dim and j = dim - 1
    for (i = dim; i <= ((dim-1)*dim); i += dim) {
        dst[i] = (*p[!flt[i]])(i, 0, dim, src);
        src += (dim - 1);
        dst[i+dim-1] = (*p[!flt[i+dim-1]])(i+dim-1, dim - 1, dim, src++);
    }
    src = src_orig + ((dim-1)*dim);

    for (j = 0; j < dim; j++) // top edge
        dst[(dim-1)*dim + j] = (*p[!flt[(dim-1)*dim + j]])(dim-1, j, dim, src++);

    void (*p_inside[2]) (int dim, pixel *src, pixel *dst);
    p_inside[0] = rdix_inside; 
    p_inside[1] = avg_9_2;


    src = src_orig + dim + 1;
    dst = dst_orig + dim + 1;
    flt = flt_orig + dim + 1;

    const int nn = ((dim - 2) + 15) >> 4;

    for (i = 1; i < dim - 1; i++) {
        switch ((dim - 2) % 16) {
            case 0: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 15: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 14: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 13: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 12: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 11: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 10: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 9: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 8: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 7: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 6: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 5: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 4: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 3: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 2: (*p_inside[!(*(flt++))])(dim, src++, dst++);
            case 1: (*p_inside[!(*(flt++))])(dim, src++, dst++);
        }

        int n = nn;

        while (--n) {
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
            (*p_inside[!(*(flt++))])(dim, src++, dst++);
        }

        src += 2;
        dst += 2;
        flt += 2;
    }
}

#define EQUALIZE_PX(dst, src) {dst.red = src.red; dst.green = src.green; dst.blue = src.blue;}

// char bokeh3_descr[] = "bokeh3: Hummm let's see...";
// void bokeh3(int dim, pixel *src, short *flt, pixel *dst) 
// {
//     int i, j, rij;
//     int rindex = dim;
    
//     // Outmost layer (corner cases)
//     if (!flt[0]) {
//         dst[0].red = (unsigned short)(((int)src[0].red +(int)src[1].red +(int)src[dim].red +(int)src[dim+1].red)>>2);
//         dst[0].blue = (unsigned short)(((int) src[0].blue+(int) src[1].blue+ (int) src[dim].blue+ (int) src[dim+1].blue)>>2);
//         dst[0].green = (unsigned short)(( (int) src[0].green+ (int) src[1].green+ (int) src[dim].green+ (int) src[dim+1].green)>>2);
//     } else EQUALIZE_PX(dst[0],  src[0])
   
//     if (!flt[dim-1]) {
//         dst[dim-1].red = (unsigned short)(( (int) src[dim-1].red + (int) src[dim-2].red + (int) src[dim*2-1].red + (int) src[dim*2-2].red)>>2);
//         dst[dim-1].blue = (unsigned short)(( (int) src[dim-1].blue+ (int) src[dim-2].blue+ (int) src[dim*2-1].blue+ (int) src[dim*2-2].blue)>>2);
//         dst[dim-1].green = (unsigned short)(( (int) src[dim-1].green+ (int) src[dim-2].green+ (int) src[dim*2-1].green+ (int) src[dim*2-2].green)>>2);
//     } else EQUALIZE_PX(dst[dim-1],  src[dim-1])

//     if (!flt[dim*(dim-1)]) {
//         dst[dim*(dim-1)].red = (unsigned short)(( (int) src[dim*(dim-1)].red + (int) src[dim*(dim-1)+1].red + (int) src[dim*(dim-2)].red + (int) src[dim*(dim-2)+1].red)>>2);
//         dst[dim*(dim-1)].blue = (unsigned short)(( (int) src[dim*(dim-1)].blue+ (int) src[dim*(dim-1)+1].blue+ (int) src[dim*(dim-2)].blue+ (int) src[dim*(dim-2)+1].blue)>>2);
//         dst[dim*(dim-1)].green = (unsigned short)(( (int) src[dim*(dim-1)].green+ (int) src[dim*(dim-1)+1].green+ (int) src[dim*(dim-2)].green+ (int) src[dim*(dim-2)+1].green)>>2);
//     } else EQUALIZE_PX(dst[dim*(dim-1)], src[dim*(dim-1)])

//     if (!flt[dim_square_mn1]) {
//         dst[dim_square_mn1].red = (unsigned short)(( (int) src[dim_square_mn1].red + (int) src[dim*dim-2].red + (int) src[dim*(dim-1)-1].red + (int) src[dim*(dim-1)-2].red)>>2);
//         dst[dim_square_mn1].blue = (unsigned short)(( (int) src[dim_square_mn1].blue+ (int) src[dim*dim-2].blue+ (int) src[dim*(dim-1)-1].blue+ (int) src[dim*(dim-1)-2].blue)>>2);
//         dst[dim_square_mn1].green = (unsigned short)(( (int) src[dim_square_mn1].green+ (int) src[dim*dim-2].green+ (int) src[dim*(dim-1)-1].green+ (int) src[dim*(dim-1)-2].green)>>2);
//     } else EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
    
    // for (j = 1; j < dim - 1; j++) {
    //     if (!flt[j]) {
    //         dst[j].red = (unsigned short)(( (int) src[j].red + (int) src[j-1].red + (int) src[j+1].red + (int) src[j+dim].red + (int) src[j+1+dim].red + (int) src[j-1+dim].red)/6);
    //         dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-1].green+ (int) src[j+1].green+ (int) src[j+dim].green+ (int) src[j+1+dim].green+ (int) src[j-1+dim].green)/6);
    //         dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-1].blue+ (int) src[j+1].blue+ (int) src[j+dim].blue+ (int) src[j+1+dim].blue+ (int) src[j-1+dim].blue)/6);
    //     } else EQUALIZE_PX(dst[j], src[j])
       
    // }

    // for (j = dim * (dim - 1) + 1; j < dim * dim - 1; j++) {
    //     if (!flt[j]) {
    //         dst[j].red = (unsigned short)(( (int) src[j].red + (int) src[j-1].red + (int) src[j+1].red + (int) src[j-dim].red + (int) src[j+1-dim].red + (int) src[j-1-dim].red)/6);
    //         dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-1].green+ (int) src[j+1].green+ (int) src[j-dim].green+ (int) src[j+1-dim].green+ (int) src[j-1-dim].green)/6);
    //         dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-1].blue+ (int) src[j+1].blue+ (int) src[j-dim].blue+ (int) src[j+1-dim].blue+ (int) src[j-1-dim].blue)/6);
    //     } else EQUALIZE_PX(dst[j], src[j])
    // }

    // for (j = dim; j < dim * (dim - 1); j += dim) {
    //     if (!flt[j]) {
    //         dst[j].red = (unsigned short)(((int) src[j].red + (int) src[j-dim].red + (int) src[j+1].red + (int) src[j+dim].red + (int) src[j+1+dim].red + (int) src[j-dim+1].red)/6);
    //         dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-dim].green+ (int) src[j+1].green+ (int) src[j+dim].green+ (int) src[j+1+dim].green+ (int) src[j-dim+1].green)/6);
    //         dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-dim].blue+ (int) src[j+1].blue+ (int) src[j+dim].blue+ (int) src[j+1+dim].blue+ (int) src[j-dim+1].blue)/6);
    //     } else EQUALIZE_PX(dst[j], src[j])
    // }

    // for (j = dim + dim - 1; j < dim * dim - 1; j += dim) {
    //     if (!flt[j]) {
    //         dst[j].red = (unsigned short) ((int) (src[j].red + (int)src[j-1].red + (int)src[j-dim].red +(int)src[j+dim].red + (int)src[j-dim-1].red + (int) src[j-1+dim].red)/6);
    //         dst[j].green =  (unsigned short) ((int) (src[j].green + (int) src[j-1].green + (int) src[j-dim].green + (int) src[j+dim].green + (int) src[j-dim-1].green + (int) src[j-1+dim].green)/6);
    //         dst[j].blue =  (unsigned short) ((int) (src[j].blue + (int) src[j-1].blue + (int)src[j-dim].blue + (int)src[j+dim].blue + (int)src[j-dim-1].blue + (int)src[j-1+dim].blue)/6);
    //     } else EQUALIZE_PX(dst[j], src[j])
    // }

//     // Inner layers
//     for (i = 1; i < dim-1; i++) {
//         for (j = 1; j < dim-1; j++) {
//             rij = rindex+j;
        
//             if (!flt[rij]) {
//                 dst[rij].red = (unsigned short) (((int) src[rij].red + (int)src[rij-1].red + (int) src[rij+1].red + (int)src[rij-dim].red + (int)src[rij-dim-1].red + (int)src[rij-dim+1].red + (int)src[rij+dim].red + (int)src[rij+dim+1].red + (int)src[rij+dim-1].red)/9);
//                 dst[rij].green = (unsigned short) (((int) src[rij].green + (int) src[rij-1].green + (int) src[rij+1].green + (int) src[rij-dim].green  + (int)src[rij-dim-1].green + (int) src[rij-dim+1].green + (int)src[rij+dim].green + (int) src[rij+dim+1].green + (int)src[rij+dim-1].green)/9);
//                 dst[rij].blue = (unsigned short) (((int) src[rij].blue+ (int) src[rij-1].blue + (int) src[rij+1].blue+src[rij-dim].blue + (int) src[rij-dim-1].blue + (int) src[rij-dim+1].blue + (int) src[rij+dim].blue + (int) src[rij+dim+1].blue+src[rij+dim-1].blue)/9);
//             } else EQUALIZE_PX(dst[rij], src[rij])
//         }
            
//         rindex += dim;
//     }
// }

#define _AVG_TOPLEFT {  dst->red = (unsigned short)(((unsigned int)src->red +(unsigned int)(src+1)->red +(unsigned int)(src+dim)->red +(unsigned int)(src+dim_ps1)->red)>>2);\
                        dst->blue = (unsigned short)(((unsigned int) src->blue+(unsigned int) (src+1)->blue+ (unsigned int) (src+dim)->blue+ (unsigned int) (src+dim_ps1)->blue)>>2);\
                        dst->green = (unsigned short)(( (unsigned int) src->green+ (unsigned int) (src+1)->green+ (unsigned int) (src+dim)->green+ (unsigned int) (src+dim_ps1)->green)>>2);}

#define _AVG_TOPRIGHT { (dst+dim_mn1)->red = (unsigned short)(( (unsigned int) (src+dim_mn1)->red + (unsigned int) (src+dim-2)->red + (unsigned int) (src+dim_times_dim-1)->red + (unsigned int) (src+dim_times_dim-2)->red)>>2);\
                        (dst+dim_mn1)->blue = (unsigned short)(( (unsigned int) (src+dim_mn1)->blue+ (unsigned int) (src+dim-2)->blue+ (unsigned int) (src+dim_times_dim-1)->blue+ (unsigned int) (src+dim_times_dim-2)->blue)>>2);\
                        (dst+dim_mn1)->green = (unsigned short)(( (unsigned int) (src+dim_mn1)->green+ (unsigned int) (src+dim-2)->green+ (unsigned int) (src+dim_times_dim-1)->green+ (unsigned int) (src+dim_times_dim-2)->green)>>2);}

#define _AVG_BOTTOMLEFT {(dst+dim_square_mn_dim)->red = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->red + (unsigned int) (src+dim_square_mn_dim+1)->red + (unsigned int) (src+dim_square_mn_2dim)->red + (unsigned int) (src+dim_square_mn_2dim+1)->red)>>2);\
                        (dst+dim_square_mn_dim)->blue = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->blue+ (unsigned int) (src+dim_square_mn_dim+1)->blue+ (unsigned int) (src+dim_square_mn_2dim)->blue+ (unsigned int) (src+dim_square_mn_2dim+1)->blue)>>2);\
                        (dst+dim_square_mn_dim)->green = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->green+ (unsigned int) (src+dim_square_mn_dim+1)->green+ (unsigned int) (src+dim_square_mn_2dim)->green+ (unsigned int) (src+dim_square_mn_2dim+1)->green)>>2);}

#define _AVG_BOTTOMRIGHT {  (dst+dim_square-1)->red = (unsigned short)(( (unsigned int) (src+dim_square-1)->red + (unsigned int) (src+dim_square-2)->red + (unsigned int) (src+dim_square_mn_dim-1)->red + (unsigned int) (src+dim_square_mn_dim-2)->red)>>2);\
                            (dst+dim_square-1)->blue = (unsigned short)(( (unsigned int) (src+dim_square-1)->blue+ (unsigned int) (src+dim_square-2)->blue+ (unsigned int) (src+dim_square_mn_dim-1)->blue+ (unsigned int) (src+dim_square_mn_dim-2)->blue)>>2);\
                            (dst+dim_square-1)->green = (unsigned short)(( (unsigned int) (src+dim_square-1)->green+ (unsigned int) (src+dim_square-2)->green+ (unsigned int) (src+dim_square_mn_dim-1)->green+ (unsigned int) (src+dim_square_mn_dim-2)->green)>>2);}

char bokeh4_descr[] = "bokeh4: sexy + unrolling";
void bokeh4(int dim, pixel *src, short *flt, pixel *dst) 
{
    //printf("\n%ld\n", sizeof(pixel));
    //printf("\n%ld\n", sizeof(unsigned short));
    //printf("\n%ld\n", sizeof(unsigned long long));

    unsigned int i, j, rij;
    //pixel *src_orig = src;
    //short *flt_orig = flt; 
    //pixel *dst_orig = dst; 
    unsigned int rindex = dim;

    unsigned int dim_square = dim * dim;
    unsigned int dim_mn1 = dim - 1;
    unsigned int dim_ps1 = dim + 1;
    unsigned int dim_times_dim = dim * 2;
    unsigned int dim_square_mn1 = dim_square - 1;
    unsigned int dim_square_mn_dim = dim_square - dim;
    unsigned int dim_square_mn_2dim = dim_square - 2 * dim;

    // switch (((flt[0] << 3) | (flt[dim_mn1] << 2) | (flt[dim_square_mn_dim] << 1) | flt[dim_square_mn1])) {
    //     // 0 0 0 0
    //     case 0:
    //         _AVG_TOPLEFT
    //         _AVG_TOPRIGHT
    //         _AVG_BOTTOMLEFT
    //         _AVG_BOTTOMRIGHT
    //         break;
    //     // 0 0 0 1
    //     case 1:
    //         _AVG_TOPLEFT
    //         _AVG_TOPRIGHT
    //         _AVG_BOTTOMLEFT
    //         EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
    //         break;
    //     // 0 0 1 0
    //     case 2:
    //         _AVG_TOPLEFT
    //         _AVG_TOPRIGHT
    //         EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])
    //         _AVG_BOTTOMRIGHT
    //         break;
    //     // 0 0 1 1
    //     case 3:
    //         break;
    //     // 0 1 0 0
    //     case 4:
    //         break;
    //     // 0 1 0 1
    //     case 5:
    //         break;
    //     // 0 1 1 0
    //     case 6:
    //         break;
    //     // 0 1 1 1
    //     case 7:
    //         break;
    //     // 1 0 0 0
    //     case 8:
    //         break;
    //     // 1 0 0 1
    //     case 9:
    //         break;
    //     // 1 0 1 0
    //     case 10:
    //         break;
    //     // 1 0 1 1
    //     case 11:
    //         break;
    //     // 1 1 0 0
    //     case 12:
    //         break;
    //     // 1 1 0 1
    //     case 13:
    //         break;
    //     // 1 1 1 0
    //     case 14:
    //         break;
    //     // 1 1 1 1
    //     case 15:
    //         break;
    // }

    // Outmost layer (corner cases)
    if (!flt[0]) {
        dst->red = (unsigned short)(((unsigned int)src->red +(unsigned int)(src+1)->red +(unsigned int)(src+dim)->red +(unsigned int)(src+dim_ps1)->red)>>2);
        dst->blue = (unsigned short)(((unsigned int) src->blue+(unsigned int) (src+1)->blue+ (unsigned int) (src+dim)->blue+ (unsigned int) (src+dim_ps1)->blue)>>2);
        dst->green = (unsigned short)(( (unsigned int) src->green+ (unsigned int) (src+1)->green+ (unsigned int) (src+dim)->green+ (unsigned int) (src+dim_ps1)->green)>>2);
    } else EQUALIZE_PX(dst[0],  src[0])
   
    if (!flt[dim_mn1]) {
        (dst+dim_mn1)->red = (unsigned short)(( (unsigned int) (src+dim_mn1)->red + (unsigned int) (src+dim-2)->red + (unsigned int) (src+dim_times_dim-1)->red + (unsigned int) (src+dim_times_dim-2)->red)>>2);
        (dst+dim_mn1)->blue = (unsigned short)(( (unsigned int) (src+dim_mn1)->blue+ (unsigned int) (src+dim-2)->blue+ (unsigned int) (src+dim_times_dim-1)->blue+ (unsigned int) (src+dim_times_dim-2)->blue)>>2);
        (dst+dim_mn1)->green = (unsigned short)(( (unsigned int) (src+dim_mn1)->green+ (unsigned int) (src+dim-2)->green+ (unsigned int) (src+dim_times_dim-1)->green+ (unsigned int) (src+dim_times_dim-2)->green)>>2);
    } else EQUALIZE_PX(dst[dim_mn1],  src[dim_mn1])

    for (j = dim; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(((unsigned int) (src+j)->red + (unsigned int) (src+j-dim)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j+dim)->red + (unsigned int) (src+j+dim_ps1)->red + (unsigned int) (src+j-dim_mn1)->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-dim)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j+dim)->green+ (unsigned int) (src+j+dim_ps1)->green+ (unsigned int) (src+j-dim_mn1)->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-dim)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j+dim)->blue+ (unsigned int) (src+j+dim_ps1)->blue+ (unsigned int) (src+j-dim_mn1)->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    if (!flt[dim_square_mn_dim]) {
        (dst+dim_square_mn_dim)->red = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->red + (unsigned int) (src+dim_square_mn_dim+1)->red + (unsigned int) (src+dim_square_mn_2dim)->red + (unsigned int) (src+dim_square_mn_2dim+1)->red)>>2);
        (dst+dim_square_mn_dim)->blue = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->blue+ (unsigned int) (src+dim_square_mn_dim+1)->blue+ (unsigned int) (src+dim_square_mn_2dim)->blue+ (unsigned int) (src+dim_square_mn_2dim+1)->blue)>>2);
        (dst+dim_square_mn_dim)->green = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->green+ (unsigned int) (src+dim_square_mn_dim+1)->green+ (unsigned int) (src+dim_square_mn_2dim)->green+ (unsigned int) (src+dim_square_mn_2dim+1)->green)>>2);
    } else EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])

    if (!flt[dim_square_mn1]) {
        (dst+dim_square-1)->red = (unsigned short)(( (unsigned int) (src+dim_square-1)->red + (unsigned int) (src+dim_square-2)->red + (unsigned int) (src+dim_square_mn_dim-1)->red + (unsigned int) (src+dim_square_mn_dim-2)->red)>>2);
        (dst+dim_square-1)->blue = (unsigned short)(( (unsigned int) (src+dim_square-1)->blue+ (unsigned int) (src+dim_square-2)->blue+ (unsigned int) (src+dim_square_mn_dim-1)->blue+ (unsigned int) (src+dim_square_mn_dim-2)->blue)>>2);
        (dst+dim_square-1)->green = (unsigned short)(( (unsigned int) (src+dim_square-1)->green+ (unsigned int) (src+dim_square-2)->green+ (unsigned int) (src+dim_square_mn_dim-1)->green+ (unsigned int) (src+dim_square_mn_dim-2)->green)>>2);
    } else EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
    
    //for (j = 1; j < dim - 1; j++) {
    //    if (!flt[j]) {
    //        dst[j].red = (unsigned short)(( (unsigned int) src[j].red + (unsigned int) src[j-1].red + (unsigned int) src[j+1].red + (unsigned int) src[j+dim].red + (unsigned int) src[j+dim_ps1].red + (unsigned int) src[j+dim_mn1].red)/6);
    //        dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-1].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j+dim].green+ (unsigned int) src[j+dim_ps1].green+ (unsigned int) src[j+dim_mn1].green)/6);
    //        dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-1].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j+dim].blue+ (unsigned int) src[j+dim_ps1].blue+ (unsigned int) src[j+dim_mn1].blue)/6);
    //    } else EQUALIZE_PX(dst[j], src[j])
    //   
    //}

    for (j = dim_square_mn_dim + 1; j < dim_square_mn1; j++) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(( (unsigned int) (src+j)->red + (unsigned int) (src+j-1)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j-dim)->red + (unsigned int) (src+j-dim_mn1)->red + (unsigned int) (src+j-dim_ps1)->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-1)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j-dim)->green+ (unsigned int) (src+j-dim_mn1)->green+ (unsigned int) (src+j-dim_ps1)->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-1)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j-dim)->blue+ (unsigned int) (src+j-dim_mn1)->blue+ (unsigned int) (src+j-dim_ps1)->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (j = dim_times_dim - 1; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            (dst+j)->red = (unsigned short) ((unsigned int) ((src+j)->red + (unsigned int)(src+j-1)->red + (unsigned int)(src+j-dim)->red +(unsigned int)(src+j+dim)->red + (unsigned int)(src+j-dim_ps1)->red + (unsigned int) (src+j+dim_mn1)->red)/6);
            (dst+j)->green =  (unsigned short) ((unsigned int) ((src+j)->green + (unsigned int) (src+j-1)->green + (unsigned int) (src+j-dim)->green + (unsigned int) (src+j+dim)->green + (unsigned int) (src+j-dim_ps1)->green + (unsigned int) (src+j+dim_mn1)->green)/6);
            (dst+j)->blue =  (unsigned short) ((unsigned int) ((src+j)->blue + (unsigned int) (src+j-1)->blue + (unsigned int)(src+j-dim)->blue + (unsigned int)(src+j+dim)->blue + (unsigned int)(src+j-dim_ps1)->blue + (unsigned int)(src+j+dim_mn1)->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    int count1 = (dim - 2) >> 3; 
    int left_overs = dim - count1 << 3; 
    // Inner layers
    for (j = 1; j < dim_mn1; j++) {
        rij = rindex + 1;

        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(( (unsigned int) (src+j)->red + (unsigned int) (src+j-1)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j+dim)->red + (unsigned int) (src+j+dim_ps1)->red + (unsigned int) (src+j+dim_mn1)->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-1)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j+dim)->green+ (unsigned int) (src+j+dim_ps1)->green+ (unsigned int) (src+j+dim_mn1)->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-1)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j+dim)->blue+ (unsigned int) (src+j+dim_ps1)->blue+ (unsigned int) (src+j+dim_mn1)->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])

        //i = 1; 
        int fake_count = count1;
        while(fake_count--){
            if (!flt[rij]) {
                (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij], src[rij])
            //rij++;
            if (!flt[rij+1]) {
                (dst+rij+1)->red = (unsigned short) (((unsigned int) (src+rij+1)->red + (unsigned int)(src+rij+1-1)->red + (unsigned int) (src+rij+1+1)->red + (unsigned int)(src+rij+1-dim)->red + (unsigned int)(src+rij+1-dim_ps1)->red + (unsigned int)(src+rij+1-dim_mn1)->red + (unsigned int)(src+rij+1+dim)->red + (unsigned int)(src+rij+1+dim_ps1)->red + (unsigned int)(src+rij+1+dim_mn1)->red)/9);
                (dst+rij+1)->green = (unsigned short) (((unsigned int) (src+rij+1)->green + (unsigned int) (src+rij+1-1)->green + (unsigned int) (src+rij+1+1)->green + (unsigned int) (src+rij+1-dim)->green  + (unsigned int)(src+rij+1-dim_ps1)->green + (unsigned int) (src+rij+1-dim_mn1)->green + (unsigned int)(src+rij+1+dim)->green + (unsigned int) (src+rij+1+dim_ps1)->green + (unsigned int)(src+rij+1+dim_mn1)->green)/9);
                (dst+rij+1)->blue = (unsigned short) (((unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij+1-1)->blue + (unsigned int) (src+rij+1+1)->blue+ (unsigned int) (src+rij+1-dim)->blue + (unsigned int) (src+rij+1-dim_ps1)->blue + (unsigned int) (src+rij+1-dim_mn1)->blue + (unsigned int) (src+rij+1+dim)->blue + (unsigned int) (src+rij+1+dim_ps1)->blue+ (unsigned int) (src+rij+1+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+1], src[rij+1])
            //rij++;
             if (!flt[rij+2]) {
                (dst+rij+2)->red = (unsigned short) (((unsigned int) (src+rij+2)->red + (unsigned int)(src+rij+2-1)->red + (unsigned int) (src+rij+2+1)->red + (unsigned int)(src+rij+2-dim)->red + (unsigned int)(src+rij+2-dim_ps1)->red + (unsigned int)(src+rij+2-dim_mn1)->red + (unsigned int)(src+rij+2+dim)->red + (unsigned int)(src+rij+2+dim_ps1)->red + (unsigned int)(src+rij+2+dim_mn1)->red)/9);
                (dst+rij+2)->green = (unsigned short) (((unsigned int) (src+rij+2)->green + (unsigned int) (src+rij+2-1)->green + (unsigned int) (src+rij+2+1)->green + (unsigned int) (src+rij+2-dim)->green  + (unsigned int)(src+rij+2-dim_ps1)->green + (unsigned int) (src+rij+2-dim_mn1)->green + (unsigned int)(src+rij+2+dim)->green + (unsigned int) (src+rij+2+dim_ps1)->green + (unsigned int)(src+rij+2+dim_mn1)->green)/9);
                (dst+rij+2)->blue = (unsigned short) (((unsigned int) (src+rij+2)->blue+ (unsigned int) (src+rij+2-1)->blue + (unsigned int) (src+rij+2+1)->blue+ (unsigned int) (src+rij+2-dim)->blue + (unsigned int) (src+rij+2-dim_ps1)->blue + (unsigned int) (src+rij+2-dim_mn1)->blue + (unsigned int) (src+rij+2+dim)->blue + (unsigned int) (src+rij+2+dim_ps1)->blue+ (unsigned int) (src+rij+2+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+2], src[rij+2])
            //rij++;
            if (!flt[rij+3]) {
                (dst+rij+3)->red = (unsigned short) (((unsigned int) (src+rij+3)->red + (unsigned int)(src+rij+3-1)->red + (unsigned int) (src+rij+3+1)->red + (unsigned int)(src+rij+3-dim)->red + (unsigned int)(src+rij+3-dim_ps1)->red + (unsigned int)(src+rij+3-dim_mn1)->red + (unsigned int)(src+rij+3+dim)->red + (unsigned int)(src+rij+3+dim_ps1)->red + (unsigned int)(src+rij+3+dim_mn1)->red)/9);
                (dst+rij+3)->green = (unsigned short) (((unsigned int) (src+rij+3)->green + (unsigned int) (src+rij+3-1)->green + (unsigned int) (src+rij+3+1)->green + (unsigned int) (src+rij+3-dim)->green  + (unsigned int)(src+rij+3-dim_ps1)->green + (unsigned int) (src+rij+3-dim_mn1)->green + (unsigned int)(src+rij+3+dim)->green + (unsigned int) (src+rij+3+dim_ps1)->green + (unsigned int)(src+rij+3+dim_mn1)->green)/9);
                (dst+rij+3)->blue = (unsigned short) (((unsigned int) (src+rij+3)->blue+ (unsigned int) (src+rij+3-1)->blue + (unsigned int) (src+rij+3+1)->blue+ (unsigned int) (src+rij+3-dim)->blue + (unsigned int) (src+rij+3-dim_ps1)->blue + (unsigned int) (src+rij+3-dim_mn1)->blue + (unsigned int) (src+rij+3+dim)->blue + (unsigned int) (src+rij+3+dim_ps1)->blue+ (unsigned int) (src+rij+3+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+3], src[rij+3])
            //rij++;
             if (!flt[rij+4]) {
                (dst+rij+4)->red = (unsigned short) (((unsigned int) (src+rij+4)->red + (unsigned int)(src+rij+4-1)->red + (unsigned int) (src+rij+4+1)->red + (unsigned int)(src+rij+4-dim)->red + (unsigned int)(src+rij+4-dim_ps1)->red + (unsigned int)(src+rij+4-dim_mn1)->red + (unsigned int)(src+rij+4+dim)->red + (unsigned int)(src+rij+4+dim_ps1)->red + (unsigned int)(src+rij+4+dim_mn1)->red)/9);
                (dst+rij+4)->green = (unsigned short) (((unsigned int) (src+rij+4)->green + (unsigned int) (src+rij+4-1)->green + (unsigned int) (src+rij+4+1)->green + (unsigned int) (src+rij+4-dim)->green  + (unsigned int)(src+rij+4-dim_ps1)->green + (unsigned int) (src+rij+4-dim_mn1)->green + (unsigned int)(src+rij+4+dim)->green + (unsigned int) (src+rij+4+dim_ps1)->green + (unsigned int)(src+rij+4+dim_mn1)->green)/9);
                (dst+rij+4)->blue = (unsigned short) (((unsigned int) (src+rij+4)->blue+ (unsigned int) (src+rij+4-1)->blue + (unsigned int) (src+rij+4+1)->blue+ (unsigned int) (src+rij+4-dim)->blue + (unsigned int) (src+rij+4-dim_ps1)->blue + (unsigned int) (src+rij+4-dim_mn1)->blue + (unsigned int) (src+rij+4+dim)->blue + (unsigned int) (src+rij+4+dim_ps1)->blue+ (unsigned int) (src+rij+4+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+4], src[rij+4])
            //rij++;
            if (!flt[rij+5]) {
                (dst+rij+5)->red = (unsigned short) (((unsigned int) (src+rij+5)->red + (unsigned int)(src+rij+5-1)->red + (unsigned int) (src+rij+5+1)->red + (unsigned int)(src+rij+5-dim)->red + (unsigned int)(src+rij+5-dim_ps1)->red + (unsigned int)(src+rij+5-dim_mn1)->red + (unsigned int)(src+rij+5+dim)->red + (unsigned int)(src+rij+5+dim_ps1)->red + (unsigned int)(src+rij+5+dim_mn1)->red)/9);
                (dst+rij+5)->green = (unsigned short) (((unsigned int) (src+rij+5)->green + (unsigned int) (src+rij+5-1)->green + (unsigned int) (src+rij+5+1)->green + (unsigned int) (src+rij+5-dim)->green  + (unsigned int)(src+rij+5-dim_ps1)->green + (unsigned int) (src+rij+5-dim_mn1)->green + (unsigned int)(src+rij+5+dim)->green + (unsigned int) (src+rij+5+dim_ps1)->green + (unsigned int)(src+rij+5+dim_mn1)->green)/9);
                (dst+rij+5)->blue = (unsigned short) (((unsigned int) (src+rij+5)->blue+ (unsigned int) (src+rij+5-1)->blue + (unsigned int) (src+rij+5+1)->blue+ (unsigned int) (src+rij+5-dim)->blue + (unsigned int) (src+rij+5-dim_ps1)->blue + (unsigned int) (src+rij+5-dim_mn1)->blue + (unsigned int) (src+rij+5+dim)->blue + (unsigned int) (src+rij+5+dim_ps1)->blue+ (unsigned int) (src+rij+5+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+5], src[rij+5])
            //rij++;
             if (!flt[rij+6]) {
                (dst+rij+6)->red = (unsigned short) (((unsigned int) (src+rij+6)->red + (unsigned int)(src+rij+6-1)->red + (unsigned int) (src+rij+6+1)->red + (unsigned int)(src+rij+6-dim)->red + (unsigned int)(src+rij+6-dim_ps1)->red + (unsigned int)(src+rij+6-dim_mn1)->red + (unsigned int)(src+rij+6+dim)->red + (unsigned int)(src+rij+6+dim_ps1)->red + (unsigned int)(src+rij+6+dim_mn1)->red)/9);
                (dst+rij+6)->green = (unsigned short) (((unsigned int) (src+rij+6)->green + (unsigned int) (src+rij+6-1)->green + (unsigned int) (src+rij+6+1)->green + (unsigned int) (src+rij+6-dim)->green  + (unsigned int)(src+rij+6-dim_ps1)->green + (unsigned int) (src+rij+6-dim_mn1)->green + (unsigned int)(src+rij+6+dim)->green + (unsigned int) (src+rij+6+dim_ps1)->green + (unsigned int)(src+rij+6+dim_mn1)->green)/9);
                (dst+rij+6)->blue = (unsigned short) (((unsigned int) (src+rij+6)->blue+ (unsigned int) (src+rij+6-1)->blue + (unsigned int) (src+rij+6+1)->blue+ (unsigned int) (src+rij+6-dim)->blue + (unsigned int) (src+rij+6-dim_ps1)->blue + (unsigned int) (src+rij+6-dim_mn1)->blue + (unsigned int) (src+rij+6+dim)->blue + (unsigned int) (src+rij+6+dim_ps1)->blue+ (unsigned int) (src+rij+6+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+6], src[rij+6])
            //rij++;
            if (!flt[rij+7]) {
                (dst+rij+7)->red = (unsigned short) (((unsigned int) (src+rij+7)->red + (unsigned int)(src+rij+7-1)->red + (unsigned int) (src+rij+7+1)->red + (unsigned int)(src+rij+7-dim)->red + (unsigned int)(src+rij+7-dim_ps1)->red + (unsigned int)(src+rij+7-dim_mn1)->red + (unsigned int)(src+rij+7+dim)->red + (unsigned int)(src+rij+7+dim_ps1)->red + (unsigned int)(src+rij+7+dim_mn1)->red)/9);
                (dst+rij+7)->green = (unsigned short) (((unsigned int) (src+rij+7)->green + (unsigned int) (src+rij+7-1)->green + (unsigned int) (src+rij+7+1)->green + (unsigned int) (src+rij+7-dim)->green  + (unsigned int)(src+rij+7-dim_ps1)->green + (unsigned int) (src+rij+7-dim_mn1)->green + (unsigned int)(src+rij+7+dim)->green + (unsigned int) (src+rij+7+dim_ps1)->green + (unsigned int)(src+rij+7+dim_mn1)->green)/9);
                (dst+rij+7)->blue = (unsigned short) (((unsigned int) (src+rij+7)->blue+ (unsigned int) (src+rij+7-1)->blue + (unsigned int) (src+rij+7+1)->blue+ (unsigned int) (src+rij+7-dim)->blue + (unsigned int) (src+rij+7-dim_ps1)->blue + (unsigned int) (src+rij+7-dim_mn1)->blue + (unsigned int) (src+rij+7+dim)->blue + (unsigned int) (src+rij+7+dim_ps1)->blue+ (unsigned int) (src+rij+7+dim_mn1)->blue)/9);
            } else EQUALIZE_PX(dst[rij+7], src[rij+7])
            rij+=8;
            //i+=8;
        }
        switch((dim - 2) % count1){
            case 6: 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                break;
            case 4: 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                break;         
            case 2: 
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
                if (!flt[rij]) {
                    (dst+rij)->red = (unsigned short) (((unsigned int) (src+rij)->red + (unsigned int)(src+rij-1)->red + (unsigned int) (src+rij+1)->red + (unsigned int)(src+rij-dim)->red + (unsigned int)(src+rij-dim_ps1)->red + (unsigned int)(src+rij-dim_mn1)->red + (unsigned int)(src+rij+dim)->red + (unsigned int)(src+rij+dim_ps1)->red + (unsigned int)(src+rij+dim_mn1)->red)/9);
                    (dst+rij)->green = (unsigned short) (((unsigned int) (src+rij)->green + (unsigned int) (src+rij-1)->green + (unsigned int) (src+rij+1)->green + (unsigned int) (src+rij-dim)->green  + (unsigned int)(src+rij-dim_ps1)->green + (unsigned int) (src+rij-dim_mn1)->green + (unsigned int)(src+rij+dim)->green + (unsigned int) (src+rij+dim_ps1)->green + (unsigned int)(src+rij+dim_mn1)->green)/9);
                    (dst+rij)->blue = (unsigned short) (((unsigned int) (src+rij)->blue+ (unsigned int) (src+rij-1)->blue + (unsigned int) (src+rij+1)->blue+ (unsigned int) (src+rij-dim)->blue + (unsigned int) (src+rij-dim_ps1)->blue + (unsigned int) (src+rij-dim_mn1)->blue + (unsigned int) (src+rij+dim)->blue + (unsigned int) (src+rij+dim_ps1)->blue+ (unsigned int) (src+rij+dim_mn1)->blue)/9);
                } else EQUALIZE_PX(dst[rij], src[rij])
                rij++;
        }
        // for (i = 1; i < dim_mn1; i+=2) {asdfasd
           

        // }
            
        rindex += dim;
    }
}

//typedef struct {
//    unsigned short red;
//    unsigned short green;
//    unsigned short blue;
//} XRGB;

typedef union {
    unsigned long long vec : 48;
    //XRGB rgb;
    pixel rgb;
} PIX;

#define PIX_VEC_AVG(p1, p2) ((((p1.vec ^ p2.vec) & 0xfffefffefffeU) >> 1) + (p1.vec & p2.vec) + ((p1.vec ^ p2.vec) & 0x000100010001U)) 
#define _PIX_VEC_AVG4(p1, p2) ((((p1.vec ^ p2.vec) & 0xfffefffefffeU) >> 1) + (p1.vec & p2.vec) + ((p1.vec ^ p2.vec) & 0x000200020002U)) 
//#define PIX_VEC_AVG4(p1, p2, p3, p4)

char bokeh5_descr[] = "bokeh5: vectorizatión!!!";
void bokeh5(int dim, pixel *src, short *flt, pixel *dst) 
{
    PIX p1, p2, p3, p4;


    int i, j, rij;
    pixel *src_orig = src;
    short *flt_orig = flt; 
    pixel *dst_orig = dst; 
    int rindex = dim;

     int dim_square = dim * dim;
    const int dim_mn1 = dim - 1;
    const int dim_ps1 = dim + 1;
    const int dim_times_dim = dim * 2;
    const int dim_square_mn1 = dim_square - 1;
    const int dim_square_mn_dim = dim_square - dim;
    const int dim_square_mn_2dim = dim_square - 2 * dim;

    //printf("\n\n\n\n\n\n\n\n\n\n%d: %d %d %d\n\n", flt[95], src[95].red, src[95].green, src[95].blue);
    //printf("\n\n\n\n\n\n\n\n\n\n%d: %d %d %d\n\n", flt[dim-2], src[dim-2].red, src[dim-2].green, src[dim-2].blue);
    //printf("\n\n\n\n\n\n\n\n\n\n%d: %d %d %d\n\n", flt[dim_times_dim-1], src[dim_times_dim-1].red, src[dim_times_dim-1].green, src[dim_times_dim-1].blue);
    //printf("\n\n\n\n\n\n\n\n\n\n%d: %d %d %d\n\n", flt[dim_times_dim-2], src[dim_times_dim-2].red, src[dim_times_dim-2].green, src[dim_times_dim-2].blue);
   
   
    // Outmost layer (corner cases)kjhkj
    if (!flt[0]) {
        p1.rgb = src[0];
        p2.rgb = src[1];
        p3.rgb = src[dim];
        p4.rgb = src[dim_ps1];

        p1.vec = PIX_VEC_AVG(p1, p2);
        p3.vec = PIX_VEC_AVG(p3, p4);
        p1.vec = _PIX_VEC_AVG4(p1, p3);

        dst[0] = p1.rgb;
    } else EQUALIZE_PX(dst[0],  src[0])
   
    if (!flt[dim_mn1]) {
        p1.rgb = src[dim_mn1];
        p2.rgb = src[dim-2];
        p3.rgb = src[dim_times_dim-1];
        p4.rgb = src[dim_times_dim-2];

        p1.vec = PIX_VEC_AVG(p1, p2);
        p3.vec = PIX_VEC_AVG(p3, p4);
        p1.vec = _PIX_VEC_AVG4(p1, p3);

        dst[dim_mn1] = p1.rgb;
    } else EQUALIZE_PX(dst[dim_mn1],  src[dim_mn1])

    if (!flt[dim_square_mn_dim]) {
        p1.rgb = src[dim_mn1];
        p2.rgb = src[dim-2];
        p3.rgb = src[dim_times_dim-1];
        p4.rgb = src[dim_times_dim-2];

        p1.vec = PIX_VEC_AVG(p1, p2);
        p3.vec = PIX_VEC_AVG(p3, p4);
        p1.vec = _PIX_VEC_AVG4(p1, p3);

        dst[dim_square_mn_dim] = p1.rgb;
    } else EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])

    if (!flt[dim_square_mn1]) {
        p1.rgb = src[dim_square_mn1];
        p2.rgb = src[dim_square-2];
        p3.rgb = src[dim_square_mn_dim-1];
        p4.rgb = src[dim_square_mn_dim-2];

        p1.vec = PIX_VEC_AVG(p1, p2);
        p3.vec = PIX_VEC_AVG(p3, p4);
        p1.vec = _PIX_VEC_AVG4(p1, p3);

        dst[dim_square_mn1] = p1.rgb;
    } else EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
    
    for (j = 1; j < dim - 1; j++) {
        if (!flt[j]) {
            dst[j].red = (unsigned short)(( (int) src[j].red + (int) src[j-1].red + (int) src[j+1].red + (int) src[j+dim].red + (int) src[j+dim_ps1].red + (int) src[j+dim_mn1].red)/6);
            dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-1].green+ (int) src[j+1].green+ (int) src[j+dim].green+ (int) src[j+dim_ps1].green+ (int) src[j+dim_mn1].green)/6);
            dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-1].blue+ (int) src[j+1].blue+ (int) src[j+dim].blue+ (int) src[j+dim_ps1].blue+ (int) src[j+dim_mn1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
       
    }

    for (j = dim_square_mn_dim + 1; j < dim_square_mn1; j++) {
        if (!flt[j]) {
            dst[j].red = (unsigned short)(( (int) src[j].red + (int) src[j-1].red + (int) src[j+1].red + (int) src[j-dim].red + (int) src[j-dim_mn1].red + (int) src[j-dim_ps1].red)/6);
            dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-1].green+ (int) src[j+1].green+ (int) src[j-dim].green+ (int) src[j-dim_mn1].green+ (int) src[j-dim_ps1].green)/6);
            dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-1].blue+ (int) src[j+1].blue+ (int) src[j-dim].blue+ (int) src[j-dim_mn1].blue+ (int) src[j-dim_ps1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (j = dim; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            dst[j].red = (unsigned short)(((int) src[j].red + (int) src[j-dim].red + (int) src[j+1].red + (int) src[j+dim].red + (int) src[j+dim_ps1].red + (int) src[j-dim_mn1].red)/6);
            dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-dim].green+ (int) src[j+1].green+ (int) src[j+dim].green+ (int) src[j+dim_ps1].green+ (int) src[j-dim_mn1].green)/6);
            dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-dim].blue+ (int) src[j+1].blue+ (int) src[j+dim].blue+ (int) src[j+dim_ps1].blue+ (int) src[j-dim_mn1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (j = dim_times_dim - 1; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            dst[j].red = (unsigned short) ((int) (src[j].red + (int)src[j-1].red + (int)src[j-dim].red +(int)src[j+dim].red + (int)src[j-dim_ps1].red + (int) src[j+dim_mn1].red)/6);
            dst[j].green =  (unsigned short) ((int) (src[j].green + (int) src[j-1].green + (int) src[j-dim].green + (int) src[j+dim].green + (int) src[j-dim_ps1].green + (int) src[j+dim_mn1].green)/6);
            dst[j].blue =  (unsigned short) ((int) (src[j].blue + (int) src[j-1].blue + (int)src[j-dim].blue + (int)src[j+dim].blue + (int)src[j-dim_ps1].blue + (int)src[j+dim_mn1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    // Inner layers
    for (i = 1; i < dim-1; i++) {
        for (j = 1; j < dim-1; j++) {
            rij = rindex+j;
        
            if (!flt[rij]) {
                dst[rij].red = (unsigned short) (((int) src[rij].red + (int)src[rij-1].red + (int) src[rij+1].red + (int)src[rij-dim].red + (int)src[rij-dim_ps1].red + (int)src[rij-dim_mn1].red + (int)src[rij+dim].red + (int)src[rij+dim_ps1].red + (int)src[rij+dim_mn1].red)/9);
                dst[rij].green = (unsigned short) (((int) src[rij].green + (int) src[rij-1].green + (int) src[rij+1].green + (int) src[rij-dim].green  + (int)src[rij-dim_ps1].green + (int) src[rij-dim_mn1].green + (int)src[rij+dim].green + (int) src[rij+dim_ps1].green + (int)src[rij+dim_mn1].green)/9);
                dst[rij].blue = (unsigned short) (((int) src[rij].blue+ (int) src[rij-1].blue + (int) src[rij+1].blue+src[rij-dim].blue + (int) src[rij-dim_ps1].blue + (int) src[rij-dim_mn1].blue + (int) src[rij+dim].blue + (int) src[rij+dim_ps1].blue+src[rij+dim_mn1].blue)/9);
            } else EQUALIZE_PX(dst[rij], src[rij])
        }
            
        rindex += dim;
    }
}

//typedef unsigned int _uint128_t __attribute__((mode(TI)));

//typedef uint64_t PACKEDPIX;
//#define PIXEL_TO_PACKEDPIX(pp, p) {pp = p.red << 40 + p.green << 20 + p.blue;}sadfas
//#define PACKEDPIX_TO_PIXEL(pp, p) {p.red = pp & 0x0fffff0000000000; p.green = pp & 0x000000fffff00000; p.blue = pp & 0x00000000000fffff;}


typedef union {
    uint64_t vec;
    //XRGB rgb;
    uint32_t split[2];
} XPIX;

XPIX XP1, XP2, XP3, XP4, XP5, XP6, XP7, XP8, XP9, XP10, XP11, XP12;

#define XPIX_VECTORIZE(s1, s2, s3, s4, s5, s6, s7, s8, s9, dd) {XP1.vec = (s1)->red << 32 + (s1)->green;\
                                                        XP2.vec = (s2)->red << 32 + (s2)->green;\
                                                        XP3.vec = (s3)->red << 32 + (s3)->green;\
                                                        XP4.vec = (s4)->red << 32 + (s4)->green;\
                                                        XP5.vec = (s5)->red << 32 + (s5)->green;\
                                                        XP6.vec = (s6)->red << 32 + (s6)->green;\
                                                        XP7.vec = (s7)->red << 32 + (s7)->green;\
                                                        XP8.vec = (s8)->red << 32 + (s8)->green;\
                                                        XP9.vec = (s1)->blue << 32 + (s2)->blue;\
                                                        XP10.vec = (s3)->blue << 32 + (s4)->blue;\
                                                        XP11.vec = (s5)->blue << 32 + (s6)->blue;\
                                                        XP12.vec = (s7)->blue << 32 + (s8)->blue;\
                                                        XP1.vec += XP2.vec + XP3.vec + XP4.vec + XP5.vec + XP6.vec + XP7.vec + XP8.vec;\
                                                        XP9.vec += XP10.vec + XP11.vec + XP12.vec;\
                                                        (dd)->red = (unsigned short) ((XP1.split[0] + (s9)->red) / 9);\
                                                        (dd)->green = (unsigned short) ((XP1.split[1] + (s9)->green) / 9);\
                                                        (dd)->blue = (unsigned short) ((XP9.split[0] + XP9.split[1] + (s9)->blue) / 9);\
                                                        }

#define _PROCESS_DATA if (!flt[rij]) XPIX_VECTORIZE(src+rij, src+rij-1, src+rij+1, src+rij-dim, src+rij-dim+1, src+rij-dim-1, src+rij+dim, src+rij+dim+1, src+rij+dim-1, dst+rij) else EQUALIZE_PX(dst[rij], src[rij]) rij++;

char bokeh_pix_descr[] = "bokeh pIXXXX!!!";
void bokeh_pix(int dim, pixel *src, short *flt, pixel *dst) 
{
    //printf("\n%ld\n", sizeof(pixel));
    //printf("\n%ld\n", sizeof(unsigned short));
    //printf("\n%ld\n", sizeof(unsigned long long));

    unsigned int i, j, rij;
    //pixel *src_orig = src;
    //short *flt_orig = flt; 
    //pixel *dst_orig = dst; 
    unsigned int rindex = dim;

    unsigned int dim_ps1 = dim + 1;
    unsigned int dim_mn1 = dim - 1;
    unsigned int dim_square = dim * dim;
    unsigned int dim_times_dim = dim * 2;
    unsigned int dim_square_mn1 = dim_square - 1;
    unsigned int dim_square_mn_dim = dim_square - dim;
    unsigned int dim_square_mn_2dim = dim_square - 2 * dim;


    // Outmost layer (corner cases)sadfas
    if (!flt[0]) {
        dst->red = (unsigned short)(((unsigned int)src->red +(unsigned int)(src+1)->red +(unsigned int)(src+dim)->red +(unsigned int)(src+dim+1)->red)>>2);
        dst->blue = (unsigned short)(((unsigned int) src->blue+(unsigned int) (src+1)->blue+ (unsigned int) (src+dim)->blue+ (unsigned int) (src+dim+1)->blue)>>2);
        dst->green = (unsigned short)(( (unsigned int) src->green+ (unsigned int) (src+1)->green+ (unsigned int) (src+dim)->green+ (unsigned int) (src+dim+1)->green)>>2);
    } else EQUALIZE_PX(dst[0],  src[0])
   
    if (!flt[dim-1]) {
        (dst+dim-1)->red = (unsigned short)(( (unsigned int) (src+dim-1)->red + (unsigned int) (src+dim-2)->red + (unsigned int) (src+dim_times_dim-1)->red + (unsigned int) (src+dim_times_dim-2)->red)>>2);
        (dst+dim-1)->blue = (unsigned short)(( (unsigned int) (src+dim-1)->blue+ (unsigned int) (src+dim-2)->blue+ (unsigned int) (src+dim_times_dim-1)->blue+ (unsigned int) (src+dim_times_dim-2)->blue)>>2);
        (dst+dim-1)->green = (unsigned short)(( (unsigned int) (src+dim-1)->green+ (unsigned int) (src+dim-2)->green+ (unsigned int) (src+dim_times_dim-1)->green+ (unsigned int) (src+dim_times_dim-2)->green)>>2);
    } else EQUALIZE_PX(dst[dim-1],  src[dim-1])

    if (!flt[dim_square_mn_dim]) {
        (dst+dim_square_mn_dim)->red = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->red + (unsigned int) (src+dim_square_mn_dim+1)->red + (unsigned int) (src+dim_square_mn_2dim)->red + (unsigned int) (src+dim_square_mn_2dim+1)->red)>>2);
        (dst+dim_square_mn_dim)->blue = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->blue+ (unsigned int) (src+dim_square_mn_dim+1)->blue+ (unsigned int) (src+dim_square_mn_2dim)->blue+ (unsigned int) (src+dim_square_mn_2dim+1)->blue)>>2);
        (dst+dim_square_mn_dim)->green = (unsigned short)(( (unsigned int) (src+dim_square_mn_dim)->green+ (unsigned int) (src+dim_square_mn_dim+1)->green+ (unsigned int) (src+dim_square_mn_2dim)->green+ (unsigned int) (src+dim_square_mn_2dim+1)->green)>>2);
    } else EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])

    if (!flt[dim_square_mn1]) {
        (dst+dim_square-1)->red = (unsigned short)(( (unsigned int) (src+dim_square-1)->red + (unsigned int) (src+dim_square-2)->red + (unsigned int) (src+dim_square_mn_dim-1)->red + (unsigned int) (src+dim_square_mn_dim-2)->red)>>2);
        (dst+dim_square-1)->blue = (unsigned short)(( (unsigned int) (src+dim_square-1)->blue+ (unsigned int) (src+dim_square-2)->blue+ (unsigned int) (src+dim_square_mn_dim-1)->blue+ (unsigned int) (src+dim_square_mn_dim-2)->blue)>>2);
        (dst+dim_square-1)->green = (unsigned short)(( (unsigned int) (src+dim_square-1)->green+ (unsigned int) (src+dim_square-2)->green+ (unsigned int) (src+dim_square_mn_dim-1)->green+ (unsigned int) (src+dim_square_mn_dim-2)->green)>>2);
    } else EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])

    for (j = dim_square_mn_dim + 1; j < dim_square_mn1; j++) {
        if (!flt[j]) {
            dst[j].red = (unsigned short)(( (int) src[j].red + (int) src[j-1].red + (int) src[j+1].red + (int) src[j-dim].red + (int) src[j-dim_mn1].red + (int) src[j-dim_ps1].red)/6);
            dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-1].green+ (int) src[j+1].green+ (int) src[j-dim].green+ (int) src[j-dim_mn1].green+ (int) src[j-dim_ps1].green)/6);
            dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-1].blue+ (int) src[j+1].blue+ (int) src[j-dim].blue+ (int) src[j-dim_mn1].blue+ (int) src[j-dim_ps1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (j = dim; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            dst[j].red = (unsigned short)(((int) src[j].red + (int) src[j-dim].red + (int) src[j+1].red + (int) src[j+dim].red + (int) src[j+dim_ps1].red + (int) src[j-dim_mn1].red)/6);
            dst[j].green = (unsigned short)(( (int) src[j].green+ (int) src[j-dim].green+ (int) src[j+1].green+ (int) src[j+dim].green+ (int) src[j+dim_ps1].green+ (int) src[j-dim_mn1].green)/6);
            dst[j].blue = (unsigned short)(( (int) src[j].blue+ (int) src[j-dim].blue+ (int) src[j+1].blue+ (int) src[j+dim].blue+ (int) src[j+dim_ps1].blue+ (int) src[j-dim_mn1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    for (j = dim_times_dim - 1; j < dim_square_mn_dim; j += dim) {
        if (!flt[j]) {
            dst[j].red = (unsigned short) ((int) (src[j].red + (int)src[j-1].red + (int)src[j-dim].red +(int)src[j+dim].red + (int)src[j-dim_ps1].red + (int) src[j+dim_mn1].red)/6);
            dst[j].green =  (unsigned short) ((int) (src[j].green + (int) src[j-1].green + (int) src[j-dim].green + (int) src[j+dim].green + (int) src[j-dim_ps1].green + (int) src[j+dim_mn1].green)/6);
            dst[j].blue =  (unsigned short) ((int) (src[j].blue + (int) src[j-1].blue + (int)src[j-dim].blue + (int)src[j+dim].blue + (int)src[j-dim_ps1].blue + (int)src[j+dim_mn1].blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])
    }

    int count1 = (dim - 2) >> 3; 
    int left_overs = dim - count1 << 3; 
    // Inner layers
    for (j = 1; j < dim-1; j++) {
        rij = rindex + 1;

        if (!flt[j]) {
            (dst+j)->red = (unsigned short)(( (unsigned int) (src+j)->red + (unsigned int) (src+j-1)->red + (unsigned int) (src+j+1)->red + (unsigned int) (src+j+dim)->red + (unsigned int) (src+j+dim+1)->red + (unsigned int) (src+j+dim-1)->red)/6);
            (dst+j)->green = (unsigned short)(( (unsigned int) (src+j)->green+ (unsigned int) (src+j-1)->green+ (unsigned int) (src+j+1)->green+ (unsigned int) (src+j+dim)->green+ (unsigned int) (src+j+dim+1)->green+ (unsigned int) (src+j+dim-1)->green)/6);
            (dst+j)->blue = (unsigned short)(( (unsigned int) (src+j)->blue+ (unsigned int) (src+j-1)->blue+ (unsigned int) (src+j+1)->blue+ (unsigned int) (src+j+dim)->blue+ (unsigned int) (src+j+dim+1)->blue+ (unsigned int) (src+j+dim-1)->blue)/6);
        } else EQUALIZE_PX(dst[j], src[j])

        i = 1; 
        int fake_count = count1;
        while(fake_count--){
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            _PROCESS_DATA
            i+=8;
        }
        switch((dim - 2) % count1){
            case 6: 
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                break;
            case 4: 
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                _PROCESS_DATA
                break;         
            case 2: 
                _PROCESS_DATA
                _PROCESS_DATA
        }
            
        rindex += dim;
    }
}


char bokeh_withjumps_descr[] = "JUMPS!!!";
void bokeh_withjumps(int dim, pixel *src, short *flt, pixel *dst) {
    unsigned int i, j, rij;
    pixel *src_orig = src;
    short *flt_orig = flt; 
    pixel *dst_orig = dst; 
    unsigned int rindex = dim;

    const unsigned int dim_square = dim * dim;
    const unsigned int dim_mn1 = dim - 1;
    const unsigned int dim_ps1 = dim + 1;
    const unsigned int dim_times_dim = dim * 2;
    const unsigned int dim_square_mn1 = dim_square - 1;
    const unsigned int dim_square_mn_dim = dim_square - dim;
    const unsigned int dim_square_mn_2dim = dim_square - 2 * dim;

    void *jt1[2] = {&&jnP1, &&jP1};
    void *jt2[2] = {&&jnP2, &&jP2};
    void *jt3[2] = {&&jnP3, &&jP3};
    void *jt4[2] = {&&jnP4, &&jP4};
    void *jt5[2] = {&&jnP5, &&jP5};
    void *jt6[2] = {&&jnP6, &&jP6};
    void *jt7[2] = {&&jnP7, &&jP7};
    void *jt8[2] = {&&jnP8, &&jP8};
    void *jt9[2] = {&&jnP9, &&jP9};

    goto *jt1[flt[0]];
jnP1:
    dst[0].red = (unsigned short)(((unsigned int)src[0].red +(unsigned int)src[1].red +(unsigned int)src[dim].red +(unsigned int)src[dim_ps1].red)>>2);
    dst[0].blue = (unsigned short)(((unsigned int) src[0].blue+(unsigned int) src[1].blue+ (unsigned int) src[dim].blue+ (unsigned int) src[dim_ps1].blue)>>2);
    dst[0].green = (unsigned short)(( (unsigned int) src[0].green+ (unsigned int) src[1].green+ (unsigned int) src[dim].green+ (unsigned int) src[dim_ps1].green)>>2);
    goto jT2;
jP1: EQUALIZE_PX(dst[0],  src[0])
jT2:
    goto *jt2[flt[dim_mn1]];
jnP2:
    dst[dim_mn1].red = (unsigned short)(( (unsigned int) src[dim_mn1].red + (unsigned int) src[dim-2].red + (unsigned int) src[dim_times_dim-1].red + (unsigned int) src[dim_times_dim-2].red)>>2);
    dst[dim_mn1].blue = (unsigned short)(( (unsigned int) src[dim_mn1].blue+ (unsigned int) src[dim-2].blue+ (unsigned int) src[dim_times_dim-1].blue+ (unsigned int) src[dim_times_dim-2].blue)>>2);
    dst[dim_mn1].green = (unsigned short)(( (unsigned int) src[dim_mn1].green+ (unsigned int) src[dim-2].green+ (unsigned int) src[dim_times_dim-1].green+ (unsigned int) src[dim_times_dim-2].green)>>2);
    goto jT3;
jP2: EQUALIZE_PX(dst[dim_mn1],  src[dim_mn1])
jT3:
    goto *jt3[flt[dim_square_mn_dim]];
jnP3:
    dst[dim_square_mn_dim].red = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].red + (unsigned int) src[dim_square_mn_dim+1].red + (unsigned int) src[dim_square_mn_2dim].red + (unsigned int) src[dim_square_mn_2dim+1].red)>>2);
    dst[dim_square_mn_dim].blue = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].blue+ (unsigned int) src[dim_square_mn_dim+1].blue+ (unsigned int) src[dim_square_mn_2dim].blue+ (unsigned int) src[dim_square_mn_2dim+1].blue)>>2);
    dst[dim_square_mn_dim].green = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].green+ (unsigned int) src[dim_square_mn_dim+1].green+ (unsigned int) src[dim_square_mn_2dim].green+ (unsigned int) src[dim_square_mn_2dim+1].green)>>2);
    goto jT4;
jP3: EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])
jT4:
    goto *jt4[flt[dim_square_mn1]];
jnP4:
    dst[dim_square-1].red = (unsigned short)(( (unsigned int) src[dim_square-1].red + (unsigned int) src[dim_square-2].red + (unsigned int) src[dim_square_mn_dim-1].red + (unsigned int) src[dim_square_mn_dim-2].red)>>2);
    dst[dim_square-1].blue = (unsigned short)(( (unsigned int) src[dim_square-1].blue+ (unsigned int) src[dim_square-2].blue+ (unsigned int) src[dim_square_mn_dim-1].blue+ (unsigned int) src[dim_square_mn_dim-2].blue)>>2);
    dst[dim_square-1].green = (unsigned short)(( (unsigned int) src[dim_square-1].green+ (unsigned int) src[dim_square-2].green+ (unsigned int) src[dim_square_mn_dim-1].green+ (unsigned int) src[dim_square_mn_dim-2].green)>>2);
    goto jT5;
jP4: EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
jT5:
    for (j = 1; j < dim - 1; j++) {
        goto *jt5[flt[j]];
jnP5:
        dst[j].red = (unsigned short)(( (unsigned int) src[j].red + (unsigned int) src[j-1].red + (unsigned int) src[j+1].red + (unsigned int) src[j+dim].red + (unsigned int) src[j+dim_ps1].red + (unsigned int) src[j+dim_mn1].red)/6);
        dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-1].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j+dim].green+ (unsigned int) src[j+dim_ps1].green+ (unsigned int) src[j+dim_mn1].green)/6);
        dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-1].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j+dim].blue+ (unsigned int) src[j+dim_ps1].blue+ (unsigned int) src[j+dim_mn1].blue)/6);
        goto jT6;
jP5: EQUALIZE_PX(dst[j], src[j])
jT6: ;
    }

    for (j = dim_square_mn_dim + 1; j < dim_square_mn1; j++) {
        goto *jt6[flt[j]];
jnP6:
        dst[j].red = (unsigned short)(( (unsigned int) src[j].red + (unsigned int) src[j-1].red + (unsigned int) src[j+1].red + (unsigned int) src[j-dim].red + (unsigned int) src[j-dim_mn1].red + (unsigned int) src[j-dim_ps1].red)/6);
        dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-1].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j-dim].green+ (unsigned int) src[j-dim_mn1].green+ (unsigned int) src[j-dim_ps1].green)/6);
        dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-1].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j-dim].blue+ (unsigned int) src[j-dim_mn1].blue+ (unsigned int) src[j-dim_ps1].blue)/6);
        goto jT7;
jP6: EQUALIZE_PX(dst[j], src[j])
jT7: ;
    }

    for (j = dim; j < dim_square_mn_dim; j += dim) {
        goto *jt7[flt[j]];
jnP7:
        dst[j].red = (unsigned short)(((unsigned int) src[j].red + (unsigned int) src[j-dim].red + (unsigned int) src[j+1].red + (unsigned int) src[j+dim].red + (unsigned int) src[j+dim_ps1].red + (unsigned int) src[j-dim_mn1].red)/6);
        dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-dim].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j+dim].green+ (unsigned int) src[j+dim_ps1].green+ (unsigned int) src[j-dim_mn1].green)/6);
        dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-dim].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j+dim].blue+ (unsigned int) src[j+dim_ps1].blue+ (unsigned int) src[j-dim_mn1].blue)/6);
        goto jT8;
jP7: EQUALIZE_PX(dst[j], src[j])
jT8: ;
    }

    for (j = dim_times_dim - 1; j < dim_square_mn_dim; j += dim) {
        goto *jt8[flt[j]];
jnP8:
        dst[j].red = (unsigned short) ((unsigned int) (src[j].red + (unsigned int)src[j-1].red + (unsigned int)src[j-dim].red +(unsigned int)src[j+dim].red + (unsigned int)src[j-dim_ps1].red + (unsigned int) src[j+dim_mn1].red)/6);
        dst[j].green =  (unsigned short) ((unsigned int) (src[j].green + (unsigned int) src[j-1].green + (unsigned int) src[j-dim].green + (unsigned int) src[j+dim].green + (unsigned int) src[j-dim_ps1].green + (unsigned int) src[j+dim_mn1].green)/6);
        dst[j].blue =  (unsigned short) ((unsigned int) (src[j].blue + (unsigned int) src[j-1].blue + (unsigned int)src[j-dim].blue + (unsigned int)src[j+dim].blue + (unsigned int)src[j-dim_ps1].blue + (unsigned int)src[j+dim_mn1].blue)/6);
        goto jT9;
jP8: EQUALIZE_PX(dst[j], src[j])
jT9: ;
    }

    for (i = 1; i < dim_mn1; i++) {
        rij = rindex + 1;

        for (j = 1; j < dim_mn1; j++) {
            goto *jt9[flt[rij]];
jnP9:
            dst[rij].red = (unsigned short) (((unsigned int) src[rij].red + (unsigned int)src[rij-1].red + (unsigned int) src[rij+1].red + (unsigned int)src[rij-dim].red + (unsigned int)src[rij-dim_ps1].red + (unsigned int)src[rij-dim_mn1].red + (unsigned int)src[rij+dim].red + (unsigned int)src[rij+dim_ps1].red + (unsigned int)src[rij+dim_mn1].red)/9);
            dst[rij].green = (unsigned short) (((unsigned int) src[rij].green + (unsigned int) src[rij-1].green + (unsigned int) src[rij+1].green + (unsigned int) src[rij-dim].green  + (unsigned int)src[rij-dim_ps1].green + (unsigned int) src[rij-dim_mn1].green + (unsigned int)src[rij+dim].green + (unsigned int) src[rij+dim_ps1].green + (unsigned int)src[rij+dim_mn1].green)/9);
            dst[rij].blue = (unsigned short) (((unsigned int) src[rij].blue+ (unsigned int) src[rij-1].blue + (unsigned int) src[rij+1].blue+src[rij-dim].blue + (unsigned int) src[rij-dim_ps1].blue + (unsigned int) src[rij-dim_mn1].blue + (unsigned int) src[rij+dim].blue + (unsigned int) src[rij+dim_ps1].blue+src[rij+dim_mn1].blue)/9);
            goto jT10;
jP9: EQUALIZE_PX(dst[rij], src[rij])
jT10:
            rij++;
        }
            
        rindex += dim;
    }
}

// typedef struct {
//         uint8_t data[16];
// } uint128_t;

// typedef union {
//     uint128_t vec : 96;
//     uint_fast32_t rgb[3];
// } __attribute__((packed)) PIX96;

// #define PIX96_INIT(p, p96) {p96.rgb[0] = (unsigned int) p.red; p96.rgb[1] = (unsigned int) p.green; p96.rgb[2] = (unsigned int) p.blue;}

// char bokeh6_descr[] = "bokeh6: :///";
// void bokeh6(int dim, pixel *src, short *flt, pixel *dst) {
//     unsigned int i, j, rij;
//     pixel *src_orig = src;
//     short *flt_orig = flt; 
//     pixel *dst_orig = dst; 
//     unsigned int rindex = dim;

//     const unsigned int dim_square = dim * dim;
//     const unsigned int dim_mn1 = dim - 1;
//     const unsigned int dim_ps1 = dim + 1;
//     const unsigned int dim_times_dim = dim * 2;
//     const unsigned int dim_square_mn1 = dim_square - 1;
//     const unsigned int dim_square_mn_dim = dim_square - dim;
//     const unsigned int dim_square_mn_2dim = dim_square - 2 * dim;


//     // Outmost layer (corner cases)
//     if (!flt[0]) {
//         dst[0].red = (unsigned short)(((unsigned int)src[0].red +(unsigned int)src[1].red +(unsigned int)src[dim].red +(unsigned int)src[dim_ps1].red)>>2);
//         dst[0].blue = (unsigned short)(((unsigned int) src[0].blue+(unsigned int) src[1].blue+ (unsigned int) src[dim].blue+ (unsigned int) src[dim_ps1].blue)>>2);
//         dst[0].green = (unsigned short)(( (unsigned int) src[0].green+ (unsigned int) src[1].green+ (unsigned int) src[dim].green+ (unsigned int) src[dim_ps1].green)>>2);
//     } else EQUALIZE_PX(dst[0],  src[0])
   
//     if (!flt[dim_mn1]) {
//         dst[dim_mn1].red = (unsigned short)(( (unsigned int) src[dim_mn1].red + (unsigned int) src[dim-2].red + (unsigned int) src[dim_times_dim-1].red + (unsigned int) src[dim_times_dim-2].red)>>2);
//         dst[dim_mn1].blue = (unsigned short)(( (unsigned int) src[dim_mn1].blue+ (unsigned int) src[dim-2].blue+ (unsigned int) src[dim_times_dim-1].blue+ (unsigned int) src[dim_times_dim-2].blue)>>2);
//         dst[dim_mn1].green = (unsigned short)(( (unsigned int) src[dim_mn1].green+ (unsigned int) src[dim-2].green+ (unsigned int) src[dim_times_dim-1].green+ (unsigned int) src[dim_times_dim-2].green)>>2);
//     } else EQUALIZE_PX(dst[dim_mn1],  src[dim_mn1])

//     if (!flt[dim_square_mn_dim]) {
//         dst[dim_square_mn_dim].red = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].red + (unsigned int) src[dim_square_mn_dim+1].red + (unsigned int) src[dim_square_mn_2dim].red + (unsigned int) src[dim_square_mn_2dim+1].red)>>2);
//         dst[dim_square_mn_dim].blue = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].blue+ (unsigned int) src[dim_square_mn_dim+1].blue+ (unsigned int) src[dim_square_mn_2dim].blue+ (unsigned int) src[dim_square_mn_2dim+1].blue)>>2);
//         dst[dim_square_mn_dim].green = (unsigned short)(( (unsigned int) src[dim_square_mn_dim].green+ (unsigned int) src[dim_square_mn_dim+1].green+ (unsigned int) src[dim_square_mn_2dim].green+ (unsigned int) src[dim_square_mn_2dim+1].green)>>2);
//     } else EQUALIZE_PX(dst[dim_square_mn_dim], src[dim_square_mn_dim])

//     if (!flt[dim_square_mn1]) {
//         dst[dim_square-1].red = (unsigned short)(( (unsigned int) src[dim_square-1].red + (unsigned int) src[dim_square-2].red + (unsigned int) src[dim_square_mn_dim-1].red + (unsigned int) src[dim_square_mn_dim-2].red)>>2);
//         dst[dim_square-1].blue = (unsigned short)(( (unsigned int) src[dim_square-1].blue+ (unsigned int) src[dim_square-2].blue+ (unsigned int) src[dim_square_mn_dim-1].blue+ (unsigned int) src[dim_square_mn_dim-2].blue)>>2);
//         dst[dim_square-1].green = (unsigned short)(( (unsigned int) src[dim_square-1].green+ (unsigned int) src[dim_square-2].green+ (unsigned int) src[dim_square_mn_dim-1].green+ (unsigned int) src[dim_square_mn_dim-2].green)>>2);
//     } else EQUALIZE_PX(dst[dim_square_mn1],  src[dim_square_mn1])
    
//     for (j = 1; j < dim - 1; j++) {
//         if (!flt[j]) {
//             dst[j].red = (unsigned short)(( (unsigned int) src[j].red + (unsigned int) src[j-1].red + (unsigned int) src[j+1].red + (unsigned int) src[j+dim].red + (unsigned int) src[j+dim_ps1].red + (unsigned int) src[j+dim_mn1].red)/6);
//             dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-1].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j+dim].green+ (unsigned int) src[j+dim_ps1].green+ (unsigned int) src[j+dim_mn1].green)/6);
//             dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-1].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j+dim].blue+ (unsigned int) src[j+dim_ps1].blue+ (unsigned int) src[j+dim_mn1].blue)/6);
//         } else EQUALIZE_PX(dst[j], src[j])
       
//     }

//     for (j = dim_square_mn_dim + 1; j < dim_square_mn1; j++) {
//         if (!flt[j]) {
//             dst[j].red = (unsigned short)(( (unsigned int) src[j].red + (unsigned int) src[j-1].red + (unsigned int) src[j+1].red + (unsigned int) src[j-dim].red + (unsigned int) src[j-dim_mn1].red + (unsigned int) src[j-dim_ps1].red)/6);
//             dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-1].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j-dim].green+ (unsigned int) src[j-dim_mn1].green+ (unsigned int) src[j-dim_ps1].green)/6);
//             dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-1].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j-dim].blue+ (unsigned int) src[j-dim_mn1].blue+ (unsigned int) src[j-dim_ps1].blue)/6);
//         } else EQUALIZE_PX(dst[j], src[j])
//     }

//     for (j = dim; j < dim_square_mn_dim; j += dim) {
//         if (!flt[j]) {
//             dst[j].red = (unsigned short)(((unsigned int) src[j].red + (unsigned int) src[j-dim].red + (unsigned int) src[j+1].red + (unsigned int) src[j+dim].red + (unsigned int) src[j+dim_ps1].red + (unsigned int) src[j-dim_mn1].red)/6);
//             dst[j].green = (unsigned short)(( (unsigned int) src[j].green+ (unsigned int) src[j-dim].green+ (unsigned int) src[j+1].green+ (unsigned int) src[j+dim].green+ (unsigned int) src[j+dim_ps1].green+ (unsigned int) src[j-dim_mn1].green)/6);
//             dst[j].blue = (unsigned short)(( (unsigned int) src[j].blue+ (unsigned int) src[j-dim].blue+ (unsigned int) src[j+1].blue+ (unsigned int) src[j+dim].blue+ (unsigned int) src[j+dim_ps1].blue+ (unsigned int) src[j-dim_mn1].blue)/6);
//         } else EQUALIZE_PX(dst[j], src[j])
//     }

//     for (j = dim_times_dim - 1; j < dim_square_mn_dim; j += dim) {
//         if (!flt[j]) {
//             dst[j].red = (unsigned short) ((unsigned int) (src[j].red + (unsigned int)src[j-1].red + (unsigned int)src[j-dim].red +(unsigned int)src[j+dim].red + (unsigned int)src[j-dim_ps1].red + (unsigned int) src[j+dim_mn1].red)/6);
//             dst[j].green =  (unsigned short) ((unsigned int) (src[j].green + (unsigned int) src[j-1].green + (unsigned int) src[j-dim].green + (unsigned int) src[j+dim].green + (unsigned int) src[j-dim_ps1].green + (unsigned int) src[j+dim_mn1].green)/6);
//             dst[j].blue =  (unsigned short) ((unsigned int) (src[j].blue + (unsigned int) src[j-1].blue + (unsigned int)src[j-dim].blue + (unsigned int)src[j+dim].blue + (unsigned int)src[j-dim_ps1].blue + (unsigned int)src[j+dim_mn1].blue)/6);
//         } else EQUALIZE_PX(dst[j], src[j])
//     }

//     PIX96 p1, p2, p3, p4, p5, p6, p7, p8, p9;

//     // Inner layers
//     for (i = 1; i < dim_mn1; i++) {
//         for (j = 1; j < dim_mn1; j++) {
//             rij = rindex+j;
        
//             if (!flt[rij]) {
//                 PIX96_INIT(src[rij], p1)
//                 PIX96_INIT(src[rij-1], p2)
//                 PIX96_INIT(src[rij+1], p3)
//                 PIX96_INIT(src[rij-dim], p4)
//                 PIX96_INIT(src[rij-dim_ps1], p5)
//                 PIX96_INIT(src[rij-dim_mn1], p6)
//                 PIX96_INIT(src[rij+dim], p7)
//                 PIX96_INIT(src[rij+dim_ps1], p8)
//                 PIX96_INIT(src[rij+dim_mn1], p9)

//                 p1.vec += p2.vec + p3.vec + p4.vec + p5.vec + p6.vec + p7.vec + p8.vec + p9.vec;

//                 dst[rij].red = (unsigned short) (p1.rgb[0] / 9);
//                 dst[rij].green = (unsigned short) (p1.rgb[1] / 9);
//                 dst[rij].blue = (unsigned short) (p1.rgb[2] / 9);

//                 //dst[rij].red = (unsigned short) (((unsigned int) src[rij].red + (unsigned int)src[rij-1].red + (unsigned int) src[rij+1].red + (unsigned int)src[rij-dim].red + (unsigned int)src[rij-dim_ps1].red + (unsigned int)src[rij-dim_mn1].red + (unsigned int)src[rij+dim].red + (unsigned int)src[rij+dim_ps1].red + (unsigned int)src[rij+dim_mn1].red)/9);
//                 //dst[rij].green = (unsigned short) (((unsigned int) src[rij].green + (unsigned int) src[rij-1].green + (unsigned int) src[rij+1].green + (unsigned int) src[rij-dim].green  + (unsigned int)src[rij-dim_ps1].green + (unsigned int) src[rij-dim_mn1].green + (unsigned int)src[rij+dim].green + (unsigned int) src[rij+dim_ps1].green + (unsigned int)src[rij+dim_mn1].green)/9);
//                 //dst[rij].blue = (unsigned short) (((unsigned int) src[rij].blue+ (unsigned int) src[rij-1].blue + (unsigned int) src[rij+1].blue+src[rij-dim].blue + (unsigned int) src[rij-dim_ps1].blue + (unsigned int) src[rij-dim_mn1].blue + (unsigned int) src[rij+dim].blue + (unsigned int) src[rij+dim_ps1].blue+src[rij+dim_mn1].blue)/9);
//             } else EQUALIZE_PX(dst[rij], src[rij])
//         }
            
//         rindex += dim;
//     }
// }

/*********************************************************************
 * register_bokeh_functions - Register all of your different versions
 *     of the bokeh kernel with the driver by calling the
 *     add_bokeh_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_bokeh_functions() 
{
    add_bokeh_function(&naive_bokeh, naive_bokeh_descr);   
    // add_bokeh_function(&bokeh, bokeh_descr);   
    // add_bokeh_function(&bokeh2, bokeh2_descr);   
    // add_bokeh_function(&bokeh3, bokeh3_descr);   
    add_bokeh_function(&bokeh4, bokeh4_descr);   
    //add_bokeh_function(&bokeh_pix, bokeh_pix_descr);  
    //add_bokeh_function(&bokeh_withjumps, bokeh_withjumps_descr); 
    //add_bokeh_function(&bokeh5, bokeh5_descr); 
    //add_bokeh_function(&bokeh6, bokeh6_descr);   
    // add_bokeh_function(&bokeh5, bokeh5_descr);   
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
    //naive_hadamard(dim, src1, src2, dst);
    //return;
    //int i, j;
    //
    //for(i = 0; i < dim; i++)
    //    for(j = 0; j < dim; j++) 
    //        dst[RIDX(i, j, dim)] = src1[RIDX(i, j, dim)] * src2[RIDX(i, j, dim)];
    
    //register elem_count = dim * dim;
    //register n = (elem_count + 31) / 32;
//
    int elem_count = dim * dim;
    int n = (elem_count + 53) / 64;

    switch (elem_count % 64) {
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

