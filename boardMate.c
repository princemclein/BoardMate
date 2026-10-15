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
struct GuestLog
{
    int room_no;
    int month;
    int year;
    int guest_nights;
};
struct MonthlyBill
{
    int month;
    int year;
    int days_in_month;
    double total_kwh;
    double total_water_amount;
    double fixed_charge_amount;
};

int main(void)
{
    return 0;
}