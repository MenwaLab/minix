#include <stdio.h>
#include <dirent.h>    
#include <sys/stat.h>  
#include <string.h> 

void print(int depth, char *name, struct stat info);
void tree(const char *path, int depth);

void
print(int depth, char *name, struct stat info)
{
    for(int i = 0; i<depth;i++)
    {
        printf("    ");
    }

    if(S_ISDIR(info.st_mode))
    {
        printf("%s/\n", name);
    }
    else
    {
        printf("%s\n", name);
    }

}


void
tree(const char *path, int depth)
{
    DIR *handle = opendir(path);

    if(handle == NULL)
    {
        return;
    }

    struct dirent *in;
    struct stat info;

    while((in = readdir(handle)) != NULL)
    {
        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, in->d_name);

        stat(new_path, &info);

        if(strcmp(in->d_name, ".") == 0 || strcmp(in->d_name, "..") == 0)
        {
            continue;
        }
        // avoids infinite recursion into its parent or itself.

        print(depth+1, in->d_name, info);

        if(S_ISDIR(info.st_mode))
        {
            tree(new_path, depth+1);
        }
    }

    closedir(handle);
}

int
main(int argc, char *argv[])
{
    const char *path = (argc > 1) ? argv[1] : ".";
    printf("%s\n", path);
    tree(path, 0);
    return 0;
}
