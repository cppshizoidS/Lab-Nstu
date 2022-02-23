#include <stdio.h>
#include <stdlib.h>

#define size 4

struct Route {
    char name[20];
    int length;
    int stops;
    int cost;
};


int main() {
    struct Route routes[size];

    // filling array
    printf("Typing template: <name> <length> <stops> <cost>\n");
    for (int route = 0; route < size; route++) {
        printf("\nRoute's %d data: ", route + 1);
        scanf("%s %d %d %d",
              routes[route].name,
              &routes[route].length,
              &routes[route].stops,
              &routes[route].cost);
    }

    // print routes
    for (int route = 0; route < size; route++) {
        printf("\n\nRoute's %d data:", route + 1);
        printf("\n\tName: %s",   routes[route].name);
        printf("\n\tLength: %d", routes[route].length);
        printf("\n\tStops: %d",  routes[route].stops);
        printf("\n\tCost: %d",   routes[route].cost);
    }

    return 0;
}


void sort(struct Route* routes[size]) {
    int start = 0;
    int index_max = -1;
    int max = -1;
    while (start < size) { // each route must be checked
        max = -1;
        index_max = -1;
        for (int route = start; route < size; route++) { // don't check sorted ones
            if (routes[route]->cost > max) {
                max = routes[route]->cost;
                index_max = route;
            }
        }

        // swap
        struct Route* temp = routes[index_max];
        routes[index_max] = routes[start];
        routes[start] = temp;

        start++;
    }
}

int main() {
    struct Route* routes[size];
    for (int route = 0; route < size; route++) {
        routes[route] = malloc(sizeof(struct Route));
    }
    // filling array
    printf("Typing template: <name> <length> <stops> <cost>\n");
    for (int route = 0; route < size; route++) {
        printf("\nRoute's %d data:\n", route + 1);
        scanf("%s %d %d %d",
              routes[route]->name,
              &routes[route]->length,
              &routes[route]->stops,
              &routes[route]->cost);
    }
    sort(&routes);
    // print sorted routes
    for (int route = 0; route < size; route++) {
        printf("\n\nRoute's %d data:", route + 1);
        printf("\n\tName: %s",   routes[route]->name);
        printf("\n\tLength: %d", routes[route]->length);
        printf("\n\tStops: %d",  routes[route]->stops);
        printf("\n\tCost: %d",   routes[route]->cost);
    }
    return 0;
}
