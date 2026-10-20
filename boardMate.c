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

    // Tenant move-in date
    int move_in_day;
    int move_in_month;
    int move_in_year;

    // Tenant move-out date (year = 0 means still staying)
    int move_out_day;
    int move_out_month;
    int move_out_year;

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

// Compare two dates: returns -1, 0, or 1
int compareDates(int year1, int month1, int day1,
                 int year2, int month2, int day2)
{
    if (year1 != year2)
        return year1 < year2 ? -1 : 1;

    if (month1 != month2)
        return month1 < month2 ? -1 : 1;

    if (day1 != day2)
        return day1 < day2 ? -1 : 1;

    return 0;
}

/* Check if a tenant is currently staying */
bool isCurrentlyStaying(struct Tenant tenant,
                        int currentDay, int currentMonth, int currentYear)
{
    // Tenant has not moved in yet
    if (compareDates(currentYear, currentMonth, currentDay,
                     tenant.move_in_year, tenant.move_in_month,
                     tenant.move_in_day) < 0)
    {
        return false;
    }

    // Tenant has already moved out
    if (tenant.move_out_year != 0 &&
        compareDates(currentYear, currentMonth, currentDay,
                     tenant.move_out_year, tenant.move_out_month,
                     tenant.move_out_day) > 0)
    {
        return false;
    }

    // Tenant is currently staying
    return true;
}

// Count tenants currently staying in a room (checks the first 'count' tenants)
int countOccupants(struct Tenant tenants[], int count, int roomNo,
                   int currentDay, int currentMonth, int currentYear)
{
    int occupants = 0;

    for (int j = 0; j < count; j++)
    {
        if (tenants[j].room_no == roomNo &&
            tenants[j].is_active &&
            isCurrentlyStaying(tenants[j], currentDay,
                               currentMonth, currentYear))
        {
            occupants++;
        }
    }

    return occupants;
}

// Check if a new tenant fits in a room for their whole stay
// (checks the first 'count' tenants)
bool roomHasSpaceForStay(struct Room room, struct Tenant tenants[],
                         int count, struct Tenant newTenant)
{
    // Check on the new tenant's move-in date
    if (countOccupants(tenants, count, room.room_no,
                       newTenant.move_in_day, newTenant.move_in_month,
                       newTenant.move_in_year) >= room.capacity)
    {
        return false;
    }

    // Check on each move-in date that falls during the new tenant's stay
    for (int j = 0; j < count; j++)
    {
        if (tenants[j].room_no != room.room_no || !tenants[j].is_active)
        {
            continue;
        }

        // Skip if this tenant moves in before the new tenant does
        if (compareDates(tenants[j].move_in_year, tenants[j].move_in_month,
                         tenants[j].move_in_day, newTenant.move_in_year,
                         newTenant.move_in_month, newTenant.move_in_day) < 0)
        {
            continue;
        }

        // Skip if this tenant moves in after the new tenant has left
        if (newTenant.move_out_year != 0 &&
            compareDates(tenants[j].move_in_year, tenants[j].move_in_month,
                         tenants[j].move_in_day, newTenant.move_out_year,
                         newTenant.move_out_month, newTenant.move_out_day) > 0)
        {
            continue;
        }

        if (countOccupants(tenants, count, room.room_no,
                           tenants[j].move_in_day, tenants[j].move_in_month,
                           tenants[j].move_in_year) >= room.capacity)
        {
            return false;
        }
    }

    return true;
}

// Check if at least one active room has space for the new tenant's stay
bool hasAvailableRoom(struct Room rooms[], int roomCount,
                      struct Tenant tenants[], int tenantsSoFar,
                      struct Tenant newTenant)
{
    for (int j = 0; j < roomCount; j++)
    {
        if (rooms[j].is_active &&
            roomHasSpaceForStay(rooms[j], tenants, tenantsSoFar, newTenant))
        {
            return true;
        }
    }

    return false;
}

