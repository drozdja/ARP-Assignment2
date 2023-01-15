#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/shm.h>
#include <bmpfile.h>
#include <math.h>


#define WIDTH 1600
#define HEIGHT 600
#define DEPTH 4

#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH "/sem_AOS_1"

void draw_circle_bmp(bmpfile_t *bmp, int x0, int y0)
{
    // Configuration about the circle:
    int radius = 40;
    rgb_pixel_t pixel = {255, 0, 0, 0};  // Color of the circle (BGRA)
    rgb_pixel_t center = {0, 0, 255, 0}; // Color of the center (BGRA)

    // Check if x0,y0 are inside boudaries:
    x0 = x0 < radius ? radius : x0 > WIDTH - radius ? WIDTH - radius : x0;
    y0 = y0 < radius ? radius : y0 > HEIGHT - radius ? HEIGHT - radius : y0;

    // Draw the circle
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            if (x * x + y * y < radius * radius)
            {
                if (x == 0 && y == 0)
                    bmp_set_pixel(bmp, x0, y0, center); // Color center pixel
                else
                    bmp_set_pixel(bmp, x0 + x, y0 + y, pixel);
            }
        }
    }
}

void find_center(rgb_pixel_t *matrix, int *pos)
{
    int max_len = 0, count = 0;
    // iterate over all pixels
    for (int x = 0; x < WIDTH; x++)
    {
        int len = 0;
        for (int y = 0; y < HEIGHT; y++)
        {
            // Check if the pixel is part of the circle
            if (matrix[x + y * WIDTH].blue + matrix[x + y * WIDTH].green + matrix[x + y * WIDTH].red < 765)
            {
                // check if this is the first pixel of the chord
                if (!len)
                {
                    pos[0] = x;
                    pos[1] = y;
                }
                len++;
            }
        }
        // Check if the length of the current chord is bigger or equal than the previous one
        if (len > max_len)
        {
            max_len = len;
            count = 1;
        }
        else if (len == max_len)
            count++;
    }
    // Compute the center of the circle
    pos[0] = pos[0] - max_len / count;
}

void save_bmp(bmpfile_t *bmp, rgb_pixel_t *matrix)
{
    // Iterate over all pixels
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {

            rgb_pixel_t *pixel = bmp_get_pixel(bmp, x, y); // Get pixel

            matrix[x + y * WIDTH] = *pixel; // Store the pixel in the matrix
        }
    }
}

void load_bmp(bmpfile_t *bmp, rgb_pixel_t *matrix)
{
    // Iterate over all pixels
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            bmp_set_pixel(bmp, x, y, matrix[x + y * WIDTH]); // Set pixel at x,y from matrix
        }
    }
}
