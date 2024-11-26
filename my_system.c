#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100  // 최대 인수 개수

int my_system(const char *cmd) {
    if (cmd == NULL) {
        return -1;  // NULL 명령어 처리
    }

    char *args[MAX_ARGS];
    char command[1024];
    strcpy(command, cmd);

    // 명령어 파싱
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // execvp를 위한 NULL 종료

    // 자식 프로세스 생성
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // 자식 프로세스에서 명령 실행
        execvp(args[0], args);
        perror("execvp");  // execvp 실패 시 에러 출력
        exit(EXIT_FAILURE);
    } else {
        // 부모 프로세스에서 자식 종료 대기
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <쉘 명령>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 명령어를 인수 배열로부터 문자열로 변환
    char command[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) {
            strcat(command, " ");
        }
    }

    printf("명령어 실행: %s\n", command);
    int result = my_system(command);
    printf("명령어 종료, 반환 값: %d\n", result);

    return EXIT_SUCCESS;
}
