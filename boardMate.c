
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
    struct Room *rooms;
    int roomCount;
    int startRoom;
    int submeterChoice;

    printf("=== BoardMate: Room Registration ===\n\n");

    printf("How many rooms do you have? ");

    if (scanf("%d", &roomCount) != 1 || roomCount <= 0)
    {
        printf("Invalid room count!\n");
        return 1;
    }

    rooms = malloc((size_t)roomCount * sizeof(struct Room));

    if (rooms == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter starting room number: ");

    if (scanf("%d", &startRoom) != 1 ||
        startRoom <= 0 ||
        startRoom > 2147483647 - (roomCount - 1))
    {
        printf("Invalid starting room number!\n");
        free(rooms);
        return 1;
    }

    for (int i = 0; i < roomCount; i++)
    {
        rooms[i].room_no = startRoom + i;
        rooms[i].is_active = true;

        printf("\nRoom %d\n", rooms[i].room_no);
        printf("Enter room capacity: ");

        if (scanf("%d", &rooms[i].capacity) != 1 ||
            rooms[i].capacity <= 0)
        {
            printf("Invalid room capacity!\n");
            free(rooms);
            return 1;
        }

        printf("Does this room have an electricity submeter? (1 = Yes, 0 = No): ");

        if (scanf("%d", &submeterChoice) != 1 ||
            (submeterChoice != 0 && submeterChoice != 1))
        {
            printf("Invalid submeter choice!\n");
            free(rooms);
            return 1;
        }

        rooms[i].has_submeter = (submeterChoice == 1);
    }

    printf("\n=== Registered Rooms ===\n");
    printf("%-10s %-10s %-15s %-10s\n",
           "Room No.", "Capacity", "Submeter", "Status");

    for (int i = 0; i < roomCount; i++)
    {
        printf("%-10d %-10d %-15s %-10s\n",
               rooms[i].room_no,
               rooms[i].capacity,
               rooms[i].has_submeter ? "Yes" : "No",
               rooms[i].is_active ? "Active" : "Inactive");
    }

    free(rooms);

    printf("\nRoom registration completed!\n");

    return 0;
}
