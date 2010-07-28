package main

import (
	"buffer"
	"conf"
	"os"
)

// A View represents the presentation of the application
type View interface {
	// General functions
	Init(config *conf.Configuration) os.Error
	Shutdown() os.Error
	// Hide() temporarily hides the view, enabling stdio to be used. In
	// views that do not operate within a terminal, this should bring up a
	// terminal instead.
	Hide() os.Error
	// Refresh the display
	Refresh() os.Error

	// Passes control to the controller
	Start() os.Error

	// Displayer opening functions
	OpenFile(filename string) os.Error
	OpenBuffer(buffer *buffer.Buffer) os.Error
}
