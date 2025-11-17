/*
 * PROJECT 2: Hospital Patient Record Management System
 * 
 * End Semester Project
 * Section: ND
 * Team: Agriim Rastogi, Akshat Gaur, Amrit Mathur, Hemant Tyagi, Dhruv Singh
 * 
 * Features Implemented:
 * - Structures
 * - Dynamic Memory Management
 * - File Handling
 * - Authentication
 * - Searching/Sorting
 * - String Handling
 * 
 * FlowChart: https://lucid.app/lucidchart/544529bb-770d-46ec-8313-6059b0461af5/edit?viewport_loc=-1502%2C-997%2C2587%2C1250%2C0_0&invitationId=inv_69ed15e9-9221-4eea-8d34-1d78aad465f6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/* ==================== CONSTANTS ==================== */
#define MAX_NAME_LENGTH 50
#define MAX_MEDICAL_NOTES_LENGTH 500
#define MAX_SPECIALISATION_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define DEFAULT_PASSWORD "1071"
#define MIN_WORKING_HOUR 9
#define MAX_WORKING_HOUR 24
#define DAYS_AHEAD 3
#define SLOTS_PER_ROW 3

/* File names */
#define DOCTOR_FILE "doctors.dat"
#define PATIENT_FILE "patients.dat"
#define APPOINTMENT_FILE "appointments.dat"
#define PASSWORD_FILE "pass.txt"
#define LOG_FILE "hospital_log.txt"
#define BILL_FILE_PREFIX "bill_"

/* Gender constants */
#define GENDER_MALE 1
#define GENDER_FEMALE 2
#define GENDER_OTHER 3

/* Data type constants for cleanup */
#define DATA_TYPE_DOCTOR 1
#define DATA_TYPE_PATIENT 2
#define DATA_TYPE_APPOINTMENT 3

/* ==================== STRUCTURES ==================== */
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char medical_notes[MAX_MEDICAL_NOTES_LENGTH];
    int age;
    int gender;
} Patient;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char specialisation[MAX_SPECIALISATION_LENGTH];
    float charge;
    int appointments;
} Doctor;

typedef struct {
    int date;
    int month;
    float hour;
} TimeSlot;

typedef struct {
    int patient_id;
    int doctor_id;
    int time_slot_index;
    char specialisation[MAX_SPECIALISATION_LENGTH];
    float charge;
    bool is_discharged;
    time_t admission_time;
    time_t discharge_time;
} Appointment;

typedef struct {
    int patient_id;
    int doctor_id;
    float consultation_charge;
    float additional_charges;
    float total_amount;
    time_t bill_date;
    char specialisation[MAX_SPECIALISATION_LENGTH];
} Bill;

/* Patient Queue Node for real-time queue simulation */
typedef struct QueueNode {
    int patient_id;
    int priority;  /* Lower number = higher priority */
    struct QueueNode *next;
} QueueNode;

/* ==================== GLOBAL VARIABLES ==================== */
static Doctor *doctor_list = NULL;
static Patient *patient_list = NULL;
static Appointment *appointment_list = NULL;
static TimeSlot *time_slot_list = NULL;
static Bill *bill_list = NULL;

static int num_doctors = 0;
static int num_patients = 0;
static int num_time_slots = 0;
static int num_appointments = 0;
static int num_bills = 0;
static bool is_doctor_list_sorted = false;
static char admin_password[MAX_PASSWORD_LENGTH];

/* Patient Queue (FIFO with priority) */
static QueueNode *queue_front = NULL;
static QueueNode *queue_rear = NULL;
static int queue_size = 0;

/* ==================== FORWARD DECLARATIONS ==================== */
/* File Operations */
static int load_data_from_files(void);
static int save_data_to_files(void);
static int create_file_if_not_exists(const char *filename);
static FILE *open_file_safe(const char *filename, const char *mode);

/* Memory Management */
static void *safe_realloc(void *ptr, size_t size);
static void *safe_malloc(size_t size);
static void cleanup_memory(void);
static void cleanup_data_type(int data_type);

/* Doctor Management */
static int add_doctor(void);
static int edit_doctor(void);
static int remove_doctor(void);
static int find_doctor_by_id(int doctor_id);
static void manage_doctors(void);
static void display_doctors_by_specialisation(void);
static void sort_doctors_by_specialisation(void);
static void remove_appointments_for_doctor(int doctor_id);

/* Patient Management */
static int register_patient(void);
static int login_patient(int *patient_id);
static void manage_patient_appointments(int patient_id);
static bool display_existing_appointments(int patient_id);
static int select_specialisation(void);
static int find_doctor_with_min_appointments(int specialisation_index);
static int select_available_time_slot(int doctor_id);
static void create_appointment(int patient_id, int doctor_id, int time_slot_index);
static void display_all_patients(void);

/* Appointment Management */
static void display_all_appointments(void);
static bool is_time_slot_available(int doctor_id, int time_slot_index);
static void generate_time_slots(void);

/* Authentication */
static int authenticate_admin(void);
static int load_password(void);
static int save_password(const char *new_password);
static int reset_password(void);

/* Utility Functions */
static int get_valid_id(const char *prompt, bool check_duplicate, int *existing_ids, int count);
static int get_valid_choice(const char *prompt, int min, int max);
static void clear_input_buffer(void);

/* Logging Functions */
static int write_log(const char *action, const char *details);
static int log_patient_registration(int patient_id, const char *patient_name);
static int log_appointment_creation(int patient_id, int doctor_id);
static int log_discharge(int patient_id, int doctor_id, time_t discharge_time);
static int log_doctor_operation(const char *operation, int doctor_id);

/* Bill Management */
static int generate_bill(int patient_id, int doctor_id, float additional_charges);
static int save_bill_to_file(const Bill *bill);
static void display_bill(const Bill *bill);
static int load_bills_from_file(void);

/* Discharge Management */
static int discharge_patient(int patient_id);
static void update_medical_notes(int patient_id);

/* Queue Management */
static int enqueue_patient(int patient_id, int priority);
static int dequeue_patient(int *patient_id);
static void display_queue(void);
static void clear_queue(void);

/* Search Functions */
static void search_patient_by_id(void);
static void search_patient_by_name(void);
static void search_doctor_by_id(void);
static void search_doctor_by_name(void);
static void search_doctor_by_specialisation(void);

/* ==================== MEMORY MANAGEMENT ==================== */

/**
 * Safe realloc with error checking
 */
