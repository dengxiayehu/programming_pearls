#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum { WB = 0, BB = 1, BC };

int foo(int balls[BC])
{
    int c[BC];
    int i;

    while (balls[WB] + balls[BB] > 1) { // loop until more than one ball left
        for (i = 0; i < BC; i++) {
            c[i] = rand()%BC;
            if (balls[c[i]] == 0) { // this color is empty, choose the other
                c[i] = !c[i];
            }
            balls[c[i]]--; // take it out
        }

        if (c[0] == c[1]) { // same color, put a black ball back
            balls[BB]++;
        } else {
            balls[WB]++; // not the same color, "recover" the white ball's number
        }
    }

    assert(balls[WB] >= 0 && balls[BB] >= 0 && balls[WB] + balls[BB] == 1);
    
    printf("WB:%d BB:%d\n", balls[WB], balls[BB]);

    return 0;
}

int main(int argc, char const* argv[])
{
    int balls[BC];

    srand((unsigned int)time(NULL));

    while (scanf("%d %d", &balls[WB], &balls[BB]) != EOF) {
        assert(balls[WB] >= 0 && balls[BB] >= 0 && balls[WB] + balls[BB] >= 1);
        assert(!foo(balls));
    }

    return 0;
}
