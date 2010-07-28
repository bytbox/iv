package main

import (
	"buffer"
	"fmt"
	"opts"
	"os"
	"cursesview"
)

// Return codes
const (
	RET_NOERR   = 0
	RET_UNKNOWN = 127
)

// the version of iv this is
const VERSION = "0.0.1"

var displayVersion = opts.Longflag("version",
	"print version information")


var view View = cursesview.NewCursesView()

func cleanExit(code int) {
	view.Shutdown()
	os.Exit(code)
}

func printErr(err os.Error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s\n", err.String())
		cleanExit(RET_UNKNOWN)
	}
}

func main() {
	// handle options
	opts.Usage("[options] [FILENAME [...]]")
	opts.Description("the editor that isn't vi")
	opts.Parse()
	if *displayVersion {
		fmt.Printf("iv v%s\n", VERSION)
		cleanExit(RET_NOERR)
	}

	// start up the view
	view.Init()

	// open up files
	for i := 0; i < len(opts.Args); i++ {
		printErr(view.OpenFile(opts.Args[i]))
	}
	if len(opts.Args) == 0 {
		// open the default buffer
		buffer, err := buffer.NewBufferDefault()
		printErr(err)
		printErr(view.OpenBuffer(buffer))
	}
	cleanExit(RET_NOERR)
}