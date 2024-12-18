#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Mobile OS Features
#define MAX_PROCESSES 128
#define MAX_SENSORS 16
#define MAX_APP_PERMISSIONS 8
#define SECURITY_TOKEN_LENGTH 32

// Power Management States
typedef enum {
    POWER_FULL,
    POWER_INTERACTIVE,
    POWER_BATTERY_SAVE,
    POWER_ULTRA_BATTERY_SAVE,
    POWER_SUSPEND
} PowerManagementState;

// Sensor Types for Mobile Devices
typedef enum {
    SENSOR_ACCELEROMETER,
    SENSOR_GYROSCOPE,
    SENSOR_GPS,
    SENSOR_PROXIMITY,
    SENSOR_LIGHT,
    SENSOR_TEMPERATURE,
    SENSOR_HEART_RATE
} SensorType;

// Security Permissions
typedef enum {
    PERM_LOCATION,
    PERM_CAMERA,
    PERM_MICROPHONE,
    PERM_STORAGE,
    PERM_NETWORK,
    PERM_CONTACTS,
    PERM_SENSORS,
    PERM_BACKGROUND_PROCESS
} AppPermission;

// Sensor Data Structure
typedef struct {
    SensorType type;
    bool is_active;
    void* data_buffer;
    uint16_t sampling_rate;
} SensorConfig;

// Process Control Block
typedef struct {
    uint32_t pid;
    char process_name[32];
    uint8_t priority;
    uint32_t memory_usage;
    bool permissions[MAX_APP_PERMISSIONS];
    PowerManagementState power_state;
    uint32_t last_active_timestamp;
} EnhancedProcessControlBlock;

// Security Token for App Authentication
typedef struct {
    uint8_t token[SECURITY_TOKEN_LENGTH];
    uint32_t creation_time;
    bool is_valid;
} SecurityToken;

// Mobile OS Kernel State
typedef struct {
    EnhancedProcessControlBlock processes[MAX_PROCESSES];
    SensorConfig sensors[MAX_SENSORS];
    SecurityToken system_token;
    PowerManagementState current_power_mode;
    uint32_t total_memory;
    uint32_t available_memory;
} MobileOSKernel;

// Global Kernel Instance
static MobileOSKernel mobile_kernel;

uint32_t system_time() {
    return (uint32_t)time(NULL); // Use Unix timestamp in seconds
}

// Power Management
void power_management(PowerManagementState new_state) {
    mobile_kernel.current_power_mode = new_state;
    
    switch (new_state) {
        case POWER_FULL:
            // Maximum performance, all resources active
            break;
        
        case POWER_BATTERY_SAVE:
            // Reduce background processes & lower sensor sampling rates
            for (int i = 0; i < MAX_SENSORS; i++) {
                if (mobile_kernel.sensors[i].is_active) {
                    mobile_kernel.sensors[i].sampling_rate /= 2;
                }
            }
            break;
        
        case POWER_ULTRA_BATTERY_SAVE:
            // Suspend non-critical processes & disable most sensors
            for (int i = 0; i < MAX_PROCESSES; i++) {
                if (mobile_kernel.processes[i].priority < 2) {
                    mobile_kernel.processes[i].power_state = POWER_SUSPEND;
                }
            }
            break;
        
        default:
            break;
    }
}

// Sensor Management
bool register_sensor(SensorType type, uint16_t sampling_rate) {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (!mobile_kernel.sensors[i].is_active) {
            mobile_kernel.sensors[i].type = type;
            mobile_kernel.sensors[i].is_active = true;
            mobile_kernel.sensors[i].sampling_rate = sampling_rate;
            
            // Allocate sensor data buffer
            mobile_kernel.sensors[i].data_buffer = malloc(1024);
            
            return true;
        }
    }
    return false;
}

// Process Creation with Permissions
uint32_t create_process(
    const char* process_name, 
    uint8_t priority, 
    AppPermission* required_permissions,
    uint8_t permission_count
) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid == 0) {
            // Initialize process
            mobile_kernel.processes[i].pid = i + 1;
            strncpy(mobile_kernel.processes[i].process_name, process_name, 31);
            mobile_kernel.processes[i].priority = priority;
            mobile_kernel.processes[i].last_active_timestamp = system_time();
            
            // Set process permissions
            for (int j = 0; j < permission_count; j++) {
                mobile_kernel.processes[i].permissions[required_permissions[j]] = true;
            }
            
            return mobile_kernel.processes[i].pid;
        }
    }
    return 0; // Process creation failed
}

// Security Token Generation
void generate_security_token() {
    // Genarate random values for security token 
    // for real implementation, have to use cryptographically secure random generation
    for (int i = 0; i < SECURITY_TOKEN_LENGTH; i++) {
        mobile_kernel.system_token.token[i] = rand() % 256;
    }
    
    mobile_kernel.system_token.creation_time = system_time();
    mobile_kernel.system_token.is_valid = true;
}

