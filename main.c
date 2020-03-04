#include <stdio.h>
#include <stdalign.h>

int main() {
    printf("Hello %lu\n", alignof(int));
}
