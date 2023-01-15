#include "./../include/processB_utilities.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/shm.h>

#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH "/sem_AOS_1"

int shmfd;
char chared_data[10];
sem_t * sem;
char * ptr;

int init(int mem_size)
{
  shmfd = shm_open(SHMOBJ_PATH, O_RDONLY, DEFFILEMODE);
  ftruncate(shmfd, mem_size);
  ptr = mmap(NULL, mem_size, PROT_READ, MAP_SHARED, shmfd, 0);
  sem_t * sem = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_init(sem, 1, 1);
}

int close_all(int mem_size)
{
    shm_unlink(SHMOBJ_PATH);
    sem_close(sem);
    sem_unlink(SEM_PATH);
    munmap(ptr, mem_size);
}

int main(int argc, char const *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

    // Infinite loop
    while (TRUE) {

        // Get input in non-blocking mode
        int cmd = getch();

        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }

        else {
            mvaddch(LINES/2, COLS/2, '0');
            refresh();
        }
    }

    endwin();
    return 0;
}
