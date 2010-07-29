package conf

import (
	"container/vector"
	"io/ioutil"
	"os"
	"path"
	"scanner"
	"strings"
)

// ReadConfig() locates and reads the configuration from all relevant 
// directories.
func ReadConfig() (*Configuration, os.Error) {
	config := &Configuration{}
	// read configuration from default directory
	config.ReadConfigFrom("/etc/iv.d")
	// read configuration from user directory
	userdir := os.Getenv("HOME")
	if len(userdir) > 0 {
		config.ReadConfigFrom(path.Join(userdir,".iv.d"))
	}
	// read configuration from environment directory, if available
	envdir := os.Getenv("IV_CONFDIR")
	if len(envdir) > 0 {
		config.ReadConfigFrom(envdir)
	}
	return config, nil
}

// A Configuration object contains the entire available configuration.
type Configuration struct {
	// The places from which data was read.
	Sources vector.StringVector
	// The base keymap, overriden in part by mode- and property- specific
	// keymaps.
	BaseKeymap Keymap
}

// A ModeConfig encapsulates the configuration for a mode.
type ModeConfig struct {}

// A PropertyConfig describes the configuration to be applied if a certain 
// property is on.
type PropertyConfig struct {}

type Keymap map[string]string

// ReadConfigFrom() reads the configuration from the specified directory.
func (config *Configuration) ReadConfigFrom(dirname string) os.Error {
	// add this directory to the sources list
	config.Sources.Push(dirname)
	// read the keymap
	config.BaseKeymap = BlankKeymap()
	config.BaseKeymap.ReadFrom(path.Join(dirname,"keymap"))
	return nil
}

func BlankKeymap() Keymap {
	return make(map[string]string)
}

// ReadFrom() reads the keymap from the specified file
func (keymap Keymap) ReadFrom(filename string) os.Error {
	kmstr, err := ioutil.ReadFile(filename)
	if err != nil {
		return err
	}
	keymap.Read(string(kmstr))
	return nil
}

// Read() reads the keymap from the passed-in string
func (keymap Keymap) Read(kmstr string) {
	// Create and start up the scanner
	kmreader := strings.NewReader(kmstr)
	s := &scanner.Scanner{}
	s.Init(kmreader)
	// loop through until end of string
	tok := s.Scan()
	for tok != scanner.EOF {
		tok = s.Scan()
	}
}
