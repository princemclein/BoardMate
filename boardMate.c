#include <stdio.h>
#include <stdbool.h>

struct Room
{
    int room_no;
    int capacity;      // max tenants in the room
    bool has_submeter; // true if the room has its own submeter
    bool is_active;    // false = room removed/closed
};
struct Tenant
{
    int tenant_id;    // unique ID per tenant
    char name[50];    // up to 49 characters
    int room_no;      // room the tenant is staying in
    int move_in_day;  // day of the month they moved in
    int move_out_day; // 0 = still staying  
    bool is_active;   // false = tenant removed
};
struct GuestLog
{
    int room_no;
    int month;
    int year;
    int guest_nights; // total overnight guest stays in this room for the month
};
struct MonthlyBill
{
    int month;
    int year;
    int days_in_month;          // 28-31
    double total_kwh;           // whole-house electricity usage
    double total_water_amount;  // water bill in pesos
    double fixed_charge_amount; // shared fixed charges (e.g. pump)
};

int main(void)
{
    struct Room r1;
    r1.room_no = 101;
    r1.capacity = 5;
    r1.has_submeter = true;
    r1.is_active = true;

    printf("Room Number: %d\n", r1.room_no);
    printf("Capacity: %d", r1.capacity);

    return 0;
}