static void *safe_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL && size > 0) {
        fprintf(stderr, "Error: Memory reallocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

/**
 * Safe malloc with error checking
 */
static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * Cleanup all allocated memory
 */
static void cleanup_memory(void) {
    free(doctor_list);
    doctor_list = NULL;
    free(patient_list);
    patient_list = NULL;
    free(appointment_list);
    appointment_list = NULL;
    free(time_slot_list);
    time_slot_list = NULL;
    free(bill_list);
    bill_list = NULL;
    clear_queue();
}

/**
 * Cleanup specific data type
 */
static void cleanup_data_type(int data_type) {
    FILE *fp;
    
    switch (data_type) {
        case DATA_TYPE_DOCTOR:
            fp = open_file_safe(DOCTOR_FILE, "wb");
            if (fp) fclose(fp);
            free(doctor_list);
            doctor_list = NULL;
            num_doctors = 0;
            break;
            
        case DATA_TYPE_PATIENT:
            fp = open_file_safe(PATIENT_FILE, "wb");
            if (fp) fclose(fp);
            free(patient_list);
            patient_list = NULL;
            num_patients = 0;
            break;
            
        case DATA_TYPE_APPOINTMENT:
            fp = open_file_safe(APPOINTMENT_FILE, "wb");
            if (fp) fclose(fp);
            free(appointment_list);
            appointment_list = NULL;
            num_appointments = 0;
            break;
    }
}

/* ==================== FILE OPERATIONS ==================== */

/**
 * Create file if it doesn't exist
 */
static int create_file_if_not_exists(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            fprintf(stderr, "Error: Cannot create file %s\n", filename);
            return 0;
        }
        fclose(fp);
    } else {
        fclose(fp);
    }
    return 1;
}

/**
 * Open file with error checking
 */
static FILE *open_file_safe(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL && (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0)) {
        if (!create_file_if_not_exists(filename)) {
            return NULL;
        }
        fp = fopen(filename, mode);
    }
    return fp;
}

/**
 * Load all data from binary files
 */
static int load_data_from_files(void) {
    FILE *fp;
    Doctor doctor;
    Patient patient;
    Appointment appointment;
    
    /* Initialize counters */
    num_doctors = 0;
    num_patients = 0;
    num_appointments = 0;
    
    /* Load doctors */
    fp = open_file_safe(DOCTOR_FILE, "rb");
    if (fp != NULL) {
        /* Check if file has data */
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (file_size > 0) {
            while (fread(&doctor, sizeof(Doctor), 1, fp) == 1) {
                num_doctors++;
                doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(Doctor));
                doctor_list[num_doctors - 1] = doctor;
            }
        }
        fclose(fp);
        if (num_doctors > 0) {
            printf("Loaded %d doctor(s) from %s\n", num_doctors, DOCTOR_FILE);
        }
    }
    
    /* Load patients */
    fp = open_file_safe(PATIENT_FILE, "rb");
    if (fp != NULL) {
        /* Check if file has data */
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (file_size > 0) {
            while (fread(&patient, sizeof(Patient), 1, fp) == 1) {
                num_patients++;
                patient_list = safe_realloc(patient_list, num_patients * sizeof(Patient));
                patient_list[num_patients - 1] = patient;
            }
        }
        fclose(fp);
        if (num_patients > 0) {
            printf("Loaded %d patient(s) from %s\n", num_patients, PATIENT_FILE);
        }
    }
    
    /* Load appointments */
    fp = open_file_safe(APPOINTMENT_FILE, "rb");
    if (fp != NULL) {
        /* Check if file has data */
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (file_size > 0) {
            while (fread(&appointment, sizeof(Appointment), 1, fp) == 1) {
                num_appointments++;
                appointment_list = safe_realloc(appointment_list, num_appointments * sizeof(Appointment));
                appointment_list[num_appointments - 1] = appointment;
                /* Initialize new fields if they weren't set (for backward compatibility) */
                if (appointment_list[num_appointments - 1].admission_time == 0) {
                    appointment_list[num_appointments - 1].admission_time = time(NULL);
                }
            }
        }
        fclose(fp);
        if (num_appointments > 0) {
            printf("Loaded %d appointment(s) from %s\n", num_appointments, APPOINTMENT_FILE);
        }
    }
    
    /* Generate time slots */
    generate_time_slots();
    
    return 1;
}

/**
 * Save all data to binary files
 */
static int save_data_to_files(void) {
    FILE *fp;
    int saved_count = 0;
    
    /* Save doctors */
    fp = fopen(DOCTOR_FILE, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot save doctor file\n");
    } else {
        if (num_doctors > 0) {
            size_t written = fwrite(doctor_list, sizeof(Doctor), num_doctors, fp);
            if (written == num_doctors) {
                saved_count++;
            }
        } else {
            /* Save empty file to maintain file structure */
            saved_count++;
        }
        fclose(fp);
    }
    
    /* Save patients */
    fp = fopen(PATIENT_FILE, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot save patient file\n");
    } else {
        if (num_patients > 0) {
            size_t written = fwrite(patient_list, sizeof(Patient), num_patients, fp);
            if (written == num_patients) {
                saved_count++;
            }
        } else {
            /* Save empty file to maintain file structure */
            saved_count++;
        }
        fclose(fp);
    }
    
    /* Save appointments */
    fp = fopen(APPOINTMENT_FILE, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot save appointment file\n");
    } else {
        if (num_appointments > 0) {
            size_t written = fwrite(appointment_list, sizeof(Appointment), num_appointments, fp);
            if (written == num_appointments) {
                saved_count++;
            }
        } else {
            /* Save empty file to maintain file structure */
            saved_count++;
        }
        fclose(fp);
    }
    
    return (saved_count == 3) ? 1 : 0;
}

/* ==================== TIME SLOT GENERATION ==================== */

/**
 * Generate time slots for the next few days
 */
static void generate_time_slots(void) {
    time_t current_time = time(NULL);
    time_t future_time;
    struct tm *time_info = localtime(&current_time);
    int current_day = time_info->tm_mday;
    
    for (int day_offset = 1; day_offset <= DAYS_AHEAD; day_offset++) {
        for (int hour = MIN_WORKING_HOUR; hour < MAX_WORKING_HOUR; hour++) {
            future_time = current_time + (hour * 3600) + (day_offset * 24 * 3600);
            struct tm *future_info = localtime(&future_time);
            
            num_time_slots++;
            time_slot_list = safe_realloc(time_slot_list, num_time_slots * sizeof(TimeSlot));
            time_slot_list[num_time_slots - 1].hour = hour;
            time_slot_list[num_time_slots - 1].date = future_info->tm_mday;
            time_slot_list[num_time_slots - 1].month = future_info->tm_mon;
        }
    }
}

/* ==================== UTILITY FUNCTIONS ==================== */

/**
 * Clear input buffer
 */
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Get valid ID with duplicate checking
 */
