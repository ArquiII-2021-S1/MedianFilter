// TODO: cambiar a arreglos de 2 dimensiones
// TODO: tamaño como variable de entorno
typedef struct Image
{
    int **data;
    int rows, cols;
} Image;

void free_image(Image image)
{
    free(image.data);
}

// TODO: funcion para inicializar la imagen en ceros