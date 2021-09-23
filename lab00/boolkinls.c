#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#pragma pack(push, 1)

char * pathcat(const char *str1, char *str2)
{    
    char *res;
    size_t strlen1 = strlen(str1);
    size_t strlen2 = strlen(str2);
    int i, j;
    res = malloc((strlen1+strlen2+1)*sizeof *res);
    strcpy(res, str1);
    for (i=strlen1, j=0; ((i<(strlen1+strlen2)) && (j<strlen2)); i++, j++)                                    
        res[i] = str2[j];
        res[strlen1+strlen2] = '\0';
    return res;
}

void strmode(mode_t mode, char * buf) 
{
    const char chars[] = "rwxrwxrwx";
    for (size_t i = 0; i < 9; i++) 
    {
        buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
    }
    buf[9] = '\0';
}

char zgftlinux(mode_t mode)
{
    char result = '-';
    
    switch (mode & S_IFMT) 
    {
        case S_IFBLK: result = 'b';  break;
        case S_IFCHR: result = 'c';  break;
        case S_IFDIR: result = 'd';  break;
        case S_IFIFO: result = 'q';  break;
        case S_IFLNK: result = 'l';  break;
        case S_IFREG: result = 'f';  break;
        case S_IFSOCK: result = 's';  break;
        default:  break;
    }
    
    /*if (mode && S_IFMT) return 'f';
    if (mode && S_IFBLK) return 'b';
    if (mode && S_IFCHR) return 'c';
    if (mode && S_IFIFO) return 'q';
    if (mode && S_IFREG) return 'r';
    if (mode && S_IFDIR) return 'd';
    if (mode && S_IFLNK) return 'l';*/
    
    return result;
}

/*int isThereMinusLOption(int argc, char **argv)
{
	int result = 0;
	char c = -1;
	while ((c = getopt(argc, argv, "-l")) != -1)
	{
		if (c == '-')
		{
			c = getopt(argc, argv, "-l");
			if (c == 'l')
			{
				//all is alright;
				return result;
			}
			else
			{
				//otherwise i need to return error code
				result = -1;
				return result;
			}
		}
		else
		{

		}
	}
}*/

