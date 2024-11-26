#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHM_SIZE 1024  // 공유 메모리 크기
#define SHM_NAME "/file_copy_shm"
#define SEM_PARENT "/sem_parent"
#define SEM_CHILD "/sem_child"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "사용법: %s <입력 파일> <출력 파일>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    int shm_fd;
    char *shared_mem;
    sem_t *sem_parent, *sem_child;

    // 공유 메모리 생성
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("공유 메모리 생성 실패");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("공유 메모리 크기 설정 실패");
        exit(EXIT_FAILURE);
    }

    shared_mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("공유 메모리 매핑 실패");
        exit(EXIT_FAILURE);
    }

    // 세마포어 생성
    sem_parent = sem_open(SEM_PARENT, O_CREAT, 0666, 1);  // 부모 시작 시 활성화
    sem_child = sem_open(SEM_CHILD, O_CREAT, 0666, 0);    // 자식 시작 시 비활성화

    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED) {
        perror("세마포어 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 프로세스 생성
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 자식 프로세스 (쓰기)
        FILE *output_fp = fopen(output_file, "w");
        if (output_fp == NULL) {
            perror("출력 파일 열기 실패");
            exit(EXIT_FAILURE);
        }

        while (1) {
            sem_wait(sem_child);  // 부모가 쓸 때까지 대기

            if (strncmp(shared_mem, "EOF", 3) == 0) {
                break;  // EOF 발견 시 종료
            }

            fprintf(output_fp, "%s", shared_mem);
            fflush(output_fp);

            sem_post(sem_parent);  // 부모 작업 허용
        }

        fclose(output_fp);
        printf("자식 프로세스: 파일 쓰기 완료.\n");
    } else {
        // 부모 프로세스 (읽기)
        FILE *input_fp = fopen(input_file, "r");
        if (input_fp == NULL) {
            perror("입력 파일 열기 실패");
            exit(EXIT_FAILURE);
        }

        while (1) {
            sem_wait(sem_parent);  // 자식이 읽기 완료할 때까지 대기

            if (fgets(shared_mem, SHM_SIZE, input_fp) == NULL) {
                strncpy(shared_mem, "EOF", 3);  // EOF 표시
                sem_post(sem_child);
                break;
            }

            sem_post(sem_child);  // 자식 작업 허용
        }

        fclose(input_fp);
        wait(NULL);  // 자식 종료 대기
        printf("부모 프로세스: 파일 읽기 완료.\n");
    }

    // 자원 해제
    sem_close(sem_parent);
    sem_close(sem_child);
    sem_unlink(SEM_PARENT);
    sem_unlink(SEM_CHILD);
    munmap(shared_mem, SHM_SIZE);
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}

