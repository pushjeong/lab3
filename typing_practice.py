import time
import difflib

def calculate_errors(original, typed):
    """
    원본 문장(original)과 입력 문장(typed)의 차이를 기반으로 오타를 계산합니다.
    문자열 양 끝의 불필요한 공백을 제거하고 비교합니다.
    """
    original = original.strip()
    typed = typed.strip()
    seq = difflib.SequenceMatcher(None, original, typed)
    differences = sum(1 for tag, _, _, _, _ in seq.get_opcodes() if tag != 'equal')
    return differences

def typing_practice():
    sentences = [
        "타자 연습은 재미있습니다.",
        "우분투에서 Python 프로그래밍을 배웁니다.",
        "오타를 줄이고 정확히 입력하세요.",
        "분당 타자 속도를 측정합니다."
    ]
    
    print("타자 연습 프로그램에 오신 것을 환영합니다!")
    print("아래 문장을 입력하세요.\n")

    total_errors = 0
    total_time = 0
    total_words = 0

    for sentence in sentences:
        print(f"\n문장: {sentence}")
        start_time = time.time()
        user_input = input("입력: ")
        end_time = time.time()

        # 시간 계산
        elapsed_time = end_time - start_time
        total_time += elapsed_time

        # 오타 계산
        errors = calculate_errors(sentence, user_input)
        total_errors += errors

        # 단어 수 계산 (공백 기준)
        total_words += len(sentence.split())

        print(f"입력 시간: {elapsed_time:.2f}초, 오타: {errors}개")

    # 평균 분당 타자수 계산
    wpm = (total_words / total_time) * 60
    print("\n결과 요약:")
    print(f"총 오타 횟수: {total_errors}")
    print(f"평균 분당 타자수: {wpm:.2f} WPM")

if __name__ == "__main__":
    typing_practice()

