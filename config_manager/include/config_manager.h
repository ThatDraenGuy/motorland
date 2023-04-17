//
// Created by viacheslav on 11.04.23.
//

#ifndef MOTORLAND_FORK_CONFIG_MANAGER_H
#define MOTORLAND_FORK_CONFIG_MANAGER_H


int init_from_file(char* path, MotorsWrapper wrapper);

int parse_config(FILE **f, Config *config);

int init_motors(Config *config, MotorsWrapper wrapper);


#endif //MOTORLAND_FORK_CONFIG_MANAGER_H
