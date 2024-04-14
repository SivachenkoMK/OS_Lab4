#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

int main(void) {
    FILE *fp;
    char path[1035];
    char kbd_path[20] = "/dev/input/event";
    char mouse_path[20] = "/dev/input/mouse";
    char event_num[3];
    char mouse_num[3];

    fp = popen("cat /proc/bus/input/devices", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("%s", path);
        if (strstr(path, "keyboard")) {
            fgets(path, sizeof(path), fp);
            sscanf(path, "H: Handlers=sysrq kbd event%s", event_num);
            strcat(kbd_path, event_num);
        }
        if (strstr(path, "mouse")) {
            fgets(path, sizeof(path), fp);
            sscanf(path, "H: Handlers=mouse%s event", mouse_num);
            strcat(mouse_path, mouse_num);
        }
    }

    pclose(fp);

    struct input_event ev;
    int fd_kbd = open(kbd_path, O_RDONLY);
    int fd_mouse = open(mouse_path, O_RDONLY);
    if (fd_kbd == -1 || fd_mouse == -1) {
        fprintf(stderr, "Cannot open keyboard or mouse input device\n");
        return EXIT_FAILURE;
    }

    while (1) {
        read(fd_kbd, &ev, sizeof(struct input_event));
        if(ev.type == 1)
            printf("Keyboard event: key %i state %i\n", ev.code, ev.value);

        char buffer[3] = {0};
        read(fd_mouse, buffer, sizeof(buffer));
        char left = buffer[0] & 0x1;
        char right = buffer[0] & 0x2;
        char x = buffer[1];
        char y = buffer[2];
        printf("Mouse event: left click %d, right click %d, x displacement %d, y displacement %d\n", left, right, x, y);
    }

    return 0;
}
