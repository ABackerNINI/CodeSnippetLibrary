/** File: traverse_file_tree.c
 *  Tags: c,file,file operation,dir,directory,traverse,file tree;
 *
 *  2020/06/12
 *
 *  Compile with:
 *      gcc traverse_file_tree.c -W -Wall -DDEBUG=1 -o traverse_file_tree.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

/*===========================================================================*/

/**
 * @brief Helper function for traverse files in directory 'dir' recursively.
 *
 * @return
 *      @retval 0 if no errors.
 *      @retval -1 if error.
 */
int traverse_file_tree_helper(char *dir) {
    DIR *p_dir = NULL;
    struct dirent *p_entry = NULL;
    struct stat statbuf;
    size_t len;

#ifdef DEBUG
    printf("DIR:%s\n", dir);
#endif

    if ((p_dir = opendir(dir)) == NULL) {
        perror("opendir");
        return -1;
    }

    len = strlen(dir);
    dir[len] = '/';

    while (NULL != (p_entry = readdir(p_dir))) {
        if (len + strlen(p_entry->d_name) + 1 <= FILENAME_MAX) {
            strcpy(dir + len + 1, p_entry->d_name);
        } else {
            continue;
        }

        lstat(dir, &statbuf);

        if ((statbuf.st_mode & S_IFMT) == S_IFDIR) { /* if it is dir */
            /* ignore "." and ".." */
            if (strcmp(".", p_entry->d_name) != 0 &&
                strcmp("..", p_entry->d_name) != 0) {
                traverse_file_tree_helper(dir);
            }
        } else {
#ifdef DEBUG
            printf("FILE:%s\n", dir);
#endif
        }
    }

    closedir(p_dir);

    return 0;
}

/**
 * @brief Traverse files in directory 'dir' recursively.
 */
void traverse_file_tree(const char *dir) {
    char path[FILENAME_MAX];
    strcpy(path, dir);
    traverse_file_tree_helper(path);
}

/*===========================================================================*/

void test_1() { traverse_file_tree(".."); }

/*===========================================================================*/

int main() {
    test_1();

    return 0;
}
