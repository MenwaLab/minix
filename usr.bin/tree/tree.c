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
        fprintf(stderr, "tree: %s: No such file or directory\n", path);
        return;
    }

    struct dirent *in;
    struct stat info;

    while((in = readdir(handle)) != NULL)
    {
        // ignorar inmediatamente las entradas de navegación
        if(strcmp(in->d_name, ".") == 0 || strcmp(in->d_name, "..") == 0) 
        {
            continue;
        }

        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, in->d_name);

        //  detectar enlaces simbólicos sin seguirlos con lstat
        if (lstat(new_path, &info) != 0) {
            continue; // si falla el stat: saltar
        }

        // imrpime el archivo, directorio o link
        print(depth+1, in->d_name, info);

        // si es link simbólico NO entra (evitar ciclos)
        if (S_ISLNK(info.st_mode)) {
            continue; 
        }

        // solo si es un directorio real entra recursivamente
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
