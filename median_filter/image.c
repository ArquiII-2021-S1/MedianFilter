typedef struct Image
{
    int **data;
    int rows, cols;
} Image;

void free_image(Image image)
{
    free(image.data);
}