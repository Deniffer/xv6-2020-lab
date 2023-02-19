#include "kernel/types.h"
#include "user/user.h"

char* convert_int_to_string(int num) {
    char *str = malloc(sizeof(char) * 20);
    // int i = 0, j = 0;

    // // Convert integer to string
    // while (num != 0) {
    //     int digit = num % 10;
    //     str[i] = digit + '0';
    //     i++;
    //     num /= 10;
    // }
    // str[i] = '\0';
    // for (j = 0; j < i / 2; j++) {
    //     char temp = str[j];
    //     str[j] = str[i - j - 1];
    //     str[i - j - 1] = temp;
    // }
    str[0] = num + '0';
    str[1] = '\0';
    return str;
}

char*  concat(char* str1, char* str2) {
    int contcat_string_length = strlen(str1) + strlen(str2);
    int i=0, j=0;
    char *concat_string = malloc(sizeof(char) * (contcat_string_length + 1));
    memset(concat_string,'\0',0);
    // memmove(concat_string, str1, strlen(str1));

    while (str1[i] != '\0') {
        concat_string[i] = str1[i];
        i++;
    }

    while(str2[j] != '\0') {
        concat_string[i+j] = str2[j];
        j++;
    }

    return concat_string;
}

int main (int argc, char * argv[]) {
    if (argc != 1) {
        printf("error");
        exit(-1);
    }

    int p[2];

    if (pipe(p) < 0) {
        printf("can not alloc pipe");
        exit(-1);
    }
    // parent process
    if(fork() != 0) {
        wait(0);
        char* fromChildString[20];
        memset(fromChildString,'\0',20);
        read(p[0],fromChildString,20);
        printf("%s%d: received pong\n",fromChildString,getpid());
    } else {
        int child_pid = getpid();
        char *pingString = concat(convert_int_to_string(child_pid),": received ping\n");
        write(p[1],pingString,strlen(pingString));
    }

    exit(0);
}