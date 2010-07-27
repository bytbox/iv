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

// Return codes
const (
	RET_NOERR   = 0
	RET_UNKNOWN = 127
)

var displayVersion = opts.Longflag("version",
	"print version information")


var view = NewCursesView()

func main() {
	// handle options
	opts.Parse()
	if *displayVersion {
		fmt.Printf("some text\n")
		os.Exit(RET_NOERR)
	}

	// start up the view
	view.Init()
	defer view.Shutdown()

	// open up files
	for i := 0; i < len(opts.Args); i++ {
		fmt.Printf("Opening '%s'\n", opts.Args[i])
	}
}
