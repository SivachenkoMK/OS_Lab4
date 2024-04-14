#include <stdio.h>
#include <unistd.h>

int main() {
    int value = 0;
    while(1) {
        printf("Enter a value: ");
        scanf("%d", &value);
        printf("Value set to %d\n", value);
        sleep(1);
    }
    return 0;
}