static int get_valid_id(const char *prompt, bool check_duplicate, int *existing_ids, int count) {
    int id;
    bool is_valid = false;
    
    while (!is_valid) {
        printf("%s: ", prompt);
        if (scanf("%d", &id) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        clear_input_buffer();
        
        id = abs(id);
        is_valid = true;
        
        if (check_duplicate) {
            for (int i = 0; i < count; i++) {
                if (existing_ids[i] == id) {
                    printf("ID already exists. Please try again.\n");
                    is_valid = false;
                    break;
                }
            }
        }
    }
    
    return id;
}

/**
 * Get valid choice within range
 */
static int get_valid_choice(const char *prompt, int min, int max) {
    int choice;
    
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        clear_input_buffer();
        
        if (choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
    }
}

/* ==================== DOCTOR MANAGEMENT ==================== */

/**
 * Sort doctors by specialisation
 */
static void sort_doctors_by_specialisation(void) {
    if (num_doctors <= 1) {
        is_doctor_list_sorted = true;
        return;
    }
    
    Doctor temp;
    for (int j = 0; j < num_doctors; j++) {
        for (int i = 0; i < num_doctors - j - 1; i++) {
            if (strcmp(doctor_list[i].specialisation, doctor_list[i + 1].specialisation) > 0) {
                temp = doctor_list[i];
                doctor_list[i] = doctor_list[i + 1];
                doctor_list[i + 1] = temp;
            }
        }
    }
    is_doctor_list_sorted = true;
}

/**
 * Find doctor by ID
 */
static int find_doctor_by_id(int doctor_id) {
    for (int i = 0; i < num_doctors; i++) {
        if (doctor_list[i].id == doctor_id) {
            return i;
        }
    }
    return -1;
}

/**
 * Add a new doctor
 */
static int add_doctor(void) {
    Doctor new_doctor;
    int existing_ids[1000];
    
    /* Collect existing IDs */
    for (int i = 0; i < num_doctors; i++) {
        existing_ids[i] = doctor_list[i].id;
    }
    
    new_doctor.id = get_valid_id("Enter Doctor ID", true, existing_ids, num_doctors);
    
    printf("Enter Doctor Name: ");
    if (scanf(" %49[^\n]", new_doctor.name) != 1) {
        clear_input_buffer();
        printf("Error reading name.\n");
        return 0;
    }
    
    printf("Enter Specialisation: ");
    if (scanf(" %49[^\n]", new_doctor.specialisation) != 1) {
        clear_input_buffer();
        printf("Error reading specialisation.\n");
        return 0;
    }
    
    printf("Enter Consultation Charge: ");
    if (scanf("%f", &new_doctor.charge) != 1) {
        clear_input_buffer();
        printf("Error reading charge.\n");
        return 0;
    }
    
    new_doctor.appointments = 0;
    
    num_doctors++;
    doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(Doctor));
    doctor_list[num_doctors - 1] = new_doctor;
    
    is_doctor_list_sorted = false;
    printf("Doctor added successfully!\n");
    log_doctor_operation("ADD", new_doctor.id);
    return 1;
}

/**
 * Edit an existing doctor
 */
static int edit_doctor(void) {
    int doctor_id, index;
    
    doctor_id = get_valid_id("Enter Doctor ID to edit", false, NULL, 0);
    index = find_doctor_by_id(doctor_id);
    
    if (index == -1) {
        printf("Doctor not found.\n");
        return 0;
    }
    
    printf("\nCurrent Doctor Information:\n");
    printf("ID: %d\n", doctor_list[index].id);
    printf("Name: %s\n", doctor_list[index].name);
    printf("Specialisation: %s\n", doctor_list[index].specialisation);
    printf("Charge: %.2f\n\n", doctor_list[index].charge);
    
    printf("Enter New Name: ");
    if (scanf(" %49[^\n]", doctor_list[index].name) != 1) {
        clear_input_buffer();
    }
    
    printf("Enter New Specialisation: ");
    if (scanf(" %49[^\n]", doctor_list[index].specialisation) != 1) {
        clear_input_buffer();
    }
    
    printf("Enter New Charge: ");
    if (scanf("%f", &doctor_list[index].charge) != 1) {
        clear_input_buffer();
    }
    
    doctor_list[index].appointments = 0;
    is_doctor_list_sorted = false;
    
    printf("Doctor information updated successfully!\n");
    log_doctor_operation("EDIT", doctor_id);
    return 1;
}

/**
 * Remove appointments for a specific doctor
 */
static void remove_appointments_for_doctor(int doctor_id) {
    for (int i = 0; i < num_appointments; i++) {
        if (appointment_list[i].doctor_id == doctor_id) {
            /* Shift remaining appointments */
            for (int j = i; j < num_appointments - 1; j++) {
                appointment_list[j] = appointment_list[j + 1];
            }
            num_appointments--;
            i--;
            
            if (num_appointments > 0) {
                appointment_list = safe_realloc(appointment_list, num_appointments * sizeof(Appointment));
            } else {
                cleanup_data_type(DATA_TYPE_APPOINTMENT);
            }
        }
    }
}

/**
 * Remove a doctor
 */
static int remove_doctor(void) {
    int doctor_id, index;
    
    doctor_id = get_valid_id("Enter Doctor ID to remove", false, NULL, 0);
    index = find_doctor_by_id(doctor_id);
    
    if (index == -1) {
        printf("Doctor not found.\n");
        return 0;
    }
    
    /* Remove associated appointments */
    remove_appointments_for_doctor(doctor_id);
    
    /* Shift remaining doctors */
    for (int i = index; i < num_doctors - 1; i++) {
        doctor_list[i] = doctor_list[i + 1];
    }
    
    num_doctors--;
    if (num_doctors > 0) {
        doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(Doctor));
    } else {
        cleanup_data_type(DATA_TYPE_DOCTOR);
    }
    
    is_doctor_list_sorted = false;
    printf("Doctor removed successfully!\n");
    log_doctor_operation("DELETE", doctor_id);
    return 1;
}

/**
 * Manage doctors (add/edit/remove)
 */
static void manage_doctors(void) {
    int choice;
    bool continue_management = true;
    
    while (continue_management) {
        printf("\n=== Doctor Management ===\n");
        printf("1. Add Doctor\n");
        printf("2. Edit Doctor\n");
        printf("3. Remove Doctor\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue_management = false;
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                add_doctor();
                break;
            case 2:
                edit_doctor();
                break;
            case 3:
                remove_doctor();
                break;
            case 4:
            default:
                continue_management = false;
                break;
        }
    }
}

/**
 * Display doctors grouped by specialisation
 */
static void display_doctors_by_specialisation(void) {
    if (doctor_list == NULL || num_doctors == 0) {
        printf("\nDoctor list is empty.\n");
        return;
    }
    
    if (!is_doctor_list_sorted) {
        sort_doctors_by_specialisation();
    }
    
    char current_specialisation[MAX_SPECIALISATION_LENGTH];
    strcpy(current_specialisation, doctor_list[0].specialisation);
    
    printf("\n=== Doctors by Specialisation ===\n");
    printf("%s:\n", current_specialisation);
    
    for (int i = 0; i < num_doctors; i++) {
        if (strcmp(current_specialisation, doctor_list[i].specialisation) != 0) {
            strcpy(current_specialisation, doctor_list[i].specialisation);
            printf("\n%s:\n", current_specialisation);
        }
        printf("  ID: %d\tName: %s\tCharge: %.2f\tAppointments: %d\n",
               doctor_list[i].id, doctor_list[i].name,
               doctor_list[i].charge, doctor_list[i].appointments);
    }
}

/* ==================== PATIENT MANAGEMENT ==================== */

/**
 * Register a new patient
 */
