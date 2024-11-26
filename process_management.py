import os
import subprocess
import time

def create_process():
    """
    새로운 프로세스를 생성하고 실행 결과를 확인하는 함수
    """
    print("새로운 프로세스를 생성합니다.")
    process = subprocess.Popen(["sleep", "5"])  # 'sleep 5' 명령 실행 (5초 동안 대기)
    print(f"프로세스 생성 완료! PID: {process.pid}")
    return process

def list_processes():
    """
    현재 실행 중인 프로세스를 나열하는 함수
    """
    print("현재 실행 중인 프로세스를 나열합니다.")
    subprocess.run(["ps", "aux"])

def kill_process(pid):
    """
    특정 PID를 가진 프로세스를 종료하는 함수
    """
    try:
        os.kill(pid, 9)  # SIGKILL 신호를 보내 프로세스 종료
        print(f"프로세스 {pid}가 성공적으로 종료되었습니다.")
    except OSError as e:
        print(f"프로세스를 종료할 수 없습니다. 오류: {e}")

def main():
    print("프로세스 관리 프로그램 시작!")
    while True:
        print("\n옵션:")
        print("1. 새로운 프로세스 생성")
        print("2. 실행 중인 프로세스 나열")
        print("3. 특정 프로세스 종료")
        print("4. 종료")
        
        choice = input("선택: ").strip()
        
        if choice == "1":
            process = create_process()
        elif choice == "2":
            list_processes()
        elif choice == "3":
            pid = int(input("종료할 프로세스 PID를 입력하세요: ").strip())
            kill_process(pid)
        elif choice == "4":
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 입력입니다. 다시 시도하세요.")

if __name__ == "__main__":
    main()