// Memory Management with Adaptive Allocation
uint32_t adaptive_memory_allocation(uint32_t requested_size) {
    // Implement intelligent memory allocation
    if (mobile_kernel.available_memory >= requested_size) {
        mobile_kernel.available_memory -= requested_size;
        return (uint32_t)malloc(requested_size);
    }
    
    // If not enough memory, attempt to free low-priority process memory
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].power_state == POWER_SUSPEND) {
            // Logic to reclaim memory
            uint32_t process_memory = mobile_kernel.processes[i].memory_usage;
            if (process_memory > 0) {
                // Free process memory
                free((void*)mobile_kernel.processes[i].memory_usage);
                mobile_kernel.available_memory += process_memory;
                mobile_kernel.processes[i].memory_usage = 0;

                // Check if we now have enough memory for the request
                if (mobile_kernel.available_memory >= requested_size) {
                    mobile_kernel.available_memory -= requested_size;
                    return (uint32_t)malloc(requested_size); // Allocate memory
                }
            }
        }
    }
    
    return 0; // Memory allocation failed
}

// Simulations ------------------------------------------------------------------

// Creating multiple processes
void create_multiple_processes() {
    AppPermission perms1[] = {PERM_LOCATION, PERM_NETWORK};
    create_process("NavigationApp", 8, perms1, 2);

    AppPermission perms2[] = {PERM_CAMERA, PERM_STORAGE};
    create_process("CameraApp", 5, perms2, 2);

    AppPermission perms3[] = {PERM_BACKGROUND_PROCESS};
    create_process("BackgroundTask", 2, perms3, 1);

    printf("Processes created:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            printf("PID: %u, Name: %s, Priority: %u\n",
                   mobile_kernel.processes[i].pid,
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].priority);
        }
    }
}

// Simulate sensor activity
void simulate_sensor_activity() {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (mobile_kernel.sensors[i].is_active) {
            // Populate the data buffer with randome numbers
            int* data = (int*)mobile_kernel.sensors[i].data_buffer;
            for (int j = 0; j < 10; j++) {  // Simulating 10 readings
                data[j] = rand() % 100;  // Random values between 0 and 99
            }

            // Print simulated data
            printf("Sensor Type %d - Simulated Data: ", mobile_kernel.sensors[i].type);
            for (int j = 0; j < 10; j++) {
                printf("%d ", data[j]);
            }
            printf("\n");
        }
    }
}

// Simulating process sheduler
void simulate_scheduler() {
    printf("Simulating process scheduler...\n");

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            printf("Running Process PID: %u, Name: %s, Priority: %u\n",
                   mobile_kernel.processes[i].pid,
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].priority);

            // Simulate some work
            mobile_kernel.processes[i].last_active_timestamp = system_time();
        }
    }
}

// Simulate power state transitions
void test_power_state_transitions() {
    printf("Current Power Mode: %d\n", mobile_kernel.current_power_mode);
    
    // Transition to Battery Save Mode
    printf("\nSwitching to POWER_BATTERY_SAVE...\n");
    power_management(POWER_BATTERY_SAVE);

    // Check updated sensor sampling rates
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (mobile_kernel.sensors[i].is_active) {
            printf("Sensor Type %d - New Sampling Rate: %u Hz\n",
                   mobile_kernel.sensors[i].type,
                   mobile_kernel.sensors[i].sampling_rate);
        }
    }

    // Transition to Ultra Battery Save Mode
    printf("\nSwitching to POWER_ULTRA_BATTERY_SAVE...\n");
    power_management(POWER_ULTRA_BATTERY_SAVE);

    // Check suspended processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            printf("Process Name: %s, Power State: %d\n",
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].power_state);
        }
    }
}

// Kernel Initialization
void initialize_mobile_os() {
    // Initialize kernel state
    memset(&mobile_kernel, 0, sizeof(MobileOSKernel));
    
    // Set initial power state
    mobile_kernel.current_power_mode = POWER_FULL;
    
    // Initialize total and available memory
    mobile_kernel.total_memory = 256 * 1024 * 1024;  // 256 MB
    mobile_kernel.available_memory = mobile_kernel.total_memory;
    
    // Generate initial security token
    generate_security_token();
}

// Main function -----------------------------------------------------------------------------

int main() {
    initialize_mobile_os();

    // Register sensors
    register_sensor(SENSOR_ACCELEROMETER, 50);
    register_sensor(SENSOR_LIGHT, 20);
    register_sensor(SENSOR_GPS, 1);

    // Create processes
    create_multiple_processes();

    // Simulate sensor activity
    simulate_sensor_activity();

    // Test scheduler
    simulate_scheduler();

    // Test power state transitions
    test_power_state_transitions();
    
    while (1) {
        // Continuous kernel maintenance
        // Check power states
        // Manage processes
        // Handle sensor data
    }

    return 0;
}
