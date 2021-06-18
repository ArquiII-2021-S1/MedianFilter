#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "handle_image.c"

#define WINDOW_SIZE 1
#define NEIGHBORHOOD_SIZE ((WINDOW_SIZE * 2 + 1) * (WINDOW_SIZE * 2 + 1))
#define PARALLEL_FILES_TO_LOAD 8


struct stat st = {0};
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
void median_filter(Image *input_image,Image *filtered_image , int window_size)
{
    // Image filtered;
    // CREATE_IMAGE(filtered)

    // TODO: definir movimiento de la memoria
    // TODO: separar por esquinas, bordes y centro
    // Iterates over the image to calculate the median values
    // #pragma omp parallel for collapse(2)
    for (int i = 1; i < IMAGE_M - 1; i++)
    {
        for (int j = 1; j < IMAGE_N - 1; j++)
        {
            // int median = get_median_value_center(image, i, j, window_size);
            int x_start = i - window_size;
            int x_end = i + window_size;
            int y_start = j - window_size;
            int y_end = j + window_size;

            int neighborhood[NEIGHBORHOOD_SIZE];
            int counter = 0;
            // TODO: eliminar el contador
            // #pragma omp parallel for
            for (int x = x_start; x <= x_end; x++)
            {
                for (int y = y_start; y <= y_end; y++)
                {
                    // Get the (x,y) pixel
                    int pixel = input_image->data[x][y];

                    // Stores it in the neighborhood
                    neighborhood[counter] = pixel;
                    counter++;
                }
            }
            // Sorts the elements of neighborhood into ascending numerical order
            bubble_sort(NEIGHBORHOOD_SIZE, neighborhood);
            // Gets median value
            int median = neighborhood[NEIGHBORHOOD_SIZE / 2];
            filtered_image->data[i][j] = median;
        }
    }
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


    // Image *input_images[PARALLEL_FILES_TO_LOAD];
    Image (*input_images)[PARALLEL_FILES_TO_LOAD] = malloc(sizeof(*input_images));
    for (int i = 0; i < PARALLEL_FILES_TO_LOAD; i++)
    {
        // printf("DATA 0 0\n",input_images[i]->data[0][0]);
        Image * imageptr;
        imageptr= &(*input_images)[i];
        RESET_IMAGE(  imageptr )
    }

    Image (*filtered_images)[PARALLEL_FILES_TO_LOAD] = malloc(sizeof(*filtered_images));
    for (int i = 0; i < PARALLEL_FILES_TO_LOAD; i++)
    {
        Image * imageptr;
        imageptr= &(*filtered_images)[i];
        RESET_IMAGE(imageptr)
    }
    
    // TODO: definir tandas de PARALLEL_FILES_TO_LOAD
    // cargar archivos
    int batches = file_amount/PARALLEL_FILES_TO_LOAD;
    for (int batches_c = 0; batches_c < batches; batches_c++)
    {
        // cargar imagenes
        //  #pragma omp parallel for
        for (int files_c = 0; files_c < PARALLEL_FILES_TO_LOAD; files_c++)
        {
            int file_numer=files_c+batches_c*PARALLEL_FILES_TO_LOAD;
            char *filename;
            asprintf(&filename, "%s/frame%d.png", input_directory, file_numer);
            printf("filename: %s\n", filename);
            Image *image = read_image( &(*filtered_images)[files_c],filename);
        }

        // procesar imagenes
        //  #pragma omp parallel for
        for (int filter_c = 0; filter_c < PARALLEL_FILES_TO_LOAD; filter_c++)
        {
            
            median_filter(&(*filtered_images)[filter_c],&(*filtered_images)[filter_c] , 1);
            printf("Frame %d procesado.\n", filter_c);
        }

        // guardar imagenes
        //  #pragma omp parallel for
        for (int file_write_c = 0; file_write_c < PARALLEL_FILES_TO_LOAD; file_write_c++)
        {
            int file_numer=file_write_c+batches_c*PARALLEL_FILES_TO_LOAD;
            char *filename;
            asprintf(&filename, "../filtered/frame%d.png", file_numer);
            write_image(filename, &(*filtered_images)[file_write_c]);
            printf("Frame %d guardado.\n", file_write_c);
        }
    }

    free(filtered_images);
    free(input_images);
}

// ./median_filter ../../frame 5
int main(int argc, char *argv[])
{
    // printf("ARGC: %d\n", argc);

    // TODO: validar argumentos
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    if (argc != 3)
    {
        printf("error in arguments\n");
        return 1;
    }

    const char *input_directory_arg = argv[1];
    const char *num_arg = argv[2];
    int num = atol(num_arg);

    printf("input_directory_arg: %s\n", input_directory_arg);

    if (num%PARALLEL_FILES_TO_LOAD==0 & num!=0)
    {
        process_files(input_directory_arg, num);
    }
    else{
        printf("number of files to process must be multiple of %d \n",PARALLEL_FILES_TO_LOAD);
    }
    

    

    // run_time = omp_get_wtime() - start_time;
    // printf("Tiempo: %f\n", run_time);

    return 0;
}

// 6 sin ifs
// real	0m27,339s
// user	0m27,041s
// sys	0m0,220s


// 40 b10 sin opm 
// real	2m39,078s
// user	2m37,570s
// sys	0m0,777s

// 40 b40 sin opm 
// real	2m46,327s
// user	2m42,713s
// sys	0m2,527s


// 40 b40 con opm 
// real	2m55,529s
// user	15m56,011s
// sys	5m37,601s

// 40 b10 con opm 

// 40 b8 con opm 