int main(int argc, char **argv)
{
	//fisrtly, we realize ls for zero args count (path to executable only)
	if (argc < 2)
	{
		char *cdir = get_current_dir_name();
		//printf("%s\n", cdir);
		
		DIR *cdirlist = opendir(cdir);
		struct dirent *dentry = NULL;
		while (dentry=readdir(cdirlist))
		{
			char *fullpath = pathcat("", dentry->d_name);
			printf("%s ", fullpath);
			if (fullpath != NULL) free(fullpath);
		}
		closedir(cdirlist);
		if (cdir != NULL)
		{
			free(cdir);
		}
		printf("\n");
				
	}
	else
	{	
		if (strstr(argv[1], "-l"))
		{
			if (argc >= 3)
			{
				//seems our ls have three arguments, probably -l with path
				//any order of this args can be presented
                
				char *cdir = argv[2];
				//printf("%s\n", cdir);
		
				DIR *cdirlist = opendir(cdir);
				struct dirent *dentry = NULL;
				while (dentry=readdir(cdirlist))
				{
					char *fullpath = pathcat(cdir, dentry->d_name);
					
					//now we can try to get full info about file
					struct stat finfo;
					if (!stat(dentry->d_name, &finfo))
					{
						//firstly, we need to get file type
                        printf("%c", zgftlinux(finfo.st_mode));
                        
                        //second step is get file rights
                        char rts[9];
                        strmode(finfo.st_mode, rts);
                        printf("%s ", rts);
                        
                        //third step is getting count of links
                        printf("%ud ", finfo.st_nlink);
                        
                        //fourth step is getting name of user by id
                        struct passwd *uginfo = getpwuid(finfo.st_uid);
                        printf("%s ", uginfo->pw_name);
                        
                        //fifth step is getting group name by group id
                        struct group *gginfo = getgrgid(finfo.st_gid);
                        printf("%s ", gginfo->gr_name);
                        
                        //sixth step is getting file size
                        printf("%ld ", finfo.st_size);
                        
                        //seventh step is getting file date
                        time_t temptime = finfo.st_mtime;
                        printf("%s ", asctime(gmtime(&temptime)));
                        
                        //last step is getting file name and link target
                        if (zgftlinux(finfo.st_mode) == 'l')
                        {
                            char linktarget[PATH_MAX];
                            readlink(fullpath, linktarget, PATH_MAX);
                            printf("%s -> %s", dentry->d_name, linktarget);
                        }
                        else
                        {
                            printf("%s", dentry->d_name);
                        }
                        
						printf("\n");
					}
					
					if (fullpath != NULL) free(fullpath);
				}
				closedir(cdirlist);
				//printf("\n");
			}
			else
			{
				//here we have ls for one -l argument only
				char *cdir = get_current_dir_name();
				//printf("%s\n", cdir);
		
				DIR *cdirlist = opendir(cdir);
				struct dirent *dentry = NULL;
				while (dentry=readdir(cdirlist))
				{
					char *fullpath = pathcat(cdir, dentry->d_name);
					
					//now we can try to get full info about file
					struct stat finfo;
					if (!stat(dentry->d_name, &finfo))
					{
						//firstly, we need to get file type
                        printf("%c", zgftlinux(finfo.st_mode));
                        
                        //second step is get file rights
                        char rts[9];
                        strmode(finfo.st_mode, rts);
                        printf("%s ", rts);
                        
                        //third step is getting count of links
                        printf("%ud ", finfo.st_nlink);
                        
                        //fourth step is getting name of user by id
                        struct passwd *uginfo = getpwuid(finfo.st_uid);
                        printf("%s ", uginfo->pw_name);
                        
                        //fifth step is getting group name by group id
                        struct group *gginfo = getgrgid(finfo.st_gid);
                        printf("%s ", gginfo->gr_name);
                        
                        //sixth step is getting file size
                        printf("%ld ", finfo.st_size);
                        
                        //seventh step is getting file date
                        time_t temptime = finfo.st_mtime;
                        printf("%s ", asctime(gmtime(&temptime)));
                        
                        //last step is getting file name and link target
                        if (zgftlinux(finfo.st_mode) == 'l')
                        {
                            char linktarget[PATH_MAX];
                            readlink(fullpath, linktarget, PATH_MAX);
                            printf("%s -> %s", dentry->d_name, linktarget);
                        }
                        else
                        {
                            printf("%s", dentry->d_name);
                        }
                        
						printf("\n");
					}
					
					if (fullpath != NULL) free(fullpath);
				}
				closedir(cdirlist);
				if (cdir != NULL)
				{
					free(cdir);
				}
				//printf("\n");

			}
		}
		else
		{
			if (argc >= 3)
			{
				//seems our ls have three arguments, probably -l with path
				//any order of this args can be presented
				if (strstr(argv[2], "-l"))
				{
					//Good, third arg is -l
					//...
                    
                    char *cdir = argv[1];
                    //printf("%s\n", cdir);
		
                    DIR *cdirlist = opendir(cdir);
                    struct dirent *dentry = NULL;
                    while (dentry=readdir(cdirlist))
                    {
                        char *fullpath = pathcat(cdir, dentry->d_name);
					
                        //now we can try to get full info about file
                        struct stat finfo;
                        if (!stat(dentry->d_name, &finfo))
                        {
                            //firstly, we need to get file type
                            printf("%c", zgftlinux(finfo.st_mode));
                        
                            //second step is get file rights
                            char rts[9];
                            strmode(finfo.st_mode, rts);
                            printf("%s ", rts);
                        
                            //third step is getting count of links
                            printf("%ud ", finfo.st_nlink);
                        
                            //fourth step is getting name of user by id
                            struct passwd *uginfo = getpwuid(finfo.st_uid);
                            printf("%s ", uginfo->pw_name);
                        
                            //fifth step is getting group name by group id
                            struct group *gginfo = getgrgid(finfo.st_gid);
                            printf("%s ", gginfo->gr_name);
                        
                            //sixth step is getting file size
                            printf("%ld ", finfo.st_size);
                        
                            //seventh step is getting file date
                            time_t temptime = finfo.st_mtime;
                            printf("%s ", asctime(gmtime(&temptime)));
                        
                            //last step is getting file name and link target
                            if (zgftlinux(finfo.st_mode) == 'l')
                            {
                                char linktarget[PATH_MAX];
                                readlink(fullpath, linktarget, PATH_MAX);
                                printf("%s -> %s", dentry->d_name, linktarget);
                            }
                            else
                            {
                                printf("%s", dentry->d_name);
                            }
                        
                            printf("\n");
                        }
					
                        if (fullpath != NULL) free(fullpath);
                    }
                    closedir(cdirlist);
                    //printf("\n");
				}
				else
				{
					printf("Unknown argument!\n");
				}
			}
			else
			{
				//seems we have ls with path without any other args there

				char *cdir = argv[1];
				//printf("%s\n", cdir);
		
				DIR *cdirlist = opendir(cdir);
				struct dirent *dentry = NULL;
				while (dentry=readdir(cdirlist))
				{
					char *fullpath = pathcat("", dentry->d_name);
					printf("%s ", fullpath);
					if (fullpath != NULL) free(fullpath);
				}
				closedir(cdirlist);
				printf("\n");

			}
		}
	}

	return 0;
}
