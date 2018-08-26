#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include<sys/stat.h>
#include<dirent.h>
//#include<algorithm>
#include<time.h>

#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}

int main(int argc, char **argv)
{
    


        struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    if(argc >= 2) d = opendir(argv[1]); 
    else d = opendir(".") ;
    if (d)
    {
        printf("\033c");
        
        printf(" %-20s%23s%17s%24s\n" , "file name", "file_size",  "permission", "last_modified") ;

        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &st_buf) ;
            
            //if(S_ISDIR (st_buf.st_mode)) 
            //{
            //printf("\033[0;32m %-30s%10ld\t", dir->d_name, st_buf.st_size );
        //printf( (S_ISDIR(st_buf.st_mode)) ? "d" : "-");}
            //else 

            printf("\033[0;33m %-30s%10ld\t",dir->d_name , st_buf.st_size);
            printf( (S_ISDIR(st_buf.st_mode)) ? "d" : "-");
            printf( (st_buf.st_mode & S_IRUSR) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWUSR) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXUSR) ? "x" : "-");
            printf( (st_buf.st_mode & S_IRGRP) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWGRP) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXGRP) ? "x" : "-");
            printf( (st_buf.st_mode & S_IROTH) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWOTH) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXOTH) ? "x" : "-");
            printf("\t%30s", ctime(&st_buf.st_mtime)) ;
        }
        closedir(d);
    }
    else printf("there seems some error with your path?") ;
    printf("\033[2;1H");

    int c;

    while (1) {
        c = kbget();
        if (c == KEY_ENTER || c == KEY_ESCAPE) {
            break;
        } else
        if (c == KEY_DOWN) {
            cursordownward(1);
        } else
        if (c == KEY_UP) {
            cursorupward(1);
        } else {
            putchar(c);
        }
    }












    printf("\n");
    return 0;
}