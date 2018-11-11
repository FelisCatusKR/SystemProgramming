/* System Programming
 * Assignment 1
 * ~ Making ls command ~
 * 생명공학과 2학년
 * 2012007729 우한샘
 * https://github.com/FelisCatusKR
 */

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

// 사용법을 틀렸을 경우 올바른 사용법을 출력하는 함수
void ErrorPrint(int errorType);
// ls의 주요 기능인 디렉터리 탐색을 시행하는 함수
void PrecDir(char *pathName, bool isOptionA, bool isOptionS);
// 파일명으로 qsort를 사용하기 위해 작성한 비교함수
int CmpByName(const void *a, const void *b);
// 파일 크기로 qsort를 사용하기 위해 작성한 비교함수
int CmpBySize(const void *a, const void *b);

int main(int argc, char *argv[]) {
  char *pathName;
  int i, count = 0;
  bool isOptionA = false, isOptionS = false;

  // 인자가 3개보다 많은 경우 올바른 사용법을 출력함
  // 인자가 3개이지만 option이 path보다 먼저 오지 않은 경우에도 출력
  if ( argc > 3 || (argc == 3 && argv[1][0] != '-') ) {
    ErrorPrint(1);
    exit(-1);
  }
  // 인자에 option을 사용한 경우, -a나 -S가 아닌 경우에도 에러 출력
  else if ( argc > 1 && argv[1][0] == '-'
            && argv[1][1] != 'a' && argv[1][1] != 'S' ) {
    ErrorPrint(2);
    exit(-1);
  }
  // 이외의 경우, 정상적인 ls 구동 절차로 진입함
  else {
    // 인자가 없거나 option만 있을 경우, "." 디렉터리를 엶
    if ( argc == 1 || (argc == 2 && argv[1][0] == '-') )
      pathName = ".";
    // option이 없이 경로만 주어진 경우 그 경로를 엶
    else if ( argc == 2  && argv[1][0] != '-' )
      pathName = argv[1];
    // option이 있는 경우 두 번째 인자가 경로이므로 이를 엶
    else
      pathName = argv[2];

    // 옵션이 들어온 것을 확인하여 이를 bool타입에 저장함
    if ( argc > 1 && argv[1][0] == '-' ) {
      if (argv[1][1] == 'a')
        isOptionA = true;
      if (argv[1][1] == 'S')
        isOptionS = true;
    }

    // 경로와 옵션 여부를 PercDir 함수로 전달하여 탐색 작업 실행
    PrecDir(pathName, isOptionA, isOptionS);
  }
  return 0;
}

void ErrorPrint(int errorType) {
  // 에러종류 1: 사용법 오류
  if ( errorType == 1 )
    puts("Error: Invalid usage!");
  // 에러종류 2: 잘못된 option 사용
  else if ( errorType == 2 )
    puts("Error: No such option!");

  puts("Usage example:");
  puts("  ./ls");
  puts("  ./ls [option]");
  puts("  ./ls [option] [path]");
  puts("  ./ls [path]");
  puts("(search \".\" directory by default)")
  puts("[option] explain:");
  puts("  -a: Print all files that include hidden files");
  puts("  -S: Sort files in order of its size");
}

