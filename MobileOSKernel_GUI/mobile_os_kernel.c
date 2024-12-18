#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define MAX_PROCESSES 128
#define MAX_SENSORS 16
#define MAX_APP_PERMISSIONS 8
#define SECURITY_TOKEN_LENGTH 32
#define MAX_LOG_ENTRIES 100

// Advanced Power Management States
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

// Enhanced Security Permissions
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

// Enhanced Process Control Block
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

// Global log buffer for simulation output
char log_buffer[MAX_LOG_ENTRIES][256];
int log_count = 0;

// Existing global variables
static MobileOSKernel mobile_kernel;

// Additional GUI Elements for new functions
HWND hwndSensorList;
HWND hwndProcessList;
HWND hwndPowerMode;
HWND hwndAddSensorButton;
HWND hwndAddProcessButton;
HWND hwndSimulateButton;
HWND hwndTransitionButton;
HWND hwndSecurityTokenButton;
HWND hwndSensorSimulateButton;
HWND hwndSchedulerSimulateButton;
HWND hwndPowerTransitionButton;
HWND hwndMultiProcessButton;
HWND hwndMemoryAllocButton;
HWND hwndLogWindow;

// Function Prototypes
void generate_security_token();
void simulate_sensor_activity();
void simulate_scheduler();
void test_power_state_transitions();
void create_multiple_processes();
uint32_t adaptive_memory_allocation(uint32_t requested_size);
void update_log_display();

uint32_t get_system_time() {
    return (uint32_t)time(NULL); // Use Unix timestamp in seconds
}

// Forward Declarations (Add these at the top)
void update_gui_state();
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// Advanced Power Management
void update_power_management(PowerManagementState new_state) {
    mobile_kernel.current_power_mode = new_state;
    
    switch (new_state) {
        case POWER_FULL:
            // Maximum performance, all resources active
            break;
        
        case POWER_BATTERY_SAVE:
            // Reduce background processes
            // Lower sensor sampling rates
            for (int i = 0; i < MAX_SENSORS; i++) {
                if (mobile_kernel.sensors[i].is_active) {
                    mobile_kernel.sensors[i].sampling_rate /= 2;
                }
            }
            break;
        
        case POWER_ULTRA_BATTERY_SAVE:
            // Suspend non-critical processes
            // Disable most sensors
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

// Enhanced Sensor Management
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

// Advanced Process Creation with Permissions
uint32_t create_enhanced_process(
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
            mobile_kernel.processes[i].last_active_timestamp = get_system_time();
            
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
    // Reset log for this simulation
    log_count = 0;

    // In a real implementation, use cryptographically secure random generation
    for (int i = 0; i < SECURITY_TOKEN_LENGTH; i++) {
        mobile_kernel.system_token.token[i] = rand() % 256;
    }
    
    mobile_kernel.system_token.creation_time = get_system_time();
    mobile_kernel.system_token.is_valid = true;

    // Log token generation details
    char token_log[256];
    sprintf(token_log, "Security Token Generated at %u", mobile_kernel.system_token.creation_time);
    strcpy(log_buffer[log_count++], token_log);

    sprintf(token_log, "Token Validity: %s", mobile_kernel.system_token.is_valid ? "Valid" : "Invalid");
    strcpy(log_buffer[log_count++], token_log);
}

// Simulate sensor activity
void simulate_sensor_activity() {
    // Reset log for this simulation
    log_count = 0;

    for (int i = 0; i < MAX_SENSORS; i++) {
        if (mobile_kernel.sensors[i].is_active) {
            // Populate the data buffer with simulated data
            int* data = (int*)mobile_kernel.sensors[i].data_buffer;
            for (int j = 0; j < 10; j++) {  // Simulating 10 readings
                data[j] = rand() % 100;  // Random values between 0 and 99
            }

            // Log sensor simulation details
            char sensor_log[256];
            sprintf(sensor_log, "Sensor Type %d - Simulated Data: ", mobile_kernel.sensors[i].type);
            for (int j = 0; j < 10; j++) {
                char temp[10];
                sprintf(temp, "%d ", data[j]);
                strcat(sensor_log, temp);
            }
            strcpy(log_buffer[log_count++], sensor_log);
        }
    }

    update_log_display();
}

// Process Scheduler Simulation
void simulate_scheduler() {
    // Reset log for this simulation
    log_count = 0;

    strcpy(log_buffer[log_count++], "Simulating process scheduler...");

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            char process_log[256];
            sprintf(process_log, "Running Process PID: %u, Name: %s, Priority: %u",
                   mobile_kernel.processes[i].pid,
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].priority);
            strcpy(log_buffer[log_count++], process_log);

            // Simulate some work
            mobile_kernel.processes[i].last_active_timestamp = get_system_time();
        }
    }

    update_log_display();
}

