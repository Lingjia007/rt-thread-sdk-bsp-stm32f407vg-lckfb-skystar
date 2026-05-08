#include <rtthread.h>

#ifdef RT_USING_DFS
#include <dfs.h>
#include <dfs_fs.h>
#include <dfs_romfs.h>

#define DBG_TAG "app.filesystem"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#ifdef RT_USING_DFS_ROMFS
static const struct romfs_dirent _romfs_root[] =
{
    {ROMFS_DIRENT_DIR, "sdcard", RT_NULL, 0},
    {ROMFS_DIRENT_DIR, "spi-flash", RT_NULL, 0},
};

const struct romfs_dirent romfs_root =
{
    ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root) / sizeof(_romfs_root[0])
};

int mnt_init(void)
{
    if (dfs_mount(RT_NULL, "/", "rom", 0, &(romfs_root)) == 0)
    {
        LOG_I("ROMFS root filesystem mounted.");
    }
    else
    {
        LOG_E("ROMFS root filesystem mount failed!");
    }

    return 0;
}
INIT_APP_EXPORT(mnt_init);
#endif

#endif
