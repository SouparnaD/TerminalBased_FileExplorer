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


vector<string> path;
int var=-1;


/*void printrest(string s)     //*** not in use yet***
{
    struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    d = opendir(s.c_str());
    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //dir = readdir(d);
            stat(dir->d_name , &st_buf) ;
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
    
}*/     
void scroll(vector<string> &v, int start, int end,string s)
{
    if(end<v.size())
    for(int i=start;i<=end ;i++)
    {
        cout<<"\033[0;33m"<<v[i];
        //printrest(s);
        cout<<endl;
        
    }
    else
    for(int i=start;i<v.size() ;i++)
    {
        cout<<"\033[0;33m"<<v[i];
        //printrest(s);
        cout<<endl;
    }
}
void directory(string s)
{
    vector<string> v;
    struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    if(s[0] != '0')
        d = opendir(s.c_str());
    else
        d = opendir(".");
    if (d)
    {
        
        printf("\033c");
        
        v.push_back("file names: ");
        v.push_back("-----------------------");

        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &st_buf) ;
            
            
            v.push_back(dir->d_name);
            
            
            
        }
    
        closedir(d);
    }
    else {printf("there seems some error with your path?") ; return;}
    




    int c;
    int countrow=2;
    struct winsize w;
    
    ioctl(0, TIOCGWINSZ, &w);
    int rows=w.ws_row;
    int remaining;
    
     
    
    int start=0;
    int end=rows-10;
    scroll(v,start,end,s);
    printf("\033[3;1H");


    


    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        } else
        if (c == KEY_DOWN) {
            if(countrow<end && countrow<v.size())
            {
                
                ++countrow;
                cursordownward(1);
            }
            else if(countrow == end && end<=v.size()-1)
            {
                
                start++;
                end++;
                printf("\033c");
                scroll(v,start,end,s);
                cursorupward(1);
                ++countrow;
            }
            //cout<<countrow;
            
            } else
        if (c == KEY_UP) {
            
            if(countrow>start && countrow<=end)
            {
                --countrow;
                cursorupward(1);
            }
            if(countrow == start && start>2)
            {
                printf("\033c");
                --countrow;
                start--;
                end--;
                scroll(v,start,end,s);
                printf("\033[1;1H");


            }
            //cout<<countrow;
        }
        else if(c==KEY_ENTER)
        {
            s=path[var];
            s.insert(s.size(),"/");
            s.insert(s.size(),v[countrow]);
            path.push_back(s);
            var++;
            cout<<s;
            directory(s);
         } 
        else if(c==KEY_LEFT)
        {
            
            if(path.size() != 0 && var>=0)
            {
                var--;
                directory(path[var]);
                
            }
        }
        else if(c == KEY_RIGHT)
        {
            if(var<path.size()-1)
            {
                var++;
                directory(path[var]);
            }
        }
            else {
            putchar(c);
        }
    }

    //printf("\n");
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
                c = KEY_RIGHT;
                break;
            case 'D':
                c = KEY_LEFT;
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
    string s;
    char cwd[100];
    getcwd(cwd,sizeof(cwd));
    s=cwd;
    path.push_back(s);
    var++;


    if(argc>=2){
        directory(argv[1]);
    }
    else 
        directory("0");

    return 0;
}  