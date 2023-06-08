#!/bin/bash

# main.c 파일
#./build.sh 파일이름
# 파일이름.c를 파일이름으로 컴파일함
FILE_NAME="main"
if [ $# -eq 1 ]
then
    FILE_NAME=$1
fi
MAIN_FILE=$FILE_NAME.c
echo $MAIN_FILE
# 실행 파일 이름
OUTPUT_FILE=$FILE_NAME
# 컴파일러 옵션
CFLAGS="-Wall -Wextra -std=gnu99"
# 라이브러리 링크 옵션
LDFLAGS="-L./libcube -lcube -lrt"

# main.c를 빌드
gcc $CFLAGS -c $MAIN_FILE -o $FILE_NAME.o

# main.c와 라이브러리 링크하여 실행 파일 생성
gcc -o $OUTPUT_FILE $FILE_NAME.o $LDFLAGS -Wl,-rpath='$ORIGIN/libcube' -lresolv -D_GNU_SOURCE

# 임시 오브젝트 파일 및 실행 파일 제거
rm -f $FILE_NAME.o

# 실행 파일 실행
# ./$OUTPUT_FILE
