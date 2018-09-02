#include<iostream>
#include<string>
#include<vector>
#include <unistd.h>
#include<sys/stat.h>
#include <fstream>
#include<dirent.h>
#include<cstring>
#include <limits.h>
//#include<ioctl>
using namespace std;

bool is_file(const char* path)  //returns true if the file exists
{
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool is_dir(const char* path)  //returns true if input is a directory
{
   struct stat statbuf;
   bool isDir = false;
   if (stat(path, &statbuf) != -1) 
   {
    if (S_ISDIR(statbuf.st_mode)) 
    {
       isDir = true;
    }
   }
   return isDir;
}
string getcurdir()
{
	char cwd[100];
	getcwd(cwd,sizeof(cwd));
    string s=cwd; return s;

}
vector<string> getcommand()
{
	string s;
    getline(cin ,s) ; 
    
    
    auto it=s.begin();
    string::iterator start;
    start=it;
    
    vector<string> v;
    while(it != s.end())
    {
      if(*it == ' ')
      {
        
        string word(start,it);
        v.push_back(word);

        start=it+1;
      }
      it++;
    }
    string word(start,s.end());
    v.push_back(word);
    return v;

}
void makedir(string name,string dest)
{
	string dirname;
    
	if(dest == ".")
		dirname = getcurdir()+"/"+name;
	else
		dirname = getcurdir()+"/"+dest+"/"+name;
	int i=mkdir(dirname.c_str(),0777);
	if(i==-1)
	{
		cout<<"error creating directory"<<endl;
	}
}

void makefile(string name, string dest)
{
	string filename;
	if(dest==".")
		filename = getcurdir()+"/"+name;
	else
		filename = getcurdir()+"/"+dest+"/"+name;
	std::ofstream file{filename};
}

char* get_dir_name(char* path)  //this function returns the last folder from an input path
{
    string str;
    str=path;
    int k=str.find_last_of("/");
    if(k==-1)  //if the input does not have any slashes
    return path;
    int size =str.size();
    string name=str.substr(k+1,(size-k));
    //printf("\nname: %s\n",name);
    //cout <<"NAME: "<< name;
    return &name[0];
}







int copy(char* src, char* dst, bool first_call)
{
    char new_dir[200];
    if(is_dir(src))  
    {
    int status;
    char* dir_name=new char[200];  //destination directory name which has to be created
    strcpy( dir_name , src );
    dir_name = get_dir_name( dir_name ); //last folder of source path is now stored in dir_name
   
    strcpy(new_dir,dst);   
    if(first_call)   //on first call of this function, it will append dir_name to new_dir
    {
        strcat(new_dir,"/");
        strcat(new_dir,dir_name);
    }
    //printf("\nnew dir:%s\n",new_dir);
    status = mkdir( new_dir , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );  //make new directory new_dir(like the one in source)
    if(status==-1)
        cout<<"Unable to create directory";
    DIR* p;
    struct dirent *d;
    p=opendir(src);        
    if(p==NULL)
    {
        perror("Cannot find directory\n");
        return 0;
    }
    while(d=readdir(p))    //keep reading files of directory until none are left
    {
        //printf("\n%s\n",d->d_name);
        //printf("%d",strlen(d->d_name));
        if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
        {
            //printf("\n%s\n",d->d_name);
            char file_inside[200];
            strcpy(file_inside,src);
            strcat(file_inside,"/");
            strcat(file_inside,d->d_name);  //file in the destination directory will have its path as directoryPATH/filename
            //printf("\n%s\n",file_inside);
            char temp_dst[200];
            strcpy(temp_dst,new_dir);
            strcat(temp_dst,"/");
            strcat(temp_dst,d->d_name);  //same with destination's new file on new directory new_dir
            //printf("%s\n",temp_dst);
            copy ( file_inside , temp_dst , false );   //recursive call to copy
        }
    }
    }

    else if(is_dir(dst))  //if source is a file and destination is a directory
    {
    char* file_name=new char[200];  //source file name
    strcpy( file_name , src );
    file_name = get_dir_name( file_name );
    strcat(dst,"/");
    strcat(dst,file_name);
    }

    if( !is_dir(src) && !is_dir(dst) )
    {
    ifstream source ( src , ios::binary );
    ofstream dest ( dst , ios::binary );

    if(!source) 
    {
        cout<<"Can't open source file(s)\n";
        return 0;
    }
    if(!dest)
    { 
        cout<< "\nCan't open destination file: "<<dst<<" src:\n"<<src;
        return 0;
    }

    dest << source.rdbuf();
    
    source.close();
    dest.close();
    }

    struct stat fst;
    stat(src,&fst);
    chown(dst,fst.st_uid,fst.st_gid);   //update owner and group to original file
    chmod(dst,fst.st_mode);   //update the permissions like that of original file
    return 1;
}


void removedirectory(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    if(is_file(dirname))
    {
        remove(dirname);
        return;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                removedirectory(path);
            }
            remove(path);
        }

    }
    rmdir(dirname);
    closedir(dir);

}

void dorename(string oldname,string newname)
{
    int result = rename(oldname.c_str(),newname.c_str());
    if(result == 0)
        cout<<"success\n";
    else
        cout<<"something went wrong\n";
}











void cmd()
{
struct winsize w;
ioctl(0, TIOCGWINSZ, &w);
int rows=w.ws_row;
int a=rows-7;

printf("\033[%d;1H",a);
printf("\033[0;36m""COMMAND MODE STARTED:\n");
printf("enter a command:\n");
while(1)
{
	vector<string> v;
	v=getcommand();
	if(v[0] == "create_dir")
	{
		makedir(v[1],v[2]);
	}
	else if(v[0]== "create_file")
	{
		makefile(v[1],v[2]);
	}
	else if(v[0] == "copy")
	{
		if(v.size()<3)
			cout<<"wrong command\n";
		int i=1;
    	
    	if(!is_file((char *)v[1].c_str()) && !is_dir((char *)v[1].c_str()))
    	{
        cout<<v[1]<<": source does not exist!\n";
        continue;
    	}
    	
		if(is_file((char *)v[2].c_str()))
    	{
        	cout<<v[2]<<" already exists. Over-write? (y/n): ";
			char c;
        	cin>>c;
        	if ( c== 'n' || c=='N' )
            	continue;
    	}
    	copy((char *)v[1].c_str(),(char *)v[2].c_str(),true);
    }
    else if(v[0] == "delete")
    {
    	if(v.size()<2)
    	{
    		printf("wrong command\n");
  			return ;
    	}
    		removedirectory(v[1].c_str());
    	

    }
    else if(v[0] == "move")
    {
        if(v.size()<3)
            cout<<"wrong command\n";
        int i=1;
        
        if(!is_file((char *)v[1].c_str()) && !is_dir((char *)v[1].c_str()))
        {
        cout<<v[1]<<": source does not exist!\n";
        continue;
        }
        
        if(is_file((char *)v[2].c_str()))
        {
            cout<<v[2]<<" already exists. Over-write? (y/n): ";
            char c;
            cin>>c;
            if ( c== 'n' || c=='N' )
                continue;
        }
        copy((char *)v[1].c_str(),(char *)v[2].c_str(),true);
        removedirectory(v[1].c_str());
    }
    else if(v[0]== "exit")
    {
        printf("\033c");
        return;
    }
    else if(v[0]== "rename")
    {
        dorename(v[1],v[2]);
    }
}



};