static int register_patient(void) {
    num_patients++;
    patient_list = safe_realloc(patient_list, num_patients * sizeof(Patient));
    
    int existing_ids[1000];
    for (int i = 0; i < num_patients - 1; i++) {
        existing_ids[i] = patient_list[i].id;
    }
    
    patient_list[num_patients - 1].id = get_valid_id("Enter Patient ID", true, existing_ids, num_patients - 1);
    
    printf("Enter Patient Name: ");
    if (scanf(" %49[^\n]", patient_list[num_patients - 1].name) != 1) {
        clear_input_buffer();
    }
    
    printf("Enter Age: ");
    if (scanf("%d", &patient_list[num_patients - 1].age) != 1) {
        clear_input_buffer();
    }
    
    printf("Enter Gender:\n");
    printf("1. Male\n");
    printf("2. Female\n");
    printf("3. Other\n");
    patient_list[num_patients - 1].gender = get_valid_choice("Enter choice: ", 1, 3);
    
    printf("Enter Medical Notes: ");
    if (scanf(" %499[^\n]", patient_list[num_patients - 1].medical_notes) != 1) {
        clear_input_buffer();
    }
    
    printf("Patient registered successfully!\n");
    int registered_id = patient_list[num_patients - 1].id;
    log_patient_registration(registered_id, patient_list[num_patients - 1].name);
    return registered_id;
}

/**
 * Login patient by ID
 */
static int login_patient(int *patient_id) {
    if (patient_list == NULL || num_patients == 0) {
        printf("\nPatient list is empty. Please register first.\n");
        return 0;
    }
    
    int id;
    bool found = false;
    
    while (!found) {
        printf("Enter Patient ID (or -1 to go back): ");
        if (scanf("%d", &id) != 1) {
            clear_input_buffer();
            printf("Invalid input.\n");
            continue;
        }
        clear_input_buffer();
        
        if (id == -1) {
            return 0;
        }
        
        id = abs(id);
        for (int i = 0; i < num_patients; i++) {
            if (patient_list[i].id == id) {
                found = true;
                *patient_id = id;
                break;
            }
        }
        
        if (!found) {
            printf("ID not found. Please try again or press -1 to go back.\n");
        }
    }
    
    return 1;
}

/**
 * Display existing appointments for a patient
 * Returns true if patient has appointments, false otherwise
 */
static bool display_existing_appointments(int patient_id) {
    bool has_appointments = false;
    
    for (int i = 0; i < num_appointments; i++) {
        if (appointment_list[i].patient_id == patient_id) {
            if (!has_appointments) {
                printf("\n=== Your Existing Appointments ===\n");
                printf("DoctorID\tPatientID\tSpecialisation\tTimeSlot\t\tCharge\n");
                has_appointments = true;
            }
            
            int slot_idx = appointment_list[i].time_slot_index;
            printf("%d\t\t%d\t\t%s\t\t%.2f, %d, %d\t\t%.2f\n",
                   appointment_list[i].doctor_id,
                   appointment_list[i].patient_id,
                   appointment_list[i].specialisation,
                   time_slot_list[slot_idx].hour,
                   time_slot_list[slot_idx].date,
                   time_slot_list[slot_idx].month,
                   appointment_list[i].charge);
        }
    }
    
    return has_appointments;
}

/**
 * Check if patient already has appointments
 */
static bool patient_has_appointments(int patient_id) {
    for (int i = 0; i < num_appointments; i++) {
        if (appointment_list[i].patient_id == patient_id) {
            return true;
        }
    }
    return false;
}

/**
 * Select specialisation from available options
 */
static int select_specialisation(void) {
    if (!is_doctor_list_sorted) {
        sort_doctors_by_specialisation();
    }
    
    /* Count unique specialisations */
    int num_specialisations = 1;
    char **specialisation_list = safe_malloc(sizeof(char *));
    specialisation_list[0] = safe_malloc(MAX_SPECIALISATION_LENGTH);
    strcpy(specialisation_list[0], doctor_list[0].specialisation);
    
    char current_spec[MAX_SPECIALISATION_LENGTH];
    strcpy(current_spec, doctor_list[0].specialisation);
    
    for (int i = 1; i < num_doctors; i++) {
        if (strcmp(current_spec, doctor_list[i].specialisation) != 0) {
            num_specialisations++;
            specialisation_list = safe_realloc(specialisation_list, num_specialisations * sizeof(char *));
            specialisation_list[num_specialisations - 1] = safe_malloc(MAX_SPECIALISATION_LENGTH);
            strcpy(specialisation_list[num_specialisations - 1], doctor_list[i].specialisation);
            strcpy(current_spec, doctor_list[i].specialisation);
        }
    }
    
    printf("\n=== Available Specialisations ===\n");
    for (int i = 0; i < num_specialisations; i++) {
        printf("%d. %s\n", i + 1, specialisation_list[i]);
    }
    
    int choice = get_valid_choice("Select specialisation: ", 1, num_specialisations);
    
    /* Free memory */
    for (int i = 0; i < num_specialisations; i++) {
        free(specialisation_list[i]);
    }
    free(specialisation_list);
    
    return choice;
}

/**
 * Find doctor with minimum appointments in a specialisation group
 */
static int find_doctor_with_min_appointments(int specialisation_index) {
    if (!is_doctor_list_sorted) {
        sort_doctors_by_specialisation();
    }
    
    /* Find start index of specialisation */
    int start_idx = 0;
    int current_spec = 1;
    
    for (int i = 0; i < num_doctors - 1 && current_spec < specialisation_index; i++) {
        if (strcmp(doctor_list[i].specialisation, doctor_list[i + 1].specialisation) != 0) {
            current_spec++;
            start_idx = i + 1;
        }
    }
    
    /* Find doctor with minimum appointments in this specialisation */
    int min_idx = start_idx;
    int end_idx = start_idx;
    
    while (end_idx + 1 < num_doctors &&
           strcmp(doctor_list[end_idx].specialisation, doctor_list[end_idx + 1].specialisation) == 0) {
        end_idx++;
        if (doctor_list[end_idx].appointments < doctor_list[min_idx].appointments) {
            min_idx = end_idx;
        }
    }
    
    doctor_list[min_idx].appointments++;
    return min_idx;
}

/**
 * Check if time slot is available for a doctor
 */
static bool is_time_slot_available(int doctor_id, int time_slot_index) {
    for (int i = 0; i < num_appointments; i++) {
        if (appointment_list[i].doctor_id == doctor_id &&
            appointment_list[i].time_slot_index == time_slot_index) {
            return false;
        }
    }
    return true;
}

/**
 * Select available time slot
 */
static int select_available_time_slot(int doctor_id) {
    printf("\n=== Available Time Slots ===\n");
    
    int available_slots[1000];
    int num_available = 0;
    int display_count = 0;
    
    for (int i = 0; i < num_time_slots; i++) {
        if (is_time_slot_available(doctor_id, i)) {
            available_slots[num_available] = i;
            num_available++;
            
            printf("%-3d. %.2f:00, Date: %d/%d\t", 
                   num_available,
                   time_slot_list[i].hour,
                   time_slot_list[i].date,
                   time_slot_list[i].month);
            
            display_count++;
            if (display_count % SLOTS_PER_ROW == 0) {
                printf("\n");
            }
        }
    }
    
    if (display_count % SLOTS_PER_ROW != 0) {
        printf("\n");
    }
    
    if (num_available == 0) {
        printf("No available time slots.\n");
        return -1;
    }
    
    int choice = get_valid_choice("Select time slot: ", 1, num_available);
    return available_slots[choice - 1];
}

