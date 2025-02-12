/*
 * École Polytechnique Montréal - GIGL
 * Hiver 2025
 * Défi - challenge1.c
 *
 * Auteurs : Victor Popovic (2288035) et Olivier Thabet (2294559)
 */

#include "challenges_part1.h"

#define OUTPUT_FILE "challenges_output.txt"

void write_directory_info(const char *dir_path, int pid, int ppid, const char *txt_files)
{
    FILE *file = fopen(OUTPUT_FILE, "a");
    fprintf(file, "Répertoire : %s\n", dir_path);
    fprintf(file, "PID : %d\n", pid);
    fprintf(file, "PPID : %d\n", ppid);
    fprintf(file, "Fichiers:\n%s\n\n", txt_files);
    fclose(file);
}

int count_text_files(const char *dir_path)
{
    DIR *dir = opendir(dir_path);

    struct dirent *entry;
    int total_count = 0;
    char txt_files[MAX_PATH_LENGTH] = "";

    while ((entry = readdir(dir)) != NULL)
    {
        // GENERE PAR CHATGPT
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        // FIN DU CODE GENERE PAR CHATGPT

        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat file_stat;
        stat(path, &file_stat);
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
            // GENERE PAR CHATGPT
            strncat(txt_files, entry->d_name, sizeof(txt_files) - strlen(txt_files) - 2);
            strncat(txt_files, "\n", sizeof(txt_files) - strlen(txt_files) - 1);
            // FIN DU CODE GENERE PAR CHATGPT
        }
    }

    closedir(dir);
    write_directory_info(dir_path, getpid(), getppid(), txt_files);
    return total_count;
}

int main(int argc, char *argv[])
{

    int total_text_files = count_text_files("root");
    printf("Nombre total de fichiers .txt : %d\n", total_text_files);
    execlp("ls", "ls", "-l", OUTPUT_FILE, NULL);

    return 0;
}
