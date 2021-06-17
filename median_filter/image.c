// TODO: cambiar a arreglos de 2 dimensiones
// TODO: tamaño como variable de entorno




#define IMAGE_N 3840
#define IMAGE_M 2160


#define CREATE_IMAGE(image_name)  Image image_name; image_name.cols=IMAGE_N;  image_name.rows=IMAGE_M; memset(image_name.data, 0, IMAGE_N * IMAGE_M * sizeof(int));

typedef struct
{
    // int data[IMAGE_N][IMAGE_M];
    int **data;
    int rows, cols;
} Image;


// Image createImage(){
//     Image image;
//     image.cols
// }



void free_image(Image image)
{
    free(image.data);
}

// TODO: funcion para inicializar la imagen en ceros