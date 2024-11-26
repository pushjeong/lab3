import os
import sys
import signal
import subprocess
import shlex

# 1. exit 명령어로 프로그램 종료
def exit_shell():
    sys.exit(0)  # 프로그램 종료

# 3. 인터럽트 신호 처리 (SIGINT, SIGQUIT)
def signal_handler(sig, frame):
    if sig == signal.SIGINT:
        pass  # Ctrl-C: 아무것도 하지 않고 계속 실행
    elif sig == signal.SIGQUIT:
        print("SIGQUIT: 프로그램을 종료하고 core dump가 생성됩니다.")  # 메시지 출력
        os.abort()  # core dump 생성

# 2. 백그라운드 실행
def run_in_background(command):
    pid = os.fork()
    if pid == 0:  # 자식 프로세스
        os.setpgrp()  # 새 프로세스 그룹 생성
        subprocess.run(command, shell=True)
        sys.exit(0)
    else:
        print(f"백그라운드에서 실행 중. PID: {pid}")

# 4. 파일 재지향 및 파이프 처리
def handle_redirection_and_pipe(command):
    if '>' in command:  # 출력 재지향
        command, output_file = command.split('>', 1)
        output_file = output_file.strip()
        with open(output_file, 'w') as file:
            subprocess.run(command, shell=True, stdout=file)
    elif '<' in command:  # 입력 재지향
        command, input_file = command.split('<', 1)
        input_file = input_file.strip()
        with open(input_file, 'r') as file:
            subprocess.run(command, shell=True, stdin=file)
    elif '|' in command:  # 파이프
        commands = command.split('|')
        processes = []
        prev_process = None

        # 각 명령어에 대해 프로세스 생성
        for cmd in commands:
            cmd = cmd.strip()
            command_list = shlex.split(cmd)  # 공백을 포함한 명령어 인자 처리
            if prev_process is None:  # 첫 번째 명령은 입력 없이 실행
                process = subprocess.Popen(command_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            else:  # 두 번째 명령부터는 입력을 prev_process의 출력으로 연결
                process = subprocess.Popen(command_list, stdin=prev_process.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            processes.append(process)
            prev_process = process  # 현재 프로세스를 prev_process로 설정

        # 마지막 프로세스의 출력을 출력
        output, error = processes[-1].communicate()
        sys.stdout.write(output.decode())  # 표준 출력에 결과 출력

        # 모든 프로세스 기다리기
        for process in processes:
            process.stdout.close()
            process.stderr.close()
            process.wait()

    else:
        command_list = shlex.split(command)  # 공백을 포함한 명령어 인자 처리
        subprocess.run(command_list, shell=True)

# 5. 기본 쉘 명령들 구현
def handle_builtin_commands(command):
    if command == "exit":
        exit_shell()
        return False  # 쉘을 종료하려면 False 반환
    elif command == "clear":  # clear 명령어 처리 추가
        subprocess.run("clear", shell=True)
    elif command.startswith("ls"):
        subprocess.run(command, shell=True)
    elif command.startswith("pwd"):
        subprocess.run(command, shell=True)
    elif command.startswith("cd"):
        path = command.split(" ", 1)[1].strip()
        try:
            os.chdir(path)
        except FileNotFoundError:
            print(f"{path}: 그런 디렉토리는 없습니다.")
    elif command.startswith("mkdir"):
        subprocess.run(command, shell=True)
    elif command.startswith("rmdir"):
        subprocess.run(command, shell=True)
    elif command.startswith("ln"):
        subprocess.run(command, shell=True)
    elif command.startswith("cp"):
        subprocess.run(command, shell=True)
    elif command.startswith("rm"):
        subprocess.run(command, shell=True)
    elif command.startswith("mv"):
        subprocess.run(command, shell=True)
    elif command.startswith("cat"):
        subprocess.run(command, shell=True)
    else:
        print(f"'{command}' 명령어를 찾을 수 없습니다.")
    return True  # 계속 쉘을 실행하려면 True 반환

def main():
    # 인터럽트 및 종료 신호 처리
    signal.signal(signal.SIGINT, signal_handler)  # Ctrl-C에 대한 처리
    signal.signal(signal.SIGQUIT, signal_handler)  # Ctrl-Z에 대한 처리

    while True:
        try:
            command = input("my_shell> ").strip()

            if not command:
                continue  # 빈 명령어는 무시

            # 2. 백그라운드 실행 처리
            if command.endswith("&"):
                command = command[:-1].strip()
                run_in_background(command)
            # 4. 파일 재지향 및 파이프 처리
            elif ">" in command or "<" in command or "|" in command:
                handle_redirection_and_pipe(command)
            else:
                # 5. 내장 명령어 처리 및 쉘 종료 여부 체크
                if not handle_builtin_commands(command):
                    break  # "exit" 명령어가 입력되면 쉘 종료

        except EOFError:
            exit_shell()
            break  # EOF (Ctrl-D) 입력 시 쉘 종료
        except KeyboardInterrupt:
            pass  # Ctrl-C를 눌렀을 때 아무것도 하지 않음

if __name__ == "__main__":
    main()