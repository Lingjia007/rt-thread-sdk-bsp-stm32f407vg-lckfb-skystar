#include <rtthread.h>

#ifdef RT_USING_DFS

#include <dfs_file.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define LS_NAME_WIDTH    68
#define LS_SIZE_WIDTH    12

static void ls_ex(const char *pathname)
{
    DIR *dir = RT_NULL;
    struct dirent *entry = RT_NULL;
    struct stat file_stat;
    char *fullpath = RT_NULL;
    char *path = RT_NULL;
    char name_buf[LS_NAME_WIDTH + 1];
    rt_size_t name_len;

    if (pathname == RT_NULL)
    {
#ifdef DFS_USING_WORKDIR
        extern char working_directory[];
        path = rt_strdup(working_directory);
#else
        path = rt_strdup("/");
#endif
    }
    else
    {
        path = rt_strdup(pathname);
    }

    if (path == RT_NULL)
    {
        rt_kprintf("Out of memory!\n");
        return;
    }

    dir = opendir(path);
    if (dir == RT_NULL)
    {
        rt_kprintf("No such directory: %s\n", path);
        rt_free(path);
        return;
    }

    rt_kprintf("Directory %s:\n", path);

    while ((entry = readdir(dir)) != RT_NULL)
    {
        fullpath = dfs_normalize_path(path, entry->d_name);
        if (fullpath == RT_NULL)
        {
            rt_kprintf("Out of memory!\n");
            break;
        }

        if (stat(fullpath, &file_stat) == 0)
        {
            name_len = rt_strlen(entry->d_name);
            if (name_len > LS_NAME_WIDTH)
            {
                rt_memcpy(name_buf, entry->d_name, LS_NAME_WIDTH);
                name_buf[LS_NAME_WIDTH] = '\0';
            }
            else
            {
                rt_memcpy(name_buf, entry->d_name, name_len);
                rt_memset(name_buf + name_len, ' ', LS_NAME_WIDTH - name_len);
                name_buf[LS_NAME_WIDTH] = '\0';
            }

            rt_kprintf("%s", name_buf);
            if (S_ISDIR(file_stat.st_mode))
            {
                rt_kprintf(" %-*s\n", LS_SIZE_WIDTH, "<DIR>");
            }
            else
            {
                rt_kprintf(" %*lu\n", LS_SIZE_WIDTH, (unsigned long)file_stat.st_size);
            }
        }
        else
        {
            rt_kprintf("%-*s BAD FILE\n", LS_NAME_WIDTH, entry->d_name);
        }

        rt_free(fullpath);
        fullpath = RT_NULL;
    }

    closedir(dir);
    rt_free(path);
}

static int cmd_ll(int argc, char **argv)
{
    if (argc == 1)
    {
        ls_ex(RT_NULL);
    }
    else
    {
        ls_ex(argv[1]);
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_ll, ll, List information about the FILEs.);

#endif
