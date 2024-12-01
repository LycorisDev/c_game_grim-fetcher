#include "grim_fetcher.h"

static int  populate_array(void);
static char *strjoin(const char *s1, const char *s2);
static void sort_strings(char **arr, int (*cmp)(const char *, const char *));

void set_map_filenames(void)
{
    DIR           *dir;
    struct dirent *entry;
    size_t        count;

    dir = opendir("maps");
    if (!dir)
        return;
    count = 0;
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] != '.')
            ++count;
    }
    closedir(dir);
    if (!count)
        return;
    man.map_filenames = calloc(count + 1, sizeof(char *));
    if (!populate_array())
        free_map_filenames();
    return;
}

void free_map_filenames(void)
{
    size_t i;

    if (!man.map_filenames)
        return;
    i = 0;
    while (man.map_filenames[i])
        free(man.map_filenames[i++]);
    free(man.map_filenames);
    man.map_filenames = 0;
    return;
}

static int populate_array(void)
{
    DIR           *dir;
    struct dirent *entry;
    size_t        i;

    if (!man.map_filenames)
        return 0;
    dir = opendir("maps");
    if (!dir)
        return 0;
    i = 0;
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] != '.')
        {
            man.map_filenames[i] = strjoin("maps/", entry->d_name);
            if (!man.map_filenames[i])
                return 0;
            ++i;
        }
    }
    closedir(dir);
    sort_strings(man.map_filenames, strcmp);
    return 1;
}

static char *strjoin(const char *s1, const char *s2)
{
    char   *joined;
    size_t len1;
    size_t len2;

    if (!s1 || !s2)
        return 0;
    len1 = strlen(s1);
    len2 = strlen(s2);
    joined = calloc(len1 + len2 + 1, sizeof(char));
    if (!joined)
        return 0;
    strcpy(joined, s1);
    strcpy(joined + len1, s2);
    return joined;
}

static void sort_strings(char **arr, int (*cmp)(const char *, const char *))
{
    size_t i;
    char   *tmp;
    int    swap_performed;

    if (!arr)
        return;
    i = 0;
    swap_performed = 0;
    while (arr[i])
    {
        if (arr[i + 1] && cmp(arr[i], arr[i + 1]) > 0)
        {
            tmp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = tmp;
            swap_performed = 1;
        }
        ++i;
        if (swap_performed && !arr[i + 1])
        {
            swap_performed = 0;
            i = 0;
        }
    }
    return;
}
