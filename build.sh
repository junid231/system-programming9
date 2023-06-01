#!/bin/bash

# main.c 파일
MAIN_FILE=main.c
# 실행 파일 이름
OUTPUT_FILE=runcube
# 컴파일러 옵션
CFLAGS="-Wall -Wextra -std=c99"
# 라이브러리 링크 옵션
# LDFLAGS="-L./libcube -lcube  "
LDFLAGS="-L./libcube -lcube -lncursesw  "
# main.c를 빌드
gcc $CFLAGS -c $MAIN_FILE -o main.o

# main.c와 라이브러리 링크하여 실행 파일 생성
gcc -o $OUTPUT_FILE main.o $LDFLAGS -Wl,-rpath='$ORIGIN/libcube'

# 임시 오브젝트 파일 및 실행 파일 제거
rm -f main.o

# 실행 파일 실행
./$OUTPUT_FILE