void PrecDir(char *pathName, bool isOptionA, bool isOptionS) {
  int i, temp, count = 0;
  DIR *dp;
  struct dirent *dentry;
  struct stat buf;

  // 파일의 용량을 확인하기 위해 lstat 함수에 사용할 파일 경로를 저장할 문자열
  char filePath[PATH_MAX + NAME_MAX + 1];

  // 디렉터리 내의 파일들을 정렬하기 위해 파일명과 크기를 저장시킬 구조체
  struct fileInfo {
    char name[NAME_MAX + 1];
    int size;
  } *fileI;

  // opendir 함수로 지정한 경로를 열고, 실패한 경우 에러 출력
  // opendir 함수는 열기 작업이 실패한 경우, NULL 포인터를 반환함과 동시에
  // errno를 변경시키므로, 이를 이용하여 어떤 문제가 발생하였는지 출력시킴
  if ( (dp = opendir(pathName)) == NULL ) {
    printf("Error: Path open failed: ");
    // ENOENT: 해당하는 디렉터리가 존재하지 않는 경우
    if ( errno == ENOENT )
      puts("No such directory!");
    // EACCES: 해당 디렉터리를 열 권한이 없는 경우
    else if ( errno == EACCES )
      puts("Permission denied!");
    // ENOTDIR: 해당 경로명이 디렉터리가 아닌, 파일 등인 경우
    else if ( errno == ENOTDIR )
      printf("%s is not a directory!\n", pathName);
    else
      puts("Unknown issue!");
    exit(-1);
  }

  // 디렉터리 내 파일의 정보를 저장할 fileInfo 구조체를 동적 할당하기 위해
  // dp로 열린 경로 내의 디렉터리 갯수를 파악함
  while ( dentry = readdir(dp) ) {
    if ( dentry->d_ino != 0 ) {
      // -a 옵션이 없을 경우, .으로 시작하는 디렉터리는 넘어감
      if ( dentry->d_name[0] == '.' && !isOptionA ) continue;
      count++;
    }
  }

  // 경로 내의 디렉터리의 갯수만큼 fileI를 동적 할당해줌
  fileI = (struct fileInfo *)malloc(count * sizeof(struct fileInfo));

  // 경로를 다시 처음부터 읽어주기 위해 rewinddir을 사용하여 포인터 초기화
  rewinddir(dp);

  // 파일 크기를 stat 함수로 불러오기 위해, ls에서 탐색중인 경로를
  // filePath 라는 문자열에 저장
  strcpy(filePath, pathName);
  temp = strlen(pathName);
  filePath[temp] = '/';

  // 동적 할당한 구조체에 디렉터리 정보를 넣어줌
  i = 0;
  while ( dentry = readdir(dp) ) {
    if ( dentry->d_ino != 0 ) {
      if ( dentry->d_name[0] == '.' && !isOptionA ) continue;
      strcpy(fileI[i].name, dentry->d_name);
      // 파일의 전체 경로를 filePath에 저장한 후, 이를 lstat 함수로 불러옴
      // 리눅스의 ls는 symbolic link의 크기를 블럭 사이즈가 아닌
      // 경로명의 크기로 표시하므로, stat 대신 lstat을 써야함!
      strcpy(filePath + temp + 1, dentry->d_name);
      if ( lstat(filePath, &buf) != 0 )
        exit(-1);
      // st_size를 불러와 fileInfo 동적 배열의 size에 저장
      fileI[i].size = buf.st_size;
      i++;
    }
  }

  if ( closedir(dp) != 0 ) {
    puts("Error: closedir failed!");
    exit(-1);
  }

  // -S 옵션이 있을 경우, 크기 내림차순순으로 정렬
  if ( isOptionS )
    qsort(fileI, count, sizeof(struct fileInfo), CmpBySize);
  // -S 옵션이 없을 경우, 이름 오름차순으로 정렬
/*
  기본적으로, 디렉터리 내 폴더/파일들의 d_ino는 이름순으로 번호가 메겨져 있으므로
  실제로 진행할 필요는 없음!
  else
    qsort(fileI, count, sizeof(struct fileInfo), CmpByName);
*/

  // 정렬이 끝난 구조체 배열을 읽어들여 디렉터리 내의 파일/폴더를
  // 실제 ls와 같이 두 칸의 공백을 두고 출력시킴
  // 이 때, 출력 결과물이 line을 넘기는 경우는 고려하지 않았음
  for ( i = 0; i < count; i++ ) {
    if (i != 0 ) printf("  ");
    printf("%s", fileI[i].name);
  }
  putchar('\n');

  free(fileI);
}

int CmpByName(const void *a, const void *b) {
  struct fileInfo {
    char name[NAME_MAX + 1];
    int size;
  };
  struct fileInfo *pA = (struct fileInfo *)a;
  struct fileInfo *pB = (struct fileInfo *)b;
  return strcmp(pA->name, pB->name);
}

int CmpBySize(const void *a, const void *b) {
  struct fileInfo {
    char name[NAME_MAX + 1];
    int size;
  };
  struct fileInfo *pA = (struct fileInfo *)a;
  struct fileInfo *pB = (struct fileInfo *)b;
  return pB->size - pA->size;
}
