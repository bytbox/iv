package main

import (
	"fmt"
	"opts"
	"os"
)

// IVError represents an error originating in the IV editor
type IVError struct {
	message string
}

func (ce IVError) String() string {
	return fmt.Sprintf("iv: %s\n", ce.message)
}


var displayVersion = opts.Longflag("version",
	"print version information")


var view = NewCursesView()

func main() {
	// handle options
	opts.Parse()
	if *displayVersion {
		fmt.Printf("some text\n")
		os.Exit(0)
	}
	
	// start up the view
	view.Init()
	defer view.Shutdown()
}
