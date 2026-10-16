#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
    // Pointer that will reference dynamically allocated room records
    struct Room *rooms; 
    
    // Stores the number of rooms entered by the landlord
    int roomCount;

    // stores the first room number
    int startRoom;

    // Ask the landlord how many rooms they have
    printf("How many rooms do you have? ");
    scanf("%d", &roomCount);
    
    // Reject zero or negative room counts
    if (roomCount <= 0) {
        printf("Invalid Room Count!");
        return 1;
    }
    
    // Allocate enough memory for the requested number of rooms
    rooms = malloc(roomCount * sizeof(struct Room));

    if (rooms == NULL) {
        printf("Memory allocation failed");
        return 1;
    }
    
    // Ask the landlord for the starting room number
    printf("Enter starting room number: ");
    
    if (scanf("%d", &startRoom) != 1 || startRoom <= 0) {
        printf("Invalid starting room number!\n");
        free(rooms);
    }

    // Generate room numbers automatically
    for (int i = 0; i < roomCount; i++) {
        rooms[i].room_no = startRoom + i;

        printf("Room %d registered. \n", rooms[i].room_no);
    }

        return 0;
}