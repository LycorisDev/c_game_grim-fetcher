#include "grim_fetcher.h"

static int     set_pixel_data(t_xpm *file, int *i_spr);
static t_color *get_pixel_data(t_xpm *file, int is_shadow);
static t_color get_pixel(t_png *img, size_t i, int is_shadow);
static void    free_and_reset_img_data(t_xpm *file);

int set_sprite_array(char *path)
{
    int   fd;
    int   i_spr;
    int   is_success;
    int   is_parsing_ongoing;
    t_xpm file;

    if (!create_background())
        return 0;
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return 0;
    i_spr = 1;
    is_success = 1;
    is_parsing_ongoing = 1;
    bzero(&file, sizeof(t_xpm));
    while (is_parsing_ongoing && is_success)
    {
        is_parsing_ongoing = set_img_file_obj(&file, fd);
        if (is_parsing_ongoing)
            is_success = set_pixel_data(&file, &i_spr);
        free_and_reset_img_data(&file);
    }
    close(fd);
    gnl(-1);
    return is_success;
}

static int set_pixel_data(t_xpm *file, int *i_spr)
{
    file->data = get_pixel_data(file, 0);
    if (file->path_shadow)
        file->data_shadow = get_pixel_data(file, 1);
    if (!file->data || (file->path_shadow && !file->data_shadow)
        || !create_sprites_from_file(file, i_spr))
        return 0;
    return 1;
}

static t_color *get_pixel_data(t_xpm *file, int is_shadow)
{
    size_t  i;
    size_t  data_len;
    char    *path;
    t_png   *img;
    t_color *data;

    path = file->path;
    if (is_shadow)
        path = file->path_shadow;
    img = load_image_from_file(path);
    if (!img)
        return 0;
    file->size.x = img->size.x;
    file->size.y = img->size.y;
    data = malloc(img->size.x * img->size.y * sizeof(t_color));
    if (data)
    {
        i = 0;
        data_len = img->size.x * img->size.y;
        while (i < data_len)
        {
            data[i] = get_pixel(img, i, is_shadow);
            ++i;
        }
    }
    free_image(img);
    return data;
}

static t_color get_pixel(t_png *img, size_t i, int is_shadow)
{
    t_ubyte red;
    t_ubyte green;
    t_ubyte blue;
    t_ubyte alpha;

    red = img->buf[i * 4 + 0];
    green = img->buf[i * 4 + 1];
    blue = img->buf[i * 4 + 2];
    alpha = img->buf[i * 4 + 3];
    if (is_shadow)
        alpha = 255 - red;
    return get_color_rgba(red, green, blue, alpha);
}

static void free_and_reset_img_data(t_xpm *file)
{
    int i;

    free(file->path);
    free(file->path_shadow);
    free(file->data);
    free(file->data_shadow);
    i = 0;
    while (i < file->segment_len)
    {
        free(file->seg[i].id);
        free(file->seg[i].cycle);
        ++i;
    }
    free(file->seg);
    bzero(file, sizeof(t_xpm));
    return;
}