// Power State Transition Test
void test_power_state_transitions() {
    // Reset log for this simulation
    log_count = 0;

    char power_log[256];
    sprintf(power_log, "Current Power Mode: %d", mobile_kernel.current_power_mode);
    strcpy(log_buffer[log_count++], power_log);
    
    // Transition to Battery Save Mode
    strcpy(log_buffer[log_count++], "Switching to POWER_BATTERY_SAVE...");
    update_power_management(POWER_BATTERY_SAVE);

    // Check updated sensor sampling rates
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (mobile_kernel.sensors[i].is_active) {
            char sensor_log[256];
            sprintf(sensor_log, "Sensor Type %d - New Sampling Rate: %u Hz",
                   mobile_kernel.sensors[i].type,
                   mobile_kernel.sensors[i].sampling_rate);
            strcpy(log_buffer[log_count++], sensor_log);
        }
    }

    // Transition to Ultra Battery Save Mode
    strcpy(log_buffer[log_count++], "Switching to POWER_ULTRA_BATTERY_SAVE...");
    update_power_management(POWER_ULTRA_BATTERY_SAVE);

    // Check suspended processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            char process_log[256];
            sprintf(process_log, "Process Name: %s, Power State: %d",
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].power_state);
            strcpy(log_buffer[log_count++], process_log);
        }
    }

    update_log_display();
}

// Create Multiple Processes
void create_multiple_processes() {
    // Reset log for this simulation
    log_count = 0;

    AppPermission perms1[] = {PERM_LOCATION, PERM_NETWORK};
    create_enhanced_process("NavigationApp", 8, perms1, 2);

    AppPermission perms2[] = {PERM_CAMERA, PERM_STORAGE};
    create_enhanced_process("CameraApp", 5, perms2, 2);

    AppPermission perms3[] = {PERM_BACKGROUND_PROCESS};
    create_enhanced_process("BackgroundTask", 2, perms3, 1);

    strcpy(log_buffer[log_count++], "Processes created:");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            char process_log[256];
            sprintf(process_log, "PID: %u, Name: %s, Priority: %u",
                   mobile_kernel.processes[i].pid,
                   mobile_kernel.processes[i].process_name,
                   mobile_kernel.processes[i].priority);
            strcpy(log_buffer[log_count++], process_log);
        }
    }

    update_log_display();
    update_gui_state();
}

// Adaptive Memory Allocation
uint32_t adaptive_memory_allocation(uint32_t requested_size) {
    // Reset log for this simulation
    log_count = 0;

    char mem_log[256];
    sprintf(mem_log, "Requested Memory Size: %u bytes", requested_size);
    strcpy(log_buffer[log_count++], mem_log);

    // Implement intelligent memory allocation
    if (mobile_kernel.available_memory >= requested_size) {
        mobile_kernel.available_memory -= requested_size;
        
        sprintf(mem_log, "Memory Allocation Successful. Remaining Memory: %u bytes", 
                mobile_kernel.available_memory);
        strcpy(log_buffer[log_count++], mem_log);
        
        return (uint32_t)malloc(requested_size);
    }
    
    // If not enough memory, attempt to free low-priority process memory
    strcpy(log_buffer[log_count++], "Insufficient Memory. Attempting to reclaim...");
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].power_state == POWER_SUSPEND) {
            // Simulated memory reclamation
            strcpy(log_buffer[log_count++], "Reclaiming memory from suspended process");
        }
    }
    
    strcpy(log_buffer[log_count++], "Memory Allocation Failed");
    
    update_log_display();
    return 0;
}

