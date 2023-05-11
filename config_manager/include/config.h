#pragma once

typedef struct{
    char* motor_name;
    int motor_type;
    int steps_per_revolution;
    int steps_per_minute;
    special_motor_attributes attributes;
} Config;

typedef struct{
    Config* config;
    size_t motors_count;
}ConfigWrapper;

typedef union{
    gpio_attributes gpio;
    spi_attributes spi;
    i2c_attributes i2c;
}special_motor_attributes;

typedef struct{
    int* pins;
}gpio_attributes;

typedef struct{
    int bus;
    int device;
    int mode;
    int max_speed_hz;
}spi_attributes;

typedef struct{
    int bus;
    int address;
}i2c_attributes;

/*typedef struct {
  char * motor_name;
  MotorConnection connection;
} Config;*/