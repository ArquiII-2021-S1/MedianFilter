// TODO: cambiar a arreglos de 2 dimensiones
// TODO: tamaño como variable de entorno

#define IMAGE_N 3840
#define IMAGE_M 2160

typedef struct Image
{
    // int data[IMAGE_N][IMAGE_M];
    int **data;
    int rows, cols;
} Image;



void free_image(Image image)
{
    free(image.data);
}

// TODO: funcion para inicializar la imagen en ceros