/**
 * Create a new appointment
 */
static void create_appointment(int patient_id, int doctor_id, int time_slot_index) {
    num_appointments++;
    appointment_list = safe_realloc(appointment_list, num_appointments * sizeof(Appointment));
    
    int doctor_idx = find_doctor_by_id(doctor_id);
    if (doctor_idx == -1) {
        printf("Error: Doctor not found.\n");
        return;
    }
    
    appointment_list[num_appointments - 1].patient_id = patient_id;
    appointment_list[num_appointments - 1].doctor_id = doctor_id;
    appointment_list[num_appointments - 1].time_slot_index = time_slot_index;
    appointment_list[num_appointments - 1].charge = doctor_list[doctor_idx].charge;
    appointment_list[num_appointments - 1].is_discharged = false;
    appointment_list[num_appointments - 1].admission_time = time(NULL);
    appointment_list[num_appointments - 1].discharge_time = 0;
    strcpy(appointment_list[num_appointments - 1].specialisation, doctor_list[doctor_idx].specialisation);
    
    printf("\n=== Appointment Confirmed ===\n");
    printf("Dear Patient, thank you for your patience.\n");
    printf("You have been successfully appointed with Dr. %s\n", doctor_list[doctor_idx].name);
    printf("Specialisation: %s\n", appointment_list[num_appointments - 1].specialisation);
    printf("Time Slot: %.2f:00, Date: %d/%d\n",
           time_slot_list[time_slot_index].hour,
           time_slot_list[time_slot_index].date,
           time_slot_list[time_slot_index].month);
    printf("Total Charge: %.2f\n", appointment_list[num_appointments - 1].charge);
    
    log_appointment_creation(patient_id, doctor_id);
}

/**
 * Manage patient appointments
 */
static void manage_patient_appointments(int patient_id) {
    if (doctor_list == NULL || num_doctors == 0) {
        printf("Sorry, no doctors are currently available.\n");
        printf("Please try again later using the Patient Login option.\n");
        return;
    }
    
    /* Check if patient already has appointments */
    if (patient_has_appointments(patient_id)) {
        display_existing_appointments(patient_id);
        printf("\nYou already have an appointment. Please contact admin for modifications.\n");
        return;
    }
    
    /* Select specialisation */
    int specialisation_choice = select_specialisation();
    
    /* Find doctor with minimum appointments */
    int doctor_idx = find_doctor_with_min_appointments(specialisation_choice);
    int selected_doctor_id = doctor_list[doctor_idx].id;
    
    /* Select time slot */
    int time_slot_index = select_available_time_slot(selected_doctor_id);
    if (time_slot_index == -1) {
        doctor_list[doctor_idx].appointments--; /* Revert appointment count */
        return;
    }
    
    /* Create appointment */
    create_appointment(patient_id, selected_doctor_id, time_slot_index);
}

/**
 * Display all patients
 */
static void display_all_patients(void) {
    if (patient_list == NULL || num_patients == 0) {
        printf("\nNo patients registered.\n");
        return;
    }
    
    printf("\n=== All Patients ===\n");
    printf("ID\tName\t\tAge\tGender\t\tMedical Notes\n");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < num_patients; i++) {
        const char *gender_str;
        switch (patient_list[i].gender) {
            case GENDER_MALE:
                gender_str = "Male";
                break;
            case GENDER_FEMALE:
                gender_str = "Female";
                break;
            case GENDER_OTHER:
                gender_str = "Other";
                break;
            default:
                gender_str = "Unknown";
        }
        
        printf("%d\t%s\t\t%d\t%s\t\t%s\n",
               patient_list[i].id,
               patient_list[i].name,
               patient_list[i].age,
               gender_str,
               patient_list[i].medical_notes);
    }
}

/* ==================== APPOINTMENT MANAGEMENT ==================== */

/**
 * Display all appointments
 */
static void display_all_appointments(void) {
    if (appointment_list == NULL || num_appointments == 0) {
        printf("\nNo appointments scheduled.\n");
        return;
    }
    
    printf("\n=== All Appointments ===\n");
    printf("DoctorID\tPatientID\tSpecialisation\t\tTimeSlot\t\tCharge\n");
    printf("----------------------------------------------------------------------------\n");
    
    for (int i = 0; i < num_appointments; i++) {
        int slot_idx = appointment_list[i].time_slot_index;
        printf("%d\t\t%d\t\t%s\t\t\t%.2f:00, %d/%d\t\t%.2f\n",
               appointment_list[i].doctor_id,
               appointment_list[i].patient_id,
               appointment_list[i].specialisation,
               time_slot_list[slot_idx].hour,
               time_slot_list[slot_idx].date,
               time_slot_list[slot_idx].month,
               appointment_list[i].charge);
    }
}

/* ==================== AUTHENTICATION ==================== */

/**
 * Load password from file
 */
static int load_password(void) {
    FILE *fp = open_file_safe(PASSWORD_FILE, "r");
    if (fp == NULL) {
        /* Create default password file */
        fp = fopen(PASSWORD_FILE, "w");
        if (fp == NULL) {
            fprintf(stderr, "Error: Cannot create password file\n");
            return 0;
        }
        fprintf(fp, "%s", DEFAULT_PASSWORD);
        fclose(fp);
        strcpy(admin_password, DEFAULT_PASSWORD);
        return 1;
    }
    
    if (fscanf(fp, " %49[^\n]", admin_password) != 1) {
        strcpy(admin_password, DEFAULT_PASSWORD);
    }
    fclose(fp);
    return 1;
}

/**
 * Save password to file
 */
static int save_password(const char *new_password) {
    FILE *fp = fopen(PASSWORD_FILE, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot save password file\n");
        return 0;
    }
    fprintf(fp, "%s", new_password);
    fclose(fp);
    strcpy(admin_password, new_password);
    return 1;
}

/**
 * Reset admin password
 */
static int reset_password(void) {
    char new_password[MAX_PASSWORD_LENGTH];
    
    printf("Enter new password: ");
    if (scanf(" %49[^\n]", new_password) != 1) {
        clear_input_buffer();
        printf("Error reading password.\n");
        return 0;
    }
    
    if (save_password(new_password)) {
        printf("Password updated successfully!\n");
        return 1;
    }
    return 0;
}

/**
 * Authenticate admin user
 */
static int authenticate_admin(void) {
    char entered_password[MAX_PASSWORD_LENGTH];
    
    if (!load_password()) {
        return 0;
    }
    
    printf("Enter Admin Password: ");
    if (scanf(" %49[^\n]", entered_password) != 1) {
        clear_input_buffer();
        return 0;
    }
    
    if (strcmp(entered_password, admin_password) != 0) {
        printf("Wrong password. Access denied.\n");
        write_log("AUTHENTICATION", "Failed admin login attempt");
        return 0;
    }
    
    write_log("AUTHENTICATION", "Successful admin login");
    return 1;
}

