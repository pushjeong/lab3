#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// SIGINT 핸들러
void handle_sigint(int sig) {
    printf("\nSIGINT 시그널(%d) 받음. 프로그램 종료 방지!\n", sig);
    printf("프로그램을 계속 실행합니다.\n");
}

// SIGUSR1 핸들러
void handle_sigusr1(int sig) {
    printf("\nSIGUSR1 시그널(%d) 받음. 사용자 정의 동작 실행!\n", sig);
}

int main() {
    // SIGINT 핸들러 등록
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("SIGINT 핸들러 등록 실패");
        return EXIT_FAILURE;
    }

    // SIGUSR1 핸들러 등록
    if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR) {
        perror("SIGUSR1 핸들러 등록 실패");
        return EXIT_FAILURE;
    }

    printf("프로그램 실행 중 (PID: %d)\n", getpid());
    printf("Ctrl+C (SIGINT) 또는 SIGUSR1 시그널을 테스트하세요.\n");

    // 무한 루프
    while (1) {
        printf("작업 중...\n");
        sleep(5);  // 5초 대기
    }

    return EXIT_SUCCESS;
}
