
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Provides memory functions like malloc() and free()

// Stores information about each room in the boarding house
struct Room
{
    int room_no;       // Unique room number
    int capacity;      // Maximum number of tenants allowed in the room
    bool has_submeter; // True if the room has its own electricity submeter
    bool is_active;    // True if the room is currently active
};

// Stores information about each tenant
struct Tenant
{
    int tenant_id;    // Unique identification number of the tenant
    char name[50];    // Tenant's name, up to 49 characters plus the null terminator
    int room_no;      // Room assigned to the tenant
    int move_in_day;  // Day of the month the tenant moved in
    int move_out_day; // Day the tenant moved out; 0 means still staying
    bool is_active;   // True if the tenant's record is active
};

// Records overnight guest stays for a particular room and month
struct GuestLog
{
    int room_no;      // Room where the guest stayed
    int month;        // Month of the guest stay
    int year;         // Year of the guest stay
    int guest_nights; // Total overnight guest stays recorded for the room
};

// Stores the information needed for a monthly utility bill
struct MonthlyBill
{
    int month;                  // Month covered by the bill
    int year;                   // Year covered by the bill
    int days_in_month;          // Number of days in the billing month (28–31)
    double total_kwh;           // Total electricity consumption of the whole house
    double total_water_amount;  // Total water bill in pesos
    double fixed_charge_amount; // Shared fixed charges, such as water pump electricity
};

int main(void)
{
    struct Room *rooms; // Pointer to the dynamically allocated array of rooms
    int roomCount;      // Number of rooms to register
    int startRoom;      // Room number assigned to the first room
    int submeterChoice; // Stores the user's submeter answer (1 = Yes, 0 = No)

    // Display the program title
    printf("=== BoardMate: Room Registration ===\n\n");

    // Ask how many rooms the boarding house has
    printf("How many rooms do you have? ");

    // Validate the input and reject zero or negative room counts
    if (scanf("%d", &roomCount) != 1 || roomCount <= 0)
    {
        printf("Invalid room count!\n");
        return 1; // Stop the program because the input is invalid
    }

    // Allocate enough memory to store all room records
    rooms = malloc((size_t)roomCount * sizeof(struct Room));

    // Check whether memory allocation was successful
    if (rooms == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Ask for the number of the first room
    printf("Enter starting room number: ");

    // Validate the starting number and prevent integer overflow
    // when generating the remaining sequential room numbers
    if (scanf("%d", &startRoom) != 1 ||
        startRoom <= 0 ||
        startRoom > 2147483647 - (roomCount - 1))
    {
        printf("Invalid starting room number!\n");
        free(rooms); // Release the allocated memory before exiting
        return 1;
    }

    // Register each room using a loop
    for (int i = 0; i < roomCount; i++)
    {
        // Generate consecutive room numbers automatically
        rooms[i].room_no = startRoom + i;

        // Mark newly registered rooms as active
        rooms[i].is_active = true;

        // Ask for the maximum number of tenants allowed in the room
        printf("\nRoom %d\n", rooms[i].room_no);
        printf("Enter room capacity: ");

        // Reject invalid input or a capacity of zero or less
        if (scanf("%d", &rooms[i].capacity) != 1 ||
            rooms[i].capacity <= 0)
        {
            printf("Invalid room capacity!\n");
            free(rooms); // Release memory before exiting
            return 1;
        }

        // Ask whether the room has its own electricity submeter
        printf("Does this room have an electricity submeter? (1 = Yes, 0 = No): ");

        // Accept only 1 or 0 as a valid answer
        if (scanf("%d", &submeterChoice) != 1 ||
            (submeterChoice != 0 && submeterChoice != 1))
        {
            printf("Invalid submeter choice!\n");
            free(rooms); // Release memory before exiting
            return 1;
        }

        // Convert the numeric answer into a Boolean value
        // 1 becomes true; 0 becomes false
        rooms[i].has_submeter = (submeterChoice == 1);
    }

    // Display the heading for the registered room records
    printf("\n=== Registered Rooms ===\n");

    // Print the column headings using aligned text
    printf("%-10s %-10s %-15s %-10s\n",
           "Room No.", "Capacity", "Submeter", "Status");

    // Display the details of every registered room
    for (int i = 0; i < roomCount; i++)
    {
        printf("%-10d %-10d %-15s %-10s\n",
               rooms[i].room_no,                            // Display the room number
               rooms[i].capacity,                           // Display the room capacity
               rooms[i].has_submeter ? "Yes" : "No",        // Convert Boolean to readable text
               rooms[i].is_active ? "Active" : "Inactive"); // Display room status
    }

    // Release the dynamically allocated memory
    free(rooms);

    // Inform the user that registration has finished
    printf("\nRoom registration completed!\n");

    return 0; // End the program successfully
}
