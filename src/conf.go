package conf

type Configuration struct {

}

// ReadConfig locates and reads the configuration from all relevant 
// directories
func ReadConfig() *Configuration {
	config = new(Configuration)
	return config
}
