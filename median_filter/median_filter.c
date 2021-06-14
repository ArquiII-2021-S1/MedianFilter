#include <stdio.h>
#include <omp.h>
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
int get_median_value(Image image, int i, int j, int window_size)
{
    int x_start = i - window_size;
    if (x_start < 0)
        x_start = 0; // Check left limit

    int x_end = i + window_size;
    if (x_end >= image.rows)
        x_end = image.rows - 1; // Check right limit

    int y_start = j - window_size;
    if (y_start < 0)
        y_start = 0; // Check top limit

    int y_end = j + window_size;
    if (y_end >= image.cols)
        y_end = image.cols - 1; // Check down limit

    gsl_vector *neighborhood = gsl_vector_alloc((x_end - x_start + 1) * (y_end - y_start + 1));
    int counter = 0;

    for (int x = x_start; x <= x_end; x++)
    {
        for (int y = y_start; y <= y_end; y++)
        {
            // Get the (x,y) pixel
            int pixel = image.data[x][y];

            // Stores it in the neighborhood
            gsl_vector_set(neighborhood, counter, pixel);
            counter++;
        }
    }

    // Sorts the elements of neighborhood into ascending numerical order
    gsl_sort_vector(neighborhood);

    // Gets median value
    int median = gsl_vector_get(neighborhood, neighborhood->size / 2);

    free(neighborhood);
    return median;
}

/** 
 * This function applies the median filter to an input image
 * image: gsl_matrix with the original image
 * window_size: 1 -> 3 x 3 window
 *              2 -> 5 x 5 window
 *              3 -> 7 x 7 window
 *              4 -> 9 x 9 window 
 * returns: gsl_matrix with the filtered image
*/
gsl_matrix *median_filter(Image image, int window_size)
{
    // Create new matrix to store the filtered image
    gsl_matrix *filtered = gsl_matrix_alloc(image.rows, image.cols);

    #pragma omp parallel
    {
        // Iterates over the image to calculate the median values
        #pragma omp for collapse(2)
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                int median = get_median_value(image, i, j, window_size);

                gsl_matrix_set(filtered, i, j, median);
            }
        }
    }

    return filtered;
}

int main()
{
    Image image = read_image("../noise/ruido.png");
    //gsl_matrix *image = read_image("../noise/beach.png");

    double start_time, run_time;
    start_time = omp_get_wtime();

    gsl_matrix *filtered_image = median_filter(image, 2);

    run_time = omp_get_wtime() - start_time;
    printf("Tiempo: %f\n", run_time);

    write_image("filtered.png", filtered_image);

    /*for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            int pixel = gsl_matrix_get(image, i, j);
            printf("%d ", pixel);
        }
        printf("\n");
    }*/

    return 0;
}