#include "config_parser.h"
#include <stdio.h>
#include <cyaml/cyaml.h>

static const cyaml_schema_value_t int_schema = { CYAML_VALUE_INT(
	CYAML_FLAG_DEFAULT, int) };

static const cyaml_schema_field_t gpio_special_fields_schema[] = {
	CYAML_FIELD_SEQUENCE("pins", CYAML_FLAG_POINTER, GpioAttributes, pins,
			     &int_schema, 0, CYAML_UNLIMITED),
	CYAML_FIELD_END
};

static const cyaml_schema_field_t uart_special_fields_schema[] = {
	CYAML_FIELD_STRING_PTR("device_name", CYAML_FLAG_POINTER,
			       UartAttributes, device_name, 0, CYAML_UNLIMITED),
	CYAML_FIELD_INT("baud_rate", CYAML_FLAG_DEFAULT, UartAttributes,
			baud_rate),
	CYAML_FIELD_END
};

static const cyaml_schema_field_t config_fields_schema[] = {
	CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER, Config, motor_name,
			       0, CYAML_UNLIMITED),
	CYAML_FIELD_INT("steps_per_revolution", CYAML_FLAG_DEFAULT, Config,
			steps_per_revolution),
	CYAML_FIELD_INT("steps_rev_per_minute", CYAML_FLAG_DEFAULT, Config,
			steps_rev_per_minute),
	CYAML_FIELD_MAPPING("gpio", CYAML_FLAG_OPTIONAL | CYAML_FLAG_DEFAULT,
			    Config, gpio_attributes,
			    gpio_special_fields_schema),
	CYAML_FIELD_MAPPING("uart", CYAML_FLAG_OPTIONAL | CYAML_FLAG_DEFAULT,
			    Config, uart_attributes,
			    uart_special_fields_schema),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t config_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Config, config_fields_schema),
};

static const cyaml_schema_field_t configs_fields_schema[] = {
	CYAML_FIELD_SEQUENCE("motors", CYAML_FLAG_POINTER, ConfigWrapper,
			     configs, &config_schema, 0, CYAML_UNLIMITED),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t configs_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, ConfigWrapper,
			    configs_fields_schema),
};

static const cyaml_config_t config = {
	.log_fn = cyaml_log, /* Use the default logging function. */
	.mem_fn = cyaml_mem, /* Use the default memory allocator. */
	.log_level = CYAML_LOG_DEBUG, /* Logging errors and warnings only. */
};

cyaml_err_t config_parser_parse_config(char *filepath, ConfigWrapper **wrapper)
{
	return cyaml_load_file(filepath, &config, &configs_schema,
			       (void **)wrapper, NULL);
}
