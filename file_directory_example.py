# 파일과 디렉토리 작업 코드
import os

def main():
    # 디렉토리 생성
    dir_name = "example_dir"
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)
        print(f"디렉토리 '{dir_name}' 생성됨.")
    else:
        print(f"디렉토리 '{dir_name}' 이미 존재함.")

    # 파일 생성 및 내용 쓰기
    file_name = os.path.join(dir_name, "example_file.txt")
    with open(file_name, "w") as f:
        f.write("Hello, 우분투! 파일과 디렉토리를 다룹니다.\n")
    print(f"파일 '{file_name}' 생성 및 내용 작성 완료.")

    # 디렉토리 내 파일 리스트 출력
    files = os.listdir(dir_name)
    print(f"'{dir_name}' 디렉토리 내 파일 리스트: {files}")

    # 파일 읽기
    with open(file_name, "r") as f:
        content = f.read()
    print(f"파일 내용:\n{content}")

    # 파일 삭제
    os.remove(file_name)
    print(f"파일 '{file_name}' 삭제 완료.")

    # 디렉토리 삭제
    os.rmdir(dir_name)
    print(f"디렉토리 '{dir_name}' 삭제 완료.")

if __name__ == "__main__":
    main()
