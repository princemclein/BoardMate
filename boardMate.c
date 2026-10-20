
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Stores information about each room
struct Room
{
    int room_no;
    int capacity;
    bool has_submeter;
    bool is_active;
};

// Stores information about each tenant
struct Tenant
{
    int tenant_id;
    char name[50];
    int room_no;
    int move_in_day;
    int move_out_day;
    bool is_active;
};

// Records overnight guest stays
struct GuestLog
{
    int room_no;
    int month;
    int year;
    int guest_nights;
};

// Stores monthly utility bill information
struct MonthlyBill
{
    int month;
    int year;
    int days_in_month;
    double total_kwh;
    double total_water_amount;
    double fixed_charge_amount;
};

// Removes leftover characters from the input buffer
void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

/* Check if a tenant is currently staying */
bool isCurrentlyStaying(struct Tenant tenant, int currentDay)
{
    // Tenant has not moved in yet
    if (currentDay < tenant.move_in_day)
    {
        return false;
    }

    // Tenant has already moved out
    if (tenant.move_out_day != 0 &&
        currentDay > tenant.move_out_day)
    {
        return false;
    }

    // Tenant is currently staying
    return true;
}

int main(void)
{
    struct Room *rooms;
    struct Tenant *tenants;

    int roomCount;
    int tenantCount;
    int startRoom;
    int submeterChoice;
    int currentDay;

    // Get the current day for occupancy checking
    printf("Enter current day of the month (1-31): ");

    if (scanf("%d", &currentDay) != 1 ||
        currentDay < 1 || currentDay > 31)
    {
        printf("Invalid current day!\n");
        return 1;
    }
    clearInputBuffer();

    // Start room registration
    printf("=== BoardMate: Room Registration ===\n\n");
    printf("How many rooms do you have? ");

    if (scanf("%d", &roomCount) != 1 || roomCount <= 0)
    {
        printf("Invalid room count!\n");
        return 1;
    }
    clearInputBuffer();

    // Allocate memory for room records
    rooms = malloc((size_t)roomCount * sizeof(struct Room));

    if (rooms == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter starting room number: ");

    if (scanf("%d", &startRoom) != 1 ||
        startRoom <= 0 ||
        startRoom > INT_MAX - (roomCount - 1))
    {
        printf("Invalid starting room number!\n");
        free(rooms);
        return 1;
    }
    clearInputBuffer();

    // Register each room
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
        clearInputBuffer();

        printf("Does this room have an electricity submeter? (1 = Yes, 0 = No): ");

        if (scanf("%d", &submeterChoice) != 1 ||
            (submeterChoice != 0 && submeterChoice != 1))
        {
            printf("Invalid submeter choice!\n");
            free(rooms);
            return 1;
        }
        clearInputBuffer();

        rooms[i].has_submeter = (submeterChoice == 1);
    }

    // Display registered rooms
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

    // Start tenant registration
    printf("\n=== BoardMate: Tenant Registration ===\n\n");
    printf("How many tenants do you want to register? ");

    if (scanf("%d", &tenantCount) != 1 || tenantCount <= 0)
    {
        printf("Invalid tenant count!\n");
        free(rooms);
        return 1;
    }
    clearInputBuffer();

    // Allocate memory for tenant records
    tenants = malloc((size_t)tenantCount * sizeof(struct Tenant));

    if (tenants == NULL)
    {
        printf("Memory allocation failed!\n");
        free(rooms);
        return 1;
    }

    // Register each tenant
    for (int i = 0; i < tenantCount; i++)
    {
        int roomChoice;
        int roomIndex;
        int currentOccupants;

        tenants[i].tenant_id = i + 1;
        tenants[i].is_active = true;

        printf("\nTenant ID: %d\n", tenants[i].tenant_id);
        printf("Enter tenant name: ");

        // Read a name that can contain spaces
        if (fgets(tenants[i].name, sizeof(tenants[i].name), stdin) == NULL)
        {
            printf("Failed to read tenant name!\n");
            free(tenants);
            free(rooms);
            return 1;
        }

        // Remove the newline from the name
        tenants[i].name[strcspn(tenants[i].name, "\n")] = '\0';

        if (tenants[i].name[0] == '\0')
        {
            printf("Tenant name cannot be empty!\n");
            free(tenants);
            free(rooms);
            return 1;
        }

        printf("Enter move-in day (1-31): ");

        if (scanf("%d", &tenants[i].move_in_day) != 1 ||
            tenants[i].move_in_day < 1 ||
            tenants[i].move_in_day > 31)
        {
            printf("Invalid move-in day!\n");
            free(tenants);
            free(rooms);
            return 1;
        }
        clearInputBuffer();

        printf("Enter move-out day (0 = still staying): ");

        if (scanf("%d", &tenants[i].move_out_day) != 1 ||
            tenants[i].move_out_day < 0 ||
            tenants[i].move_out_day > 31 ||
            (tenants[i].move_out_day != 0 &&
             tenants[i].move_out_day < tenants[i].move_in_day))
        {
            printf("Invalid move-out day!\n");
            free(tenants);
            free(rooms);
            return 1;
        }
        clearInputBuffer();

        // Keep asking until a valid room is selected
        while (true)
        {
            printf("Enter assigned room number: ");

            if (scanf("%d", &roomChoice) != 1)
            {
                printf("Invalid room number!\n");
                free(tenants);
                free(rooms);
                return 1;
            }
            clearInputBuffer();

            // Find the selected room
            roomIndex = -1;

            for (int j = 0; j < roomCount; j++)
            {
                if (rooms[j].room_no == roomChoice)
                {
                    roomIndex = j;
                    break;
                }
            }

            if (roomIndex == -1 || !rooms[roomIndex].is_active)
            {
                printf("Room does not exist or is inactive. Try again.\n");
                continue;
            }

            // Check capacity for tenants who are currently staying
            currentOccupants = 0;

            if (isCurrentlyStaying(tenants[i], currentDay))
            {
                for (int j = 0; j < i; j++)
                {
                    if (tenants[j].room_no == roomChoice &&
                        tenants[j].is_active &&
                        isCurrentlyStaying(tenants[j], currentDay))
                    {
                        currentOccupants++;
                    }
                }

                if (currentOccupants >= rooms[roomIndex].capacity)
                {
                    printf("Room is full! Choose another room.\n");
                    continue;
                }
            }

            tenants[i].room_no = roomChoice;
            break;
        }

        printf("Tenant registered successfully!\n");
    }

    // Display all registered tenants
    printf("\n=== Registered Tenants ===\n");
    printf("%-8s %-25s %-10s %-10s %-10s %-20s\n",
           "ID", "Name", "Room", "Move In", "Move Out", "Occupancy");

    for (int i = 0; i < tenantCount; i++)
    {
        // Determine occupancy using the current day
        const char *occupancy;

        if (!tenants[i].is_active)
        {
            occupancy = "Inactive Record";
        }
        else if (isCurrentlyStaying(tenants[i], currentDay))
        {
            occupancy = "Staying";
        }
        else if (currentDay < tenants[i].move_in_day)
        {
            occupancy = "Not Moved In";
        }
        else
        {
            occupancy = "Moved Out";
        }

        printf("%-8d %-25.25s %-10d %-10d %-10d %-20s\n",
               tenants[i].tenant_id,
               tenants[i].name,
               tenants[i].room_no,
               tenants[i].move_in_day,
               tenants[i].move_out_day,
               occupancy);
    }

    // Release allocated memory
    free(tenants);
    free(rooms);

    printf("\nRoom and tenant registration completed!\n");

    return 0;
}