/* ==================== LOGGING FUNCTIONS ==================== */

/**
 * Write log entry to hospital_log.txt
 */
static int write_log(const char *action, const char *details) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp == NULL) {
        return 0;
    }
    
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    
    fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s: %s\n",
            time_info->tm_year + 1900,
            time_info->tm_mon + 1,
            time_info->tm_mday,
            time_info->tm_hour,
            time_info->tm_min,
            time_info->tm_sec,
            action,
            details);
    
    fclose(fp);
    return 1;
}

/**
 * Log patient registration
 */
static int log_patient_registration(int patient_id, const char *patient_name) {
    char details[200];
    snprintf(details, sizeof(details), "Patient ID: %d, Name: %s registered", patient_id, patient_name);
    return write_log("PATIENT_REGISTRATION", details);
}

/**
 * Log appointment creation
 */
static int log_appointment_creation(int patient_id, int doctor_id) {
    char details[200];
    snprintf(details, sizeof(details), "Patient ID: %d assigned to Doctor ID: %d", patient_id, doctor_id);
    return write_log("APPOINTMENT_CREATED", details);
}

/**
 * Log patient discharge
 */
static int log_discharge(int patient_id, int doctor_id, time_t discharge_time) {
    char details[300];
    struct tm *time_info = localtime(&discharge_time);
    snprintf(details, sizeof(details), 
             "Patient ID: %d discharged from Doctor ID: %d at %04d-%02d-%02d %02d:%02d:%02d",
             patient_id, doctor_id,
             time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    return write_log("PATIENT_DISCHARGE", details);
}

/**
 * Log doctor operations
 */
static int log_doctor_operation(const char *operation, int doctor_id) {
    char details[200];
    snprintf(details, sizeof(details), "%s - Doctor ID: %d", operation, doctor_id);
    return write_log("DOCTOR_OPERATION", details);
}

/* ==================== BILL MANAGEMENT ==================== */

/**
 * Generate bill for a patient
 */
static int generate_bill(int patient_id, int doctor_id, float additional_charges) {
    int doctor_idx = find_doctor_by_id(doctor_id);
    if (doctor_idx == -1) {
        printf("Error: Doctor not found.\n");
        return 0;
    }
    
    Bill new_bill;
    new_bill.patient_id = patient_id;
    new_bill.doctor_id = doctor_id;
    new_bill.consultation_charge = doctor_list[doctor_idx].charge;
    new_bill.additional_charges = additional_charges;
    new_bill.total_amount = new_bill.consultation_charge + new_bill.additional_charges;
    new_bill.bill_date = time(NULL);
    strcpy(new_bill.specialisation, doctor_list[doctor_idx].specialisation);
    
    num_bills++;
    bill_list = safe_realloc(bill_list, num_bills * sizeof(Bill));
    bill_list[num_bills - 1] = new_bill;
    
    display_bill(&new_bill);
    save_bill_to_file(&new_bill);
    
    char log_details[300];
    snprintf(log_details, sizeof(log_details), 
             "Bill generated for Patient ID: %d, Doctor ID: %d, Total: %.2f",
             patient_id, doctor_id, new_bill.total_amount);
    write_log("BILL_GENERATED", log_details);
    
    return 1;
}

/**
 * Save bill to individual text file
 */
static int save_bill_to_file(const Bill *bill) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s%d_%ld.txt", BILL_FILE_PREFIX, 
             bill->patient_id, (long)bill->bill_date);
    
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return 0;
    }
    
    struct tm *time_info = localtime(&bill->bill_date);
    int patient_idx = -1;
    int doctor_idx = find_doctor_by_id(bill->doctor_id);
    
    for (int i = 0; i < num_patients; i++) {
        if (patient_list[i].id == bill->patient_id) {
            patient_idx = i;
            break;
        }
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "        HOSPITAL BILL RECEIPT\n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "Date: %04d-%02d-%02d %02d:%02d:%02d\n",
            time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
            time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    fprintf(fp, "----------------------------------------\n");
    
    if (patient_idx >= 0) {
        fprintf(fp, "Patient ID: %d\n", bill->patient_id);
        fprintf(fp, "Patient Name: %s\n", patient_list[patient_idx].name);
        fprintf(fp, "Age: %d\n", patient_list[patient_idx].age);
    }
    
    if (doctor_idx >= 0) {
        fprintf(fp, "Doctor ID: %d\n", bill->doctor_id);
        fprintf(fp, "Doctor Name: %s\n", doctor_list[doctor_idx].name);
        fprintf(fp, "Specialisation: %s\n", bill->specialisation);
    }
    
    fprintf(fp, "----------------------------------------\n");
    fprintf(fp, "Consultation Charge:     %.2f\n", bill->consultation_charge);
    fprintf(fp, "Additional Charges:      %.2f\n", bill->additional_charges);
    fprintf(fp, "----------------------------------------\n");
    fprintf(fp, "TOTAL AMOUNT:            %.2f\n", bill->total_amount);
    fprintf(fp, "========================================\n");
    
    fclose(fp);
    return 1;
}

/**
 * Display bill on screen
 */
