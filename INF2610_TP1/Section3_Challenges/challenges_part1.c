/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - part1.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
 */

#include "challenges_part1.h"

#define OUTPUT_FILE "challenge1_output.txt"

void write_directory_info(const char *dir_path, int pid, int ppid, const char *txt_files)
{
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (file == NULL)
    {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }
fprintf(file, "Data 1: %s\n", dir_path);
fprintf(file, "Data 2: %d\n", pid);
fprintf(file, "Data 3: %d\n", ppid);
fprintf(file, "Files:\n%s\n\n", txt_files);

    fclose(file);
}

int count_text_files(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        return 0;
    }

    struct dirent *entry;
    int total_count = 0;
    char txt_files[MAX_PATH_LENGTH] = "";

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat file_stat;
        if (stat(path, &file_stat) == -1)
        {
            continue;
        }

        if (S_ISDIR(file_stat.st_mode))
        {
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
        }
        else if (strstr(entry->d_name, ".txt"))
        {
            total_count++;
            strncat(txt_files, entry->d_name, sizeof(txt_files) - strlen(txt_files) - 2);
            strncat(txt_files, "\n", sizeof(txt_files) - strlen(txt_files) - 1);
        }
    }

    closedir(dir);
    write_directory_info(dir_path, getpid(), getppid(), (*txt_files) ? txt_files : "{ Vide , car aucun fichier . txt }");
    return total_count;
}

int main(int argc, char *argv[])
{
    remove(OUTPUT_FILE);
    
    int total_text_files = count_text_files("root");
    printf("N fichiers: %d\n", total_text_files);
    execlp("ls", "ls", "-l",  "challenge1_output.txt", NULL);
}