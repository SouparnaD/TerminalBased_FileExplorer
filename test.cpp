#include <iostream>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<vector>
#include <sys/ioctl.h>
#include<string>
#include<algorithm>
#include<time.h>
using namespace std;



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

void scroll(vector<string> &v, int start, int end)
{
    if(end<v.size())
    for(int i=start;i<=end ;i++)
    {
        cout<<"\033[0;33m"<<v[i]<<endl;
        
    }
    else
    for(int i=start;i<v.size() ;i++)
    {
        cout<<"\033[0;33m"<<v[i]<<endl;
        
    }


}

    


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
    

    vector<string> v;
        struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    if(argc >= 2) d = opendir(argv[1]); 
    else d = opendir(".") ;
    if (d)
    {
        
        printf("\033c");
        
        v.push_back("file name");

        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &st_buf) ;
            
            
            v.push_back(dir->d_name);
            
            
            
        }
    
        closedir(d);
    }
    else printf("there seems some error with your path?") ;
    




    int c;
    int countrow=1;
    struct winsize w;
    
    ioctl(0, TIOCGWINSZ, &w);
    int rows=w.ws_row;
    int remaining;
    
     
    
    int start=0;
    int end=rows-3;
    scroll(v,start,end);
    printf("\033[2;1H");
    while (1) {
        c = kbget();
        if (c == KEY_ENTER || c == KEY_ESCAPE) {
            break;
        } else
        if (c == KEY_DOWN) {
            cursordownward(1);
            countrow++;
            if(countrow>end)
            {
                printf("\033c");
                start++;
                end++;
                scroll(v,start,end);

            }
        } else
        if (c == KEY_UP) {
            
            cursorupward(1);
            countrow-=1;
            if(countrow <=start && start>0)
            {
                printf("\033c");
                //printf("\033[1;1H");
                start--;
                end--;
                countrow--;
                scroll(v,start,end);
                printf("\033[2;1H");
            }
            
        } else {
            putchar(c);
        }
    }

    printf("\n");
    return 0;
}  