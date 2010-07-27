package main

import (
	"curses"
	"os"
)

// A View represents the presentation of the application
type View interface {
	// General functions
	Init() os.Error
	Shutdown() os.Error
	// Hide() temporarily hides the view, enabling stdio to be used. In 
	// views that do not operate within a terminal, this should bring up a
	// terminal instead.
	Hide() os.Error
	// Refresh the display
	Refresh() os.Error

	// Displayer opening functions
	OpenFile(filename string) os.Error
	OpenBuffer(buffer *Buffer) os.Error
}

type cView struct{}

func NewCursesView() View {
	return &cView{}
}

func (v cView) Init() os.Error {
	curses.Initscr() /* start ncurses */
	if curses.Stdwin == nil {
		v.Shutdown()
		return &IVError{"Could not initialize curses"}
	}
	curses.Start_color() /* enable the use of color */
	curses.Cbreak()      /* don't wait for RETURN or ENTER to get input */
	curses.Noecho()      /* don't immediately echo characters */
	return nil
}

func (v cView) Shutdown() os.Error {
	curses.Endwin()
	return nil
}

func (v cView) Hide() os.Error {
	return NotImplementedError()
}

func (v cView) Refresh() os.Error {
	return NotImplementedError()
}

func (v cView) OpenFile(filename string) os.Error {
	return NotImplementedError()
}

func (v cView) OpenBuffer(buffer *Buffer) os.Error {
	return NotImplementedError()
}
