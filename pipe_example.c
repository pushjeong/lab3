#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe_fd[2];  // 파이프를 위한 파일 디스크립터 배열
    pid_t pid;
    char write_msg[] = "안녕하세요, 자식 프로세스!";
    char read_msg[100];

    // 파이프 생성
    if (pipe(pipe_fd) == -1) {
        perror("파이프 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 자식 프로세스 생성
    pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 자식 프로세스
        close(pipe_fd[1]);  // 파이프의 쓰기 끝 닫기
        read(pipe_fd[0], read_msg, sizeof(read_msg));  // 파이프에서 읽기
        printf("자식 프로세스에서 받은 메시지: %s\n", read_msg);
        close(pipe_fd[0]);  // 파이프의 읽기 끝 닫기
        exit(EXIT_SUCCESS);
    } else {
        // 부모 프로세스
        close(pipe_fd[0]);  // 파이프의 읽기 끝 닫기
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1);  // 파이프에 쓰기
        printf("부모 프로세스에서 메시지 보냄: %s\n", write_msg);
        close(pipe_fd[1]);  // 파이프의 쓰기 끝 닫기
        wait(NULL);  // 자식 프로세스 종료 대기
    }

    return EXIT_SUCCESS;
}
