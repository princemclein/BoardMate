#include <stdio.h>
#include <stdbool.h>

struct Room
{
    int room_no;
    int capacity;
    bool has_submeter;
    bool is_active;
};
struct Tenant
{
    int tenant_id;
    char name[50];
    int room_no;
    int move_in_day;
    int move_out_day;
    bool is_active;
};
int main(void)
{
    return 0;
}