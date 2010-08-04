package main

import (
	"buffer"
	"conf"
	"cursesview"
	"fmt"
	"opts"
	"os"
	"view"
)

// Return codes
const (
	RET_NOERR   = 0
	RET_UNKNOWN = 127
)

// the version of iv this is
const VERSION = "0.1.0"

var displayVersion = opts.Longflag("version",
	"print version information")


var cView view.View = cursesview.NewCursesView()

func cleanExit(code int) {
	cView.Shutdown()
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

	// read the configuration
	config, _ := conf.ReadConfig()

	cursesview.Initstring = fmt.Sprintf(
		"iv v%s - Copyright (C) Scott Lawrence 2010", VERSION)

	// start up the view
	cView.Init(config)

	// open up files
	for i := 0; i < len(opts.Args); i++ {
		printErr(cView.OpenFile(opts.Args[i]))
	}
	if len(opts.Args) == 0 {
		// open the default buffer
		buffer, err := buffer.NewBufferDefault()
		printErr(err)
		printErr(cView.OpenBuffer(buffer))
	}

	// run the view
	cView.Start()

	cleanExit(RET_NOERR)
}
