#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
DIR *dp;
struct dirent *dir;
struct stat info;
char pathname[BUFSIZ+1]; /* for full pathname of a file or a directory */
if (stat(argv[1], &info) == -1)
   printf("\n Stat for %s failed\n", argv[1]);
else {
   printf ("\nStat for %s is OK.\n", argv[1]);
   if (info.st_mode & S_IFREG){   /* if it is a regular file */
      printf ("%s is a regular file\n", argv[1]);
      printf ("File size is %ld\n", info.st_size); 
      }
   else 
   if (info.st_mode & S_IFDIR){  /* if it is a directory */
      printf ("%s is a directory\n", argv[1]);
      printf ("Directory size is %ld\n", info.st_size);
      if ((dp = opendir(argv[1])) == NULL){   /* try to open a directory */
         printf("Cannot open directory %s\n", argv[1]);
         exit(1);
         } 
      while ((dir = readdir(dp)) != NULL){ /* read the directory  in the cycle */
         if (dir->d_ino == 0) continue; /* Removed file, skeep it */
         /* preparing the full path name for the entry in the dir */
         sprintf(pathname,"%s/%s", argv[1], dir->d_name); /* concatenation to create a pathname */
         /* Checking */ 
         if (stat(pathname, &info) == -1){
           printf("Stat for %s failed\n", dir->d_name);
           perror(dir->d_name);
           continue;
          }
         else {
          if (info.st_mode & S_IFREG) /* if regular file in this directory  */
          printf("%-20s File size %ld bytes \n", dir->d_name, info.st_size);
          } 
         }
      closedir(dp); 
      }   
   } 
return(0);
}


