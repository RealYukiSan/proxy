#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <utils/string.h>

char** strsplit(const char* s, const char* delim, size_t* nb) {
    void* data;
    char* _s = ( char* )s;
    const char** ptrs;
    size_t ptrsSize, nbWords = 1,sLen = strlen( s ), delimLen = strlen( delim );

    while ( ( _s = strstr( _s, delim ) ) ) {
      _s += delimLen;
      ++nbWords;
    }
    ptrsSize = ( nbWords + 1 ) * sizeof( char* );
    ptrs =
    data = malloc( ptrsSize + sLen + 1 );
    if ( data ) {
      *ptrs =
      _s = strcpy( ( ( char* )data ) + ptrsSize, s );
      if ( nbWords > 1 ) {
        while ( ( _s = strstr( _s, delim ) ) ) {
          *_s = '\0';
          _s += delimLen;
          *++ptrs = _s;
        }
      }
      *++ptrs = NULL;
    }
    if ( nb ) *nb = data ? nbWords : 0;
    return data;
}

int strcopy(char **c, char *n) {
    size_t len = strlen(n) + 1;
    *c = malloc(len);
    if (!c) {
        return 0;
    }
    if (snprintf(*c, len, "%s", n))
        return 1;
    else 0;
}

int findstr(char* str, char* toFind) {
    for (int a = 0, b = 0; a < strlen(str); a++) {
        if (str[a] == toFind[b]) b++;
        else b = 0;
        if (b == strlen(toFind)) return a + 1;
    }
    return 0;
}

int findarray(char** array, char* val) {
    int result = 0;
    char isFound = 0;
    doSearch:
    while(array[result]) {
        for (int a = 0; a < strlen(val); a++) {
            if (array[result][a] != val[a]) {
                result++;
                goto doSearch;
            }
        }
        isFound = 1;
        break;
    }
    if (isFound) return result;
    else return -1;
}

char* generateHex(int len) {
    char* result = malloc(len ? (len * 2) + 1 : 18);
    char* hexVal = "0123456789ABCDEF";
    if (!len) {
        for (int a = 0, b = 0; a < 17; a++) {
            result[a] = tolower(hexVal[rand() % 16]), b++;
            if (b == 3) result[a] = ':', b = 0;
        }
        result[17] = '\0';
    } else {
        for (int a = 0; a < len * 2; a++) result[a] = hexVal[rand() % 16];
        result[len * 2] = '\0';
    }

    return result;
}


char* arrayjoin(char** array, char* joinVal, char autoRemove) {
    int a = 0, totalPos = 0, currentPos = 0;

    while(array[a]) {
        if (array[a][0] || (!array[a][0] && !autoRemove)) {
            totalPos += strlen(array[a++]) + strlen(joinVal);
        } else if (!array[a][0] && autoRemove) a++;
    }

    char* result = malloc(totalPos + 1);
    a = 0;

    while(array[a]) {
        if (array[a][0] || (!array[a][0] && !autoRemove)) {
            sprintf(result + currentPos, "%s%s", array[a], joinVal);
            currentPos += strlen(array[a++]) + strlen(joinVal);
        } else if (!array[a][0] && autoRemove) a++;
    }

    result[currentPos] = '\0';
    //result[pos - valLen] = '\0';

    return result;
}

char* CatchMessage(const char *message, ...) {
    static char* result;
    result = malloc(1024);
    va_list ap;
    va_start(ap, message);
    vsnprintf(result, 1024, message, ap);
    va_end(ap);
    return result;
}
