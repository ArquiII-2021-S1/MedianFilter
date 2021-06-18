#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "handle_image.c"

#define WINDOW_SIZE 1
#define NEIGHBORHOOD_SIZE ((WINDOW_SIZE*2+1)*(WINDOW_SIZE*2+1))

struct stat st = {0};
// TODO: eliminar sort y usar funcion para calcular la mediana

// TODO: cómo se ejecutaría en el GPU?
void bubble_sort(int n, int *array)
{
    int temp;
    //Sort the array in ascending order
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (array[i] > array[j])
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

// void swap(int *p,int *q) {
//    int t;

//    t=*p;
//    *p=*q;
//    *q=t;
// }

// void sort(int a[],int n) {
//    int i,j,temp;

//    for(i = 0;i < n-1;i++) {
//       for(j = 0;j < n-i-1;j++) {
//          if(a[j] > a[j+1])
//             swap(&a[j],&a[j+1]);
//       }
//    }
// }

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
int get_median_value_center(Image *image, int i, int j, int window_size)
{
    int x_start = i - window_size;
    int x_end = i + window_size;
    int y_start = j - window_size;
    int y_end = j + window_size;


    // int x_start = i - window_size;
    // if (x_start < 0)
    //     x_start = 0; // Check left limit

    // int x_end = i + window_size;
    // if (x_end >= IMAGE_M)
    //     x_end = IMAGE_M - 1; // Check right limit

    // int y_start = j - window_size;
    // if (y_start < 0)
    //     y_start = 0; // Check top limit

    // int y_end = j + window_size;
    // if (y_end >= IMAGE_N)
    //     y_end = IMAGE_N - 1; // Check down limit

    // int elements = (x_end - x_start + 1) * (y_end - y_start + 1);
    // int *neighborhood = (int *)malloc(sizeof(int) * elements);
    int neighborhood[NEIGHBORHOOD_SIZE];

    int counter = 0;

    for (int x = x_start; x <= x_end; x++)
    {
        for (int y = y_start; y <= y_end; y++)
        {
            // Get the (x,y) pixel
            int pixel = image->data[x][y];

            // Stores it in the neighborhood
            neighborhood[counter] = pixel;
            counter++;
        }
    }

    // Sorts the elements of neighborhood into ascending numerical order
    bubble_sort(NEIGHBORHOOD_SIZE, neighborhood);

    // Gets median value
    int median = neighborhood[NEIGHBORHOOD_SIZE / 2];

    // free(neighborhood);
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
Image *median_filter(Image *image, int window_size)
{
    // Image filtered;
    CREATE_IMAGE(filtered)


    // TODO: separar por esquinas, bordes y centro
    // Iterates over the image to calculate the median values
    for (int i = 0; i < IMAGE_M; i++)
    {
        for (int j = 0; j < IMAGE_N; j++)
        {
            int median = get_median_value_center(image, i, j, window_size);

            filtered->data[i][j] = median;
        }
    }

    return filtered;
}

int process_files(const char *input_directory, int file_amount)
{
    // Creates Filtered folder if it doesnt exist
    if (stat("../filtered", &st) == -1)
    {
        mkdir("../filtered", 0700);
    }

    // double start_time, run_time;
    // start_time = omp_get_wtime();

    // Iterates over the image to calculate the median values
    for (int i = 0; i < file_amount; i++)
    {


        char *filename;
        asprintf(&filename,  "%s/frame%d.png", input_directory, i);
        // char filename[500];
        // snprintf(filename, 30, "%s/frame%d.png", input_directory, i); // puts string into buffer

        printf("filename: %s\n",filename );
        Image *image = read_image(filename);

        Image *filtered_image = median_filter(image, 1);

        snprintf(filename, 30, "../filtered/frame%d.png", i); // puts string into buffer
        write_image(filename, filtered_image);

        // free_image(image);
        // free_image(filtered_image);

        FREE_IMAGE(image)
        FREE_IMAGE(filtered_image)
        printf("Frame %d procesado.\n", i);
    }
}

// ./median_filter ../../frame 5
int main(int argc, char *argv[])
{
    // printf("ARGC: %d\n", argc);

    // TODO: validar argumentos
    int i;
    for ( i = 0; i < argc; i++)
    {
       printf("argv[%d]: %s\n", i,argv[i]);
    }
    

    if (argc != 3)
    {
        printf("error in arguments\n");
        return 1;
    }

    const char *input_directory_arg = argv[1];
    const char *num_arg = argv[2];
    int num = atol(num_arg);

    // Image * newImage=malloc(sizeof(Image));
    // CREATE_IMAGE(newImage);
    // printf("cols:%d \n", IMAGE_N);
    // CREATE_IMAGE(newimage);
    printf("input_directory_arg: %s\n", input_directory_arg);
    process_files(input_directory_arg, num);

    // run_time = omp_get_wtime() - start_time;
    // printf("Tiempo: %f\n", run_time);

    return 0;
}

// 6 sin ifs
// real	0m27,339s
// user	0m27,041s
// sys	0m0,220s
