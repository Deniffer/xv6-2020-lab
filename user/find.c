#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"


int isEqual(char * str1, char *str2) {
    if (strlen(str1) != strlen(str2)) {
        return 0;
    }
    for (int i=0; i<strlen(str1); i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}

void find(char *path, char *fileName) {
    // while loop read file inside path

    // if file is file
        // check is the same as fileName
            // printf path/fileName
        // find subPath 

    int fd;
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "Error find: cannot open %s\n", path);
        exit(-1);
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "Error find: cannot stat %s\n", path);
        close(fd);
        exit(-1);
    }

    // if file isn't directory
    if (st.type != T_DIR) {{
        printf("Invaild path!");
        close(fd);
        exit(-1); 
    }}

    // read dir
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        // if (de.inum == 0) {
        //     continue;
        // }
        // int isEqualDot = !strcmp(de.name, ".");
        // int isEqualDoubleDot = !strcmp(de.name, "..");
        if (isEqual(de.name, ".") || isEqual(de.name, "..")) {
            continue;
        }
        memset(buf, '\0', 512);
        strcpy(buf, path);
        // 你们看得懂吗? 我看得懂hh
        p = buf+strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        stat(buf,&st);
        switch(st.type) {
            case T_DIR:
                if (de.inum == 0) {
                    break;
                }
                find(buf,fileName);
                break;
            case T_FILE:
                if(!strcmp(de.name, fileName)) {
                    printf("%s\n",buf);
                }
                break;
        } 
    }
}

int main (int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error! Usage: find directoryName fileName");
        exit(-1);
    }
    // get directory Name & fileName
    char *path = argv[1];
    char *fileName = argv[2];

    find(path, fileName);
    exit(0);
}