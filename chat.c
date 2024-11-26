#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>

#define MSG_SIZE 100

// 메시지 구조체
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

// 메시지 큐 ID 전역 변수
int msgid;

// 메시지 읽기 스레드 함수
void *receive_messages(void *arg) {
    struct msg_buffer message;
    long msg_type = *(long *)arg;

    while (1) {
        // 메시지 읽기
        if (msgrcv(msgid, &message, sizeof(message.msg_text), msg_type, 0) == -1) {
            perror("메시지 수신 실패");
            exit(EXIT_FAILURE);
        }

        // 'exit' 메시지 처리
        if (strncmp(message.msg_text, "exit", 4) == 0) {
            printf("\n상대방이 채팅을 종료했습니다. 프로그램을 종료합니다.\n");
            exit(EXIT_SUCCESS);
        }

        // 받은 메시지 출력
        printf("\n상대방: %s", message.msg_text);
    }

    return NULL;
}

int main() {
    key_t key;
    struct msg_buffer message;
    long my_type, other_type;
    pthread_t receiver_thread;

    // 키 생성
    key = ftok("chat_key", 65);

    // 메시지 큐 생성
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("메시지 큐 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 사용자 역할 선택
    printf("채팅 시작! 역할을 선택하세요:\n");
    printf("1. 사용자 1 (메시지 유형 1 보내기, 유형 2 받기)\n");
    printf("2. 사용자 2 (메시지 유형 2 보내기, 유형 1 받기)\n");
    printf("선택: ");
    int choice;
    scanf("%d", &choice);
    getchar();  // 버퍼 비우기

    if (choice == 1) {
        my_type = 1;
        other_type = 2;
    } else if (choice == 2) {
        my_type = 2;
        other_type = 1;
    } else {
        printf("잘못된 선택입니다. 종료합니다.\n");
        exit(EXIT_FAILURE);
    }

    // 메시지 수신 스레드 시작
    pthread_create(&receiver_thread, NULL, receive_messages, &other_type);

    // 메시지 전송 루프
    printf("\n채팅을 시작합니다! 'exit'을 입력하면 종료됩니다.\n");
    while (1) {
        message.msg_type = my_type;

        printf("나: ");
        fgets(message.msg_text, MSG_SIZE, stdin);

        // 메시지 보내기
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("메시지 전송 실패");
        }

        // 'exit' 메시지 처리
        if (strncmp(message.msg_text, "exit", 4) == 0) {
            printf("채팅을 종료합니다.\n");
            break;
        }
    }

    // 메시지 큐 삭제 (사용자 1만 삭제)
    if (my_type == 1) {
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("메시지 큐 삭제 실패");
        }
    }

    return EXIT_SUCCESS;
}
