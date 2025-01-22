/*
 * Ecole Polytechnique Montreal - GIGLL
 * Hiver 2025
 * SystemCalls - part2.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <string.h>


void part21 ()
{
    const char *message_write = "77dbcb01f573f1c32g196c3a7d27f62e (printed using write)\n";
    const char *message_print =  "77dbcb01f573f1c32g196c3a7d27f62e (printed using printf)";

    for (const char *carac = message_print; *carac != '\0'; carac++) {
        printf("%c", *carac);
    }
    write(STDOUT_FILENO, message_write, strlen(message_write));

    printf("\n");

}

void part22 ()
{
    size_t size = 0;
    setvbuf( stdout, NULL, _IONBF, 0);
    part21();
    
}

int main (int argc, char* argv[])
{
    
    if (argc!=2)
    {   printf("Le programme a un seul paramètre : 1 ou 2\n");
        return 1;
    }
    switch (atoi(argv[1])) {
         case 1:        part21();
                        break;
         case 2:        part22();
                        break;
        default:        printf(" Le paramètre du programme est invalide\n");
                        return 1;
    }
    return 0;
}
