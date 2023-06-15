#include "config_parser.h"
#include <stdio.h>
#include <cyaml/cyaml.h>


static const cyaml_schema_value_t int_schema = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int)
};

static const cyaml_schema_field_t gpio_special_fields_schema[] = {
        CYAML_FIELD_SEQUENCE("pins", CYAML_FLAG_POINTER, GpioAttributes, pins, &int_schema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t uart_special_fields_schema[] = {
        CYAML_FIELD_STRING_PTR("device_name", CYAML_FLAG_POINTER, UartAttributes, device_name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT("baud_rate", CYAML_FLAG_DEFAULT, UartAttributes, baud_rate),
        CYAML_FIELD_END
};


static const cyaml_schema_field_t config_fields_schema[] = {
        CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER, Config, motor_name, 0, CYAML_UNLIMITED),
        //CYAML_FIELD_STRING_PTR("type", CYAML_FLAG_POINTER, Config, motor_type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT("steps_per_revolution", CYAML_FLAG_DEFAULT, Config, steps_per_revolution),
        CYAML_FIELD_INT("steps_rev_per_minute", CYAML_FLAG_DEFAULT, Config, steps_rev_per_minute),
        CYAML_FIELD_MAPPING("gpio", CYAML_FLAG_OPTIONAL | CYAML_FLAG_DEFAULT, Config, gpio_attributes,
                            gpio_special_fields_schema),
        CYAML_FIELD_MAPPING("uart", CYAML_FLAG_OPTIONAL | CYAML_FLAG_DEFAULT, Config, uart_attributes,
                            uart_special_fields_schema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t config_schema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Config, config_fields_schema),
};

static const cyaml_schema_field_t configs_fields_schema[] = {
        CYAML_FIELD_SEQUENCE("motors", CYAML_FLAG_POINTER, ConfigWrapper, configs, &config_schema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t configs_schema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, ConfigWrapper, configs_fields_schema),
};


static const cyaml_config_t config = {
        .log_fn = cyaml_log,            /* Use the default logging function. */
        .mem_fn = cyaml_mem,            /* Use the default memory allocator. */
        .log_level = CYAML_LOG_DEBUG, /* Logging errors and warnings only. */
};

int config_parser_parse_config(char *filepath, ConfigWrapper **wrapper) {
    return cyaml_load_file(filepath, &config, &configs_schema, (void **) wrapper, NULL);
}

void config_parser_print_config(int err, ConfigWrapper *wrapper) {
    printf("Error: %d\n", err);
    printf("Motor count: %zu\n", wrapper->configs_count);
    for (size_t i = 0; i < wrapper->configs_count; ++i) {
	    Config conf = wrapper->configs[i];
	    printf("Motor %zu: %s\n", i + 1, conf.motor_name);
	    printf("steps_rev_per_minute: %d\n", conf.steps_rev_per_minute);
	    printf("steps_rev_per_minute: %d\n", conf.steps_rev_per_minute);
	    if (conf.gpio_attributes.pins_count) {
		    printf("pins: ");
		    for (size_t j = 0; j < conf.gpio_attributes.pins_count; ++j) {
			    printf("%d ", conf.gpio_attributes.pins[j]);
		    }
		    printf("\n");
	    }
	    if(conf.uart_attributes.device_name){
		    printf("device name: %s\n", conf.uart_attributes.device_name);
		    printf("baud rate: %d\n", conf.uart_attributes.baud_rate);
	    }
	    /*if (conf.gpio_attributes) {
            printf("pins: ");
            for (size_t j = 0; j < conf.gpio_attributes->pins_count; ++j) {
                printf("%d ", conf.gpio_attributes->pins[j]);
            }
            printf("\n");
        }
        if(conf.uart_attributes){
            printf("device name: %s\n", conf.uart_attributes->device_name);
            printf("baud rate: %d\n", conf.uart_attributes->baud_rate);
        }*/
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
    	printf("Error: File path argument is required.\n");
    	return 1;
    }

    char *filepath = argv[1];
    ConfigWrapper *wrapper;
    int err = cyaml_load_file(filepath, &config, &configs_schema, (cyaml_data_t **) &wrapper, NULL);

    config_parser_print_config(err, wrapper);

    //cyaml_free(&config, &configs_schema, &wrapper, 0);
}
