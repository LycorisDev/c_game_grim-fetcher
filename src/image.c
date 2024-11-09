#include "grim_fetcher.h"
#include "../lib/lodepng.h"

t_png* load_image_from_file(char* png_path)
{
    t_png    *img;
    t_uivec2 size;
    unsigned int err;
    
    img = malloc(sizeof(t_png));
    if (!img)
        return 0;
    err = lodepng_decode32_file(&img->buf, &size.x, &size.y, png_path);
    if (err)
    {
        fprintf(stderr, "Lodepng error n°%u for \"%s\": %s\n", 
            err, png_path, lodepng_error_text(err));
        free_image(img);
        return 0;
    }
    img->size.x = size.x;
    img->size.y = size.y;
    return img;
}

void free_image(t_png* img)
{
    if (img)
        free(img->buf);
    free(img);
    return;
}
