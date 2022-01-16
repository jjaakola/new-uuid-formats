// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uuid67.h"
#include "uuid67format.h"

int main(int argc, char **args) {
    uuid_fptr uuid_func = uuid6;
    if (argc > 1 && strncmp("7", args[1], sizeof(char)) == 0) {
        uuid_func = uuid7;
    }

    UUID *uuid_data = malloc(16);
    int rc = uuid_func(uuid_data);
    if (rc == 0) {

#ifdef DEBUG
        print_octets(uuid_data);
#endif

        char *uuids = malloc(37);
        uuid_string(uuid_data, uuids);
        printf("%s\n", uuids);
        free(uuids);
    } else {
        fprintf(stderr, "Not good, return code is: %d\n", rc);
    }
    if (uuid_data != NULL)
        free(uuid_data);
}
