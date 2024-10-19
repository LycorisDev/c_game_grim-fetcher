#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <png.h>
#include <string.h>

// gcc -o png_to_xpm-shadows png_to_xpm-shadows.c -lpng
// ./png_to_xpm-shadows input.png output.xpm

void abort_(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

void read_png_file(const char *file_name, int *width, int *height, png_bytep **row_pointers)
{
    FILE *fp = fopen(file_name, "rb");
    if (!fp) abort_("File %s could not be opened for reading", file_name);

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort_("png_create_read_struct failed");

    png_infop info = png_create_info_struct(png);
    if (!info) abort_("png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png))) abort_("Error during init_io");

    png_init_io(png, fp);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < (*height); y++)
        (*row_pointers)[y] = (png_byte *)malloc(png_get_rowbytes(png, info));

    png_read_image(png, *row_pointers);
    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
}

void write_xpm_file(const char *file_name, png_bytep *row_pointers, int width, int height)
{
    FILE *fp = fopen(file_name, "w");
    if (!fp) abort_("File %s could not be opened for writing", file_name);

    fprintf(fp, "/* XPM */\n");
    fprintf(fp, "static char *xpm_data[] = {\n");
    fprintf(fp, "\"%d %d %d %d\",\n", width, height, 256, 2); // header: width height num_colors chars_per_pixel

    // Write the color table
    for (int i = 0; i < 256; i++)
        fprintf(fp, "\"%02X c #%02X%02X%02X\",\n", i, i, i, i);

    // Write pixel data
    for (int y = 0; y < height; y++)
    {
        fprintf(fp, "\"");
        for (int x = 0; x < width; x++)
        {
            png_bytep px = &(row_pointers[y][x * 4]);
            unsigned char b = px[2];
            unsigned char g = px[1];
            unsigned char r = px[0];
            unsigned char a = 255 - px[3]; // invert alpha
            fprintf(fp, "%02X", a); // Use only the alpha value for now
        }
        fprintf(fp, "\",\n");
    }
    fprintf(fp, "};\n");

    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_png> <output_xpm>\n", argv[0]);
        return 1;
    }

    const char *input_png = argv[1];
    const char *output_xpm = argv[2];

    int width, height;
    png_bytep *row_pointers = NULL;

    printf("Reading PNG file...\n");
    read_png_file(input_png, &width, &height, &row_pointers);
    printf("PNG file read successfully: width=%d, height=%d\n", width, height);

    printf("Writing XPM file...\n");
    write_xpm_file(output_xpm, row_pointers, width, height);
    printf("XPM file written successfully.\n");

    // Clean up
    if (row_pointers)
    {
        for (int y = 0; y < height; y++)
            free(row_pointers[y]);
        free(row_pointers);
    }

    return 0;
}