// Check whether a date is valid
bool isValidDate(int day, int month, int year)
{
    int daysInMonth;

    if (year < 2000 || month < 1 || month > 12 || day < 1)
        return false;

    // Set the number of days in the selected month
    switch (month)
    {
    case 2:
        // Account for leap years
        daysInMonth = (year % 400 == 0 ||
                       (year % 4 == 0 && year % 100 != 0))
                          ? 29
                          : 28;
        break;

    case 4:
    case 6:
    case 9:
    case 11:
        daysInMonth = 30;
        break;

    default:
        daysInMonth = 31;
    }

    return day <= daysInMonth;
}

// Save all rooms to rooms.txt, one room per line
void saveRooms(struct Room rooms[], int roomCount)
{
    FILE *fp = fopen("rooms.txt", "w"); // "w" = write (overwrites old file)

    if (fp == NULL)
    {
        printf("Could not open rooms.txt for writing!\n");
        return;
    }

    for (int i = 0; i < roomCount; i++)
    {
        // Format: room_no|capacity|has_submeter|is_active
        fprintf(fp, "%d|%d|%d|%d\n",
                rooms[i].room_no,
                rooms[i].capacity,
                rooms[i].has_submeter,
                rooms[i].is_active);
    }

    fclose(fp); // always close the file so the data is saved
}

// Save all tenants to tenants.txt, one tenant per line
void saveTenants(struct Tenant tenants[], int tenantCount)
{
    FILE *fp = fopen("tenants.txt", "w"); // "w" = write (overwrites old file)

    if (fp == NULL)
    {
        printf("Could not open tenants.txt for writing!\n");
        return;
    }

    for (int i = 0; i < tenantCount; i++)
    {
        // Format: tenant_id|name|room_no|in_year|in_month|in_day|out_year|out_month|out_day|is_active
        fprintf(fp, "%d|%s|%d|%d|%d|%d|%d|%d|%d|%d\n",
                tenants[i].tenant_id,
                tenants[i].name,
                tenants[i].room_no,
                tenants[i].move_in_year,
                tenants[i].move_in_month,
                tenants[i].move_in_day,
                tenants[i].move_out_year,
                tenants[i].move_out_month,
                tenants[i].move_out_day,
                tenants[i].is_active);
    }

    fclose(fp);
}

// Load rooms from rooms.txt, returns how many were loaded (0 if no file yet)
int loadRooms(struct Room **rooms)
{
    FILE *fp = fopen("rooms.txt", "r"); // "r" = read
    char line[200];
    int count = 0;

    *rooms = NULL;

    // No file yet (first run), so there is nothing to load
    if (fp == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        struct Room room;
        int hasSubmeter;
        int isActive;

        // Split the line by '|' into the fields
        if (sscanf(line, "%d|%d|%d|%d",
                   &room.room_no,
                   &room.capacity,
                   &hasSubmeter,
                   &isActive) != 4)
        {
            continue; // skip a broken line
        }

        room.has_submeter = (hasSubmeter == 1);
        room.is_active = (isActive == 1);

        // Grow the array by one room
        struct Room *temp = realloc(*rooms, (size_t)(count + 1) * sizeof(struct Room));

        if (temp == NULL)
        {
            printf("Memory allocation failed!\n");
            break;
        }

        *rooms = temp;
        (*rooms)[count] = room;
        count++;
    }

    fclose(fp);
    return count;
}