static void display_bill(const Bill *bill) {
    int patient_idx = -1;
    int doctor_idx = find_doctor_by_id(bill->doctor_id);
    
    for (int i = 0; i < num_patients; i++) {
        if (patient_list[i].id == bill->patient_id) {
            patient_idx = i;
            break;
        }
    }
    
    struct tm *time_info = localtime(&bill->bill_date);
    
    printf("\n========================================\n");
    printf("        HOSPITAL BILL RECEIPT\n");
    printf("========================================\n");
    printf("Date: %04d-%02d-%02d %02d:%02d:%02d\n",
           time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
           time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    printf("----------------------------------------\n");
    
    if (patient_idx >= 0) {
        printf("Patient ID: %d\n", bill->patient_id);
        printf("Patient Name: %s\n", patient_list[patient_idx].name);
    }
    
    if (doctor_idx >= 0) {
        printf("Doctor: Dr. %s\n", doctor_list[doctor_idx].name);
        printf("Specialisation: %s\n", bill->specialisation);
    }
    
    printf("----------------------------------------\n");
    printf("Consultation Charge:     %.2f\n", bill->consultation_charge);
    printf("Additional Charges:      %.2f\n", bill->additional_charges);
    printf("----------------------------------------\n");
    printf("TOTAL AMOUNT:            %.2f\n", bill->total_amount);
    printf("========================================\n");
}

/**
 * Load bills from file (optional - for history)
 */
static int load_bills_from_file(void) {
    /* Bills are stored in individual files, so we don't need to load them all */
    /* This function can be extended if needed */
    return 1;
}

/* ==================== DISCHARGE MANAGEMENT ==================== */

/**
 * Update medical notes for a patient
 */
static void update_medical_notes(int patient_id) {
    int patient_idx = -1;
    for (int i = 0; i < num_patients; i++) {
        if (patient_list[i].id == patient_id) {
            patient_idx = i;
            break;
        }
    }
    
    if (patient_idx == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    printf("Current Medical Notes: %s\n", patient_list[patient_idx].medical_notes);
    printf("Enter additional medical notes (or press Enter to skip): ");
    
    char additional_notes[500];
    if (scanf(" %499[^\n]", additional_notes) == 1) {
        if (strlen(patient_list[patient_idx].medical_notes) + strlen(additional_notes) + 2 < MAX_MEDICAL_NOTES_LENGTH) {
            strcat(patient_list[patient_idx].medical_notes, "\n");
            strcat(patient_list[patient_idx].medical_notes, additional_notes);
        }
    }
}

/**
 * Discharge a patient
 */
static int discharge_patient(int patient_id) {
    int appointment_idx = -1;
    
    for (int i = 0; i < num_appointments; i++) {
        if (appointment_list[i].patient_id == patient_id && !appointment_list[i].is_discharged) {
            appointment_idx = i;
            break;
        }
    }
    
    if (appointment_idx == -1) {
        printf("No active appointment found for this patient.\n");
        return 0;
    }
    
    time_t discharge_time = time(NULL);
    appointment_list[appointment_idx].discharge_time = discharge_time;
    appointment_list[appointment_idx].is_discharged = true;
    
    update_medical_notes(patient_id);
    
    printf("Enter additional charges (if any): ");
    float additional_charges = 0.0;
    if (scanf("%f", &additional_charges) != 1) {
        clear_input_buffer();
    }
    
    generate_bill(patient_id, appointment_list[appointment_idx].doctor_id, additional_charges);
    log_discharge(patient_id, appointment_list[appointment_idx].doctor_id, discharge_time);
    
    printf("Patient discharged successfully!\n");
    return 1;
}

/* ==================== QUEUE MANAGEMENT ==================== */

/**
 * Enqueue patient to queue (priority-based)
 */
static int enqueue_patient(int patient_id, int priority) {
    QueueNode *new_node = safe_malloc(sizeof(QueueNode));
    new_node->patient_id = patient_id;
    new_node->priority = priority;
    new_node->next = NULL;
    
    if (queue_front == NULL) {
        queue_front = queue_rear = new_node;
    } else {
        /* Insert based on priority (lower number = higher priority) */
        QueueNode *current = queue_front;
        QueueNode *prev = NULL;
        
        while (current != NULL && current->priority <= priority) {
            prev = current;
            current = current->next;
        }
        
        if (prev == NULL) {
            /* Insert at front */
            new_node->next = queue_front;
            queue_front = new_node;
        } else {
            /* Insert in middle or end */
            prev->next = new_node;
            new_node->next = current;
            if (current == NULL) {
                queue_rear = new_node;
            }
        }
    }
    
    queue_size++;
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Patient ID: %d added to queue with priority: %d", patient_id, priority);
    write_log("QUEUE_OPERATION", log_details);
    return 1;
}

/**
 * Dequeue patient from queue
 */
static int dequeue_patient(int *patient_id) {
    if (queue_front == NULL) {
        return 0;
    }
    
    QueueNode *temp = queue_front;
    *patient_id = temp->patient_id;
    queue_front = queue_front->next;
    
    if (queue_front == NULL) {
        queue_rear = NULL;
    }
    
    free(temp);
    queue_size--;
    
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Patient ID: %d removed from queue", *patient_id);
    write_log("QUEUE_OPERATION", log_details);
    return 1;
}

/**
 * Display current queue
 */
static void display_queue(void) {
    if (queue_front == NULL) {
        printf("\nQueue is empty.\n");
        return;
    }
    
    printf("\n=== Patient Queue ===\n");
    printf("Position\tPatient ID\tPriority\n");
    printf("----------------------------------------\n");
    
    QueueNode *current = queue_front;
    int position = 1;
    
    while (current != NULL) {
        printf("%d\t\t%d\t\t%d\n", position++, current->patient_id, current->priority);
        current = current->next;
    }
    printf("Total patients in queue: %d\n", queue_size);
}

/**
 * Clear the queue
 */
static void clear_queue(void) {
    while (queue_front != NULL) {
        QueueNode *temp = queue_front;
        queue_front = queue_front->next;
        free(temp);
    }
    queue_rear = NULL;
    queue_size = 0;
}

/* ==================== SEARCH FUNCTIONS ==================== */

/**
 * Search patient by ID
 */
static void search_patient_by_id(void) {
    int search_id = get_valid_id("Enter Patient ID to search", false, NULL, 0);
    
    for (int i = 0; i < num_patients; i++) {
        if (patient_list[i].id == search_id) {
            printf("\n=== Patient Found ===\n");
            printf("ID: %d\n", patient_list[i].id);
            printf("Name: %s\n", patient_list[i].name);
            printf("Age: %d\n", patient_list[i].age);
            printf("Gender: %s\n", 
                   patient_list[i].gender == GENDER_MALE ? "Male" :
                   patient_list[i].gender == GENDER_FEMALE ? "Female" : "Other");
            printf("Medical Notes: %s\n", patient_list[i].medical_notes);
            return;
        }
    }
    
    printf("Patient not found.\n");
}

/**
 * Search patient by name
 */
static void search_patient_by_name(void) {
    char search_name[MAX_NAME_LENGTH];
    printf("Enter Patient Name to search: ");
    if (scanf(" %49[^\n]", search_name) != 1) {
        clear_input_buffer();
        return;
    }
    
    bool found = false;
    for (int i = 0; i < num_patients; i++) {
        if (strstr(patient_list[i].name, search_name) != NULL) {
            if (!found) {
                printf("\n=== Search Results ===\n");
                found = true;
            }
            printf("ID: %d, Name: %s, Age: %d\n", 
                   patient_list[i].id, patient_list[i].name, patient_list[i].age);
        }
    }
    
    if (!found) {
        printf("No patients found with that name.\n");
    }
}

/**
 * Search doctor by ID
 */
static void search_doctor_by_id(void) {
    int search_id = get_valid_id("Enter Doctor ID to search", false, NULL, 0);
    int idx = find_doctor_by_id(search_id);
    
    if (idx >= 0) {
        printf("\n=== Doctor Found ===\n");
        printf("ID: %d\n", doctor_list[idx].id);
        printf("Name: %s\n", doctor_list[idx].name);
        printf("Specialisation: %s\n", doctor_list[idx].specialisation);
        printf("Charge: %.2f\n", doctor_list[idx].charge);
        printf("Appointments: %d\n", doctor_list[idx].appointments);
    } else {
        printf("Doctor not found.\n");
    }
}

/**
 * Search doctor by name
 */
static void search_doctor_by_name(void) {
    char search_name[MAX_NAME_LENGTH];
    printf("Enter Doctor Name to search: ");
    if (scanf(" %49[^\n]", search_name) != 1) {
        clear_input_buffer();
        return;
    }
    
    bool found = false;
    for (int i = 0; i < num_doctors; i++) {
        if (strstr(doctor_list[i].name, search_name) != NULL) {
            if (!found) {
                printf("\n=== Search Results ===\n");
                found = true;
            }
            printf("ID: %d, Name: %s, Specialisation: %s, Charge: %.2f\n",
                   doctor_list[i].id, doctor_list[i].name,
                   doctor_list[i].specialisation, doctor_list[i].charge);
        }
    }
    
    if (!found) {
        printf("No doctors found with that name.\n");
    }
}

/**
 * Search doctor by specialisation
 */
static void search_doctor_by_specialisation(void) {
    char search_spec[MAX_SPECIALISATION_LENGTH];
    printf("Enter Specialisation to search: ");
    if (scanf(" %49[^\n]", search_spec) != 1) {
        clear_input_buffer();
        return;
    }
    
    bool found = false;
    for (int i = 0; i < num_doctors; i++) {
        if (strcmp(doctor_list[i].specialisation, search_spec) == 0) {
            if (!found) {
                printf("\n=== Doctors with Specialisation: %s ===\n", search_spec);
                found = true;
            }
            printf("ID: %d, Name: %s, Charge: %.2f, Appointments: %d\n",
                   doctor_list[i].id, doctor_list[i].name,
                   doctor_list[i].charge, doctor_list[i].appointments);
        }
    }
    
    if (!found) {
        printf("No doctors found with that specialisation.\n");
    }
}

/* ==================== MAIN FUNCTIONS ==================== */

/**
 * Handle patient menu
 */
static void handle_patient_menu(void) {
    int choice, patient_id;
    
    printf("\n=== Patient Menu ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Join Queue\n");
    printf("4. Back to Main Menu\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    switch (choice) {
        case 1:
            patient_id = register_patient();
            if (patient_id > 0) {
                manage_patient_appointments(patient_id);
            }
            break;
            
        case 2:
            if (login_patient(&patient_id)) {
                bool has_appointments = display_existing_appointments(patient_id);
                if (!has_appointments) {
                    manage_patient_appointments(patient_id);
                }
            }
            break;
            
        case 3: {
            int queue_patient_id = get_valid_id("Enter Patient ID to join queue", false, NULL, 0);
            int priority = get_valid_choice("Enter priority (1=High, 2=Medium, 3=Low): ", 1, 3);
            enqueue_patient(queue_patient_id, priority);
            printf("Patient added to queue successfully!\n");
            break;
        }
            
        case 4:
        default:
            break;
    }
}

/**
 * Handle admin menu
 */
static void handle_admin_menu(void) {
    if (!authenticate_admin()) {
        return;
    }
    
    int choice;
    bool continue_admin = true;
    
    while (continue_admin) {
        printf("\n=== Admin Menu ===\n");
        printf("1. Manage Doctors (Add/Edit/Delete)\n");
        printf("2. View All Patients\n");
        printf("3. View Doctors by Specialisation\n");
        printf("4. View All Appointments\n");
        printf("5. Search Patients\n");
        printf("6. Search Doctors\n");
        printf("7. Discharge Patient\n");
        printf("8. Manage Patient Queue\n");
        printf("9. Reset Password\n");
        printf("10. Clean Data Files\n");
        printf("11. Back to Main Menu\n");
        printf("Enter your choice: ");
        fflush(stdout);  /* Ensure output is flushed before input */
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue_admin = false;
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                manage_doctors();
                break;
                
            case 2:
                display_all_patients();
                break;
                
            case 3:
                display_doctors_by_specialisation();
                break;
                
            case 4:
                display_all_appointments();
                break;
                
            case 5: {
                int search_choice;
                printf("\n=== Search Patients ===\n");
                printf("1. Search by ID\n");
                printf("2. Search by Name\n");
                printf("3. Back\n");
                search_choice = get_valid_choice("Enter choice: ", 1, 3);
                if (search_choice == 1) {
                    search_patient_by_id();
                } else if (search_choice == 2) {
                    search_patient_by_name();
                }
                break;
            }
                
            case 6: {
                int search_choice;
                printf("\n=== Search Doctors ===\n");
                printf("1. Search by ID\n");
                printf("2. Search by Name\n");
                printf("3. Search by Specialisation\n");
                printf("4. Back\n");
                search_choice = get_valid_choice("Enter choice: ", 1, 4);
                if (search_choice == 1) {
                    search_doctor_by_id();
                } else if (search_choice == 2) {
                    search_doctor_by_name();
                } else if (search_choice == 3) {
                    search_doctor_by_specialisation();
                }
                break;
            }
                
            case 7: {
                int patient_id = get_valid_id("Enter Patient ID to discharge", false, NULL, 0);
                discharge_patient(patient_id);
                break;
            }
                
            case 8: {
                int queue_choice;
                printf("\n=== Patient Queue Management ===\n");
                printf("1. View Queue\n");
                printf("2. Process Next Patient (Dequeue)\n");
                printf("3. Back\n");
                queue_choice = get_valid_choice("Enter choice: ", 1, 3);
                if (queue_choice == 1) {
                    display_queue();
                } else if (queue_choice == 2) {
                    int dequeued_id;
                    if (dequeue_patient(&dequeued_id)) {
                        printf("Patient ID %d processed and removed from queue.\n", dequeued_id);
                    } else {
                        printf("Queue is empty.\n");
                    }
                }
                break;
            }
                
            case 9:
                reset_password();
                break;
                
            case 10: {
                int data_type, confirm;
                printf("\nWhich data to clean?\n");
                printf("1. Doctor Data\n");
                printf("2. Patient Data\n");
                printf("3. Appointment Data\n");
                printf("4. Cancel\n");
                data_type = get_valid_choice("Enter choice: ", 1, 4);
                
                if (data_type >= 1 && data_type <= 3) {
                    confirm = get_valid_choice("Are you sure? (1=Yes, 2=No): ", 1, 2);
                    if (confirm == 1) {
                        cleanup_data_type(data_type);
                        printf("Data cleaned successfully.\n");
                    }
                }
                break;
            }
                
            case 11:
            default:
                continue_admin = false;
                break;
        }
    }
}

/**
 * Main function
 */
int main(void) {
    int main_choice;
    
    printf("=== Hospital Patient Record Management System ===\n");
    printf("Loading data from files...\n");
    fflush(stdout);
    
    if (!load_data_from_files()) {
        fprintf(stderr, "Warning: Some data may not have loaded correctly.\n");
    } else {
        printf("Data loading completed.\n");
    }
    fflush(stdout);
    
    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("1. Patient Features\n");
        printf("2. Admin Features\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &main_choice) != 1) {
            clear_input_buffer();
            main_choice = 3; /* Exit on invalid input */
        }
        clear_input_buffer();
        
        switch (main_choice) {
            case 1:
                handle_patient_menu();
                break;
                
            case 2:
                handle_admin_menu();
                break;
                
            case 3:
            default:
                printf("\nSaving data and exiting...\n");
                if (save_data_to_files()) {
                    printf("Data saved successfully.\n");
                } else {
                    fprintf(stderr, "Warning: Some data may not have saved correctly.\n");
                }
                cleanup_memory();
                printf("Thank you for using the system. Goodbye!\n");
                return 0;
        }
    }
}
