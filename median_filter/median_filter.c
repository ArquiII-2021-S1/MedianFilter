#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_sort_vector.h>
#include "handle_image.c"


/**
 * This function searchs the median value for a specific pixel in (i,j)
 * image: gsl_matrix with the original image
 * i: position in x of the central pixel
 * j: position in y of the central pixel
 * window_size: 1 -> 3 x 3 window
 *              2 -> 5 x 5 window
 *              3 -> 7 x 7 window
 *              4 -> 9 x 9 window 
 * returns: median value
*/
int *get_median_value(gsl_matrix *image, int i, int j, int window_size)
{
    int x_start = i - window_size;
    if (x_start < 0)
        x_start = 0; // Check left limit

    int x_end = i + window_size;
    if (x_end >= image->size1)
        x_end = image->size1 - 1; // Check right limit

    int y_start = j - window_size;
    if (y_start < 0)
        y_start = 0; // Check top limit

    int y_end = j + window_size;
    if (y_end >= image->size2)
        y_end = image->size2 - 1; // Check down limit

    gsl_vector *neighborhood = gsl_vector_alloc((x_end - x_start + 1)*(y_end - y_start + 1));
    int counter = 0;

    for (int x = x_start; x <= x_end; x++)
    {
        for (int y = y_start; y <= y_end; y++)
        {
            // Get the (x,y) pixel
            double pixel = gsl_matrix_get(image, x, y);

            // Stores it in the neighborhood
            gsl_vector_set(neighborhood, counter, pixel);
            counter++;
        }
    }

    // Sorts the elements of neighborhood into ascending numerical order
    gsl_sort_vector(neighborhood);
    
    // Gets median value
    int median = gsl_vector_get(neighborhood, neighborhood->size / 2);

    return median;
}

int main()
{
    gsl_matrix *image = read_image("test2.png");

    gsl_vector* neighborhood = get_window_elements(image, 5, 0, 1);

    /*for (int i = 0; i < image->size1; i++)
    {
        for (int j = 0; j < image->size2; j++)
        {
            int pixel = gsl_matrix_get(image, i, j);
            printf("%d ", pixel);
        }
        printf("\n");
    }*/
    

    return 0;
}