void update_gui_state() {
    char buffer[256];

    // Clear and repopulate sensor list
    SendMessage(hwndSensorList, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (mobile_kernel.sensors[i].is_active) {
            sprintf(buffer, "Sensor %d: Type %d, Rate: %u Hz", 
                    i, mobile_kernel.sensors[i].type, mobile_kernel.sensors[i].sampling_rate);
            SendMessage(hwndSensorList, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    }

    // Clear and repopulate process list
    SendMessage(hwndProcessList, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (mobile_kernel.processes[i].pid != 0) {
            sprintf(buffer, "PID %u: %s (Priority %u)", 
                    mobile_kernel.processes[i].pid, 
                    mobile_kernel.processes[i].process_name, 
                    mobile_kernel.processes[i].priority);
            SendMessage(hwndProcessList, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    }

    // Update power mode display
    sprintf(buffer, "Current Power Mode: %d", mobile_kernel.current_power_mode);
    SetWindowText(hwndPowerMode, buffer);
}

// Update log display function
void update_log_display() {
    // Clear existing log
    SendMessage(hwndLogWindow, LB_RESETCONTENT, 0, 0);
    
    // Add log entries
    for (int i = 0; i < log_count; i++) {
        SendMessage(hwndLogWindow, LB_ADDSTRING, 0, (LPARAM)log_buffer[i]);
    }
}

// Update WindowProcedure to handle new simulation buttons
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        if ((HWND)lp == hwndAddSensorButton) {
            register_sensor(SENSOR_ACCELEROMETER, 50);
            update_gui_state();
        } else if ((HWND)lp == hwndAddProcessButton) {
            AppPermission perms[] = {PERM_LOCATION};
            create_enhanced_process("TestProcess", 1, perms, 1);
            update_gui_state();
        } else if ((HWND)lp == hwndSimulateButton) {
            // Simulate kernel actions
            update_gui_state();
        } else if ((HWND)lp == hwndTransitionButton) {
            update_power_management(POWER_BATTERY_SAVE);
            update_gui_state();
        } else if ((HWND)lp == hwndSecurityTokenButton) {
            generate_security_token();
        } else if ((HWND)lp == hwndSensorSimulateButton) {
            simulate_sensor_activity();
        } else if ((HWND)lp == hwndSchedulerSimulateButton) {
            simulate_scheduler();
        } else if ((HWND)lp == hwndPowerTransitionButton) {
            test_power_state_transitions();
        } else if ((HWND)lp == hwndMultiProcessButton) {
            create_multiple_processes();
        } else if ((HWND)lp == hwndMemoryAllocButton) {
            // Simulate memory allocation of 1024 bytes
            adaptive_memory_allocation(1024);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// Modify WinMain to include new GUI elements
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char className[] = "MobileOSKernelGUI";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(className, "Mobile OS Kernel GUI", WS_OVERLAPPEDWINDOW, 
                              CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, NULL, NULL, hInstance, NULL);

    // Create GUI Elements
    hwndSensorList = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, 20, 50, 300, 200, hwnd, NULL, hInstance, NULL);
    hwndProcessList = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, 400, 50, 300, 200, hwnd, NULL, hInstance, NULL);
    hwndPowerMode = CreateWindow("STATIC", "Current Power Mode: FULL", WS_CHILD | WS_VISIBLE, 20, 10, 300, 30, hwnd, NULL, hInstance, NULL);
    hwndAddSensorButton = CreateWindow("BUTTON", "Add Sensor", WS_CHILD | WS_VISIBLE, 20, 300, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndAddProcessButton = CreateWindow("BUTTON", "Add Process", WS_CHILD | WS_VISIBLE, 200, 300, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndSimulateButton = CreateWindow("BUTTON", "Simulate", WS_CHILD | WS_VISIBLE, 400, 300, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndTransitionButton = CreateWindow("BUTTON", "Power Transition", WS_CHILD | WS_VISIBLE, 600, 300, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndSecurityTokenButton = CreateWindow("BUTTON", "Generate Token", WS_CHILD | WS_VISIBLE, 20, 350, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndSensorSimulateButton = CreateWindow("BUTTON", "Simulate Sensors", WS_CHILD | WS_VISIBLE, 200, 350, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndSchedulerSimulateButton = CreateWindow("BUTTON", "Simulate Scheduler", WS_CHILD | WS_VISIBLE, 380, 350, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndPowerTransitionButton = CreateWindow("BUTTON", "Power Transitions", WS_CHILD | WS_VISIBLE, 560, 350, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndMultiProcessButton = CreateWindow("BUTTON", "Create Processes", WS_CHILD | WS_VISIBLE, 740, 350, 150, 30, hwnd, NULL, hInstance, NULL);
    hwndMemoryAllocButton = CreateWindow("BUTTON", "Memory Alloc", WS_CHILD | WS_VISIBLE, 20, 400, 150, 30, hwnd, NULL, hInstance, NULL);
    
    // Log Window for Simulation Output
    hwndLogWindow = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOSEL,  20, 450, 960, 250, hwnd, NULL, hInstance, NULL);

    // Initialize available memory (for demonstration)
    mobile_kernel.total_memory = 1024 * 1024;  // 1MB
    mobile_kernel.available_memory = mobile_kernel.total_memory;

    // Seed random number generator
    srand((unsigned int)time(NULL));

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Initialize the kernel state
    memset(&mobile_kernel, 0, sizeof(MobileOSKernel));
    mobile_kernel.current_power_mode = POWER_FULL;

    // Optional: Add some initial setup
    register_sensor(SENSOR_ACCELEROMETER, 50);
    register_sensor(SENSOR_GPS, 10);

    AppPermission initial_perms[] = {PERM_LOCATION};
    create_enhanced_process("SystemInit", 10, initial_perms, 1);

    // Update initial GUI state
    update_gui_state();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// main function as a fallback for some compilers
int main(int argc, char *argv[]) {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}