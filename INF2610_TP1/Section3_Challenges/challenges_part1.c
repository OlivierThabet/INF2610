/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - part1.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
 */

#include "challenges_part1.h"

int count_text_files(const char *dir_path)
{
    DIR *dir = opendir(dir_path);

    struct dirent *entry;
    int total_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        int pid = fork();
        if (pid == 0)
        {

            int child_count = count_text_files(path);
            exit(child_count);
        }
        else
        {

            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                total_count += WEXITSTATUS(status);
            }
        }
        if (strstr(entry->d_name, ".txt"))
        {
            total_count++;
        }
    }

    closedir(dir);
    return total_count;
}

int main(int argc, char *argv[])
{
    int total_text_files = count_text_files("root");
    printf("N fichiers: %d\n", total_text_files);
}
