//
// Created by viacheslav on 11.04.23.
//

#include "config_parser.h"

void parse_config(FILE **f, MotorsWrapper* wrapper) {
    for(int i = 0; i < wrapper->motor_count; ++i){
        char* motor_name;
        if(fread(motor_name, sizeof(char), MAX_NAME_LENGTH) != MAX_NAME_LEGTH){
            return;
        }
    }
}