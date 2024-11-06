idf_component_register(SRCS "drew-testing.c"
                    INCLUDE_DIRS "."
		REQUIRES esp_adc
	PRIV_REQUIRES esp_driver_gpio)
