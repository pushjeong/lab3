import os

def list_files_recursively(directory):
    print(f"디렉토리: {directory}")
    try:
        # 주어진 디렉토리 내 항목 나열
        items = os.listdir(directory)
    except PermissionError:
        print(f"권한 없음: {directory}")
        return

    for item in items:
        item_path = os.path.join(directory, item)
        if os.path.isdir(item_path):
            # 디렉토리일 경우 재귀적으로 탐색
            list_files_recursively(item_path)
        else:
            # 파일일 경우 출력
            print(f"  파일: {item}")

if __name__ == "__main__":
    # 탐색할 디렉토리를 지정합니다.
    target_directory = input("탐색할 디렉토리 경로를 입력하세요 (기본값: 현재 디렉토리): ").strip() or "."
    if os.path.exists(target_directory):
        list_files_recursively(target_directory)
    else:
        print("유효하지 않은 디렉토리 경로입니다.")