// Load tenants from tenants.txt, returns how many were loaded (0 if no file yet)
int loadTenants(struct Tenant **tenants)
{
    FILE *fp = fopen("tenants.txt", "r");
    char line[300];
    int count = 0;

    *tenants = NULL;

    if (fp == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        struct Tenant tenant;
        int isActive;

        // %49[^|] reads the name (spaces allowed) until the next '|'
        if (sscanf(line, "%d|%49[^|]|%d|%d|%d|%d|%d|%d|%d|%d",
                   &tenant.tenant_id,
                   tenant.name,
                   &tenant.room_no,
                   &tenant.move_in_year,
                   &tenant.move_in_month,
                   &tenant.move_in_day,
                   &tenant.move_out_year,
                   &tenant.move_out_month,
                   &tenant.move_out_day,
                   &isActive) != 10)
        {
            continue; // skip a broken line
        }

        tenant.is_active = (isActive == 1);

        struct Tenant *temp = realloc(*tenants, (size_t)(count + 1) * sizeof(struct Tenant));

        if (temp == NULL)
        {
            printf("Memory allocation failed!\n");
            break;
        }

        *tenants = temp;
        (*tenants)[count] = tenant;
        count++;
    }

    fclose(fp);
    return count;
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
    int currentMonth;
    int currentYear;

    // Get the current date for occupancy checking
    // Keep asking until the current year is valid
    while (true)
    {
        printf("Enter current year: ");

        if (scanf("%d", &currentYear) == 1 && currentYear >= 2000)
        {
            clearInputBuffer();
            break; // valid input, leave the loop
        }

        printf("Invalid current year! Try again.\n");
        clearInputBuffer(); // remove bad input so the loop doesn't repeat forever
    }

    // Keep asking until the current month is valid
    while (true)
    {
        printf("Enter current month (1-12): ");

        if (scanf("%d", &currentMonth) == 1 &&
            currentMonth >= 1 && currentMonth <= 12)
        {
            clearInputBuffer();
            break;
        }

        printf("Invalid current month! Try again.\n");
        clearInputBuffer();
    }

    // Keep asking until the current day is valid for that month and year
    while (true)
    {
        printf("Enter current day: ");

        if (scanf("%d", &currentDay) == 1 &&
            isValidDate(currentDay, currentMonth, currentYear))
        {
            clearInputBuffer();
            break;
        }

        printf("Invalid current day! Try again.\n");
        clearInputBuffer();
    }

    // TEMP TEST: load saved data and show the counts
    struct Room *savedRooms;
    struct Tenant *savedTenants;
    int savedRoomCount = loadRooms(&savedRooms);
    int savedTenantCount = loadTenants(&savedTenants);

    printf("Loaded %d room(s) and %d tenant(s) from files.\n",
           savedRoomCount, savedTenantCount);

    free(savedRooms);
    free(savedTenants);

    // Start room registration
    printf("\n=== BoardMate: Room Registration ===\n\n");
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

    saveRooms(rooms, roomCount);
    printf("Rooms saved to rooms.txt\n");

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

        // Get tenant move-in date
        // Keep asking until the move-in year is valid
        while (true)
        {
            printf("Enter move-in year: ");

            if (scanf("%d", &tenants[i].move_in_year) == 1 &&
                tenants[i].move_in_year >= 2000)
            {
                clearInputBuffer();
                break; // valid input, leave the loop
            }

            printf("Invalid move-in year! Try again.\n");
            clearInputBuffer(); // remove bad input so the loop doesn't repeat forever
        }

        // Keep asking until the move-in month is valid
        while (true)
        {
            printf("Enter move-in month (1-12): ");

            if (scanf("%d", &tenants[i].move_in_month) == 1 &&
                tenants[i].move_in_month >= 1 &&
                tenants[i].move_in_month <= 12)
            {
                clearInputBuffer();
                break;
            }

            printf("Invalid move-in month! Try again.\n");
            clearInputBuffer();
        }

        // Keep asking until the move-in day is valid for that month and year
        while (true)
        {
            printf("Enter move-in day: ");

            if (scanf("%d", &tenants[i].move_in_day) == 1 &&
                isValidDate(tenants[i].move_in_day,
                            tenants[i].move_in_month,
                            tenants[i].move_in_year))
            {
                clearInputBuffer();
                break;
            }

            printf("Invalid move-in day! Try again.\n");
            clearInputBuffer();
        }

        // Get tenant move-out date
        // Keep asking until the move-out date is valid and not before move-in
        while (true)
        {
            // Keep asking until the move-out year is valid (0 = still staying)
            while (true)
            {
                printf("Enter move-out year (0 = still staying): ");

                if (scanf("%d", &tenants[i].move_out_year) == 1 &&
                    (tenants[i].move_out_year == 0 ||
                     tenants[i].move_out_year >= 2000))
                {
                    clearInputBuffer();
                    break;
                }

                printf("Invalid move-out year! Try again.\n");
                clearInputBuffer();
            }

            // Still staying: clear unused move-out date fields
            if (tenants[i].move_out_year == 0)
            {
                tenants[i].move_out_month = 0;
                tenants[i].move_out_day = 0;
                break;
            }

            // Keep asking until the move-out month is valid
            while (true)
            {
                printf("Enter move-out month (1-12): ");

                if (scanf("%d", &tenants[i].move_out_month) == 1 &&
                    tenants[i].move_out_month >= 1 &&
                    tenants[i].move_out_month <= 12)
                {
                    clearInputBuffer();
                    break;
                }

                printf("Invalid move-out month! Try again.\n");
                clearInputBuffer();
            }

            // Keep asking until the move-out day is valid for that month and year
            while (true)
            {
                printf("Enter move-out day: ");

                if (scanf("%d", &tenants[i].move_out_day) == 1 &&
                    isValidDate(tenants[i].move_out_day,
                                tenants[i].move_out_month,
                                tenants[i].move_out_year))
                {
                    clearInputBuffer();
                    break;
                }

                printf("Invalid move-out day! Try again.\n");
                clearInputBuffer();
            }

            // Ensure the move-out date is not before move-in
            if (compareDates(tenants[i].move_out_year,
                             tenants[i].move_out_month,
                             tenants[i].move_out_day,
                             tenants[i].move_in_year,
                             tenants[i].move_in_month,
                             tenants[i].move_in_day) >= 0)
            {
                break; // valid move-out date
            }

            printf("Move-out date cannot be before move-in date! Try again.\n");
        }

        // Stop if no room has space for this tenant's stay
        if (!hasAvailableRoom(rooms, roomCount, tenants, i, tenants[i]))
        {
            printf("All rooms are full for this stay! Cannot register more tenants.\n");
            tenantCount = i; // keep only the tenants registered so far
            break;           // leave the tenant registration loop
        }

        // Keep asking until a valid room is selected
        while (true)
        {
            printf("Enter assigned room number: ");

            // Re-ask if the input is not a number
            if (scanf("%d", &roomChoice) != 1)
            {
                printf("Invalid room number! Try again.\n");
                clearInputBuffer(); // remove bad input so the loop doesn't repeat forever
                continue;
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

            // Check the room has space for the whole stay
            if (!roomHasSpaceForStay(rooms[roomIndex], tenants, i, tenants[i]))
            {
                printf("Room is full during this stay! Choose another room.\n");
                continue;
            }

            tenants[i].room_no = roomChoice;
            break;
        }

        printf("Tenant registered successfully!\n");
    }

    // Display all registered tenants
    printf("\n=== Registered Tenants ===\n");
    printf("%-5s %-20s %-6s %-12s %-12s %-18s\n",
           "ID", "Name", "Room", "Move In", "Move Out", "Occupancy");

    for (int i = 0; i < tenantCount; i++)
    {
        // Determine occupancy using the current date
        const char *occupancy;
        char moveIn[20];
        char moveOut[20];

        snprintf(moveIn, sizeof(moveIn), "%04d-%02d-%02d",
                 tenants[i].move_in_year,
                 tenants[i].move_in_month,
                 tenants[i].move_in_day);

        if (tenants[i].move_out_year == 0)
        {
            snprintf(moveOut, sizeof(moveOut), "Still staying");
        }
        else
        {
            snprintf(moveOut, sizeof(moveOut), "%04d-%02d-%02d",
                     tenants[i].move_out_year,
                     tenants[i].move_out_month,
                     tenants[i].move_out_day);
        }

        if (!tenants[i].is_active)
        {
            occupancy = "Inactive Record";
        }
        else if (isCurrentlyStaying(tenants[i], currentDay,
                                    currentMonth, currentYear))
        {
            occupancy = "Staying";
        }
        else if (compareDates(currentYear, currentMonth, currentDay,
                              tenants[i].move_in_year,
                              tenants[i].move_in_month,
                              tenants[i].move_in_day) < 0)
        {
            occupancy = "Not Moved In";
        }
        else
        {
            occupancy = "Moved Out";
        }

        printf("%-5d %-20.20s %-6d %-12s %-12s %-18s\n",
               tenants[i].tenant_id,
               tenants[i].name,
               tenants[i].room_no,
               moveIn,
               moveOut,
               occupancy);
    }

    saveTenants(tenants, tenantCount);
    printf("Tenants saved to tenants.txt\n");

    // Release allocated memory
    free(tenants);
    free(rooms);

    printf("\nRoom and tenant registration completed!\n");

    return 0;
}