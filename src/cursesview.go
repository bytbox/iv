package cursesview

import (
	"curses"
	"buffer"	
	"display"
	"errors"
	"os"
)

// The split modes
const (
	NOSPLIT = iota
	HSPLIT
	VSPLIT
)

type cView struct {
	win *curses.Window
	
	splitMode int
	mainDisplay *display.Displayer
	auxDisplay *display.Displayer
}

func NewCursesView() *cView {
	return &cView{}
}

func (v cView) Init() os.Error {
	v.splitMode=NOSPLIT
	curses.Initscr() /* start ncurses */
	if curses.Stdwin == nil {
		v.Shutdown()
		return &errors.IVError{"Could not initialize curses"}
	}
	v.win=curses.Stdwin
	curses.Start_color() /* enable the use of color */
	curses.Cbreak()      /* don't wait for RETURN or ENTER to get input */
	curses.Noecho()      /* don't immediately echo characters */
	return nil
}

func (v cView) Shutdown() os.Error {
	curses.Endwin()
	return nil
}

// Start() starts the control loop for the curses view.
func (v cView) Start() os.Error {
	// for now, don't make it a seperate goroutine
	return v.control()
}

// The control loop for the curses view
func (v cView) control() os.Error {
	for true {
		v.win.Addch(0,0,int32(v.win.Getch()),0)
	}
	return nil
}

func (v cView) Hide() os.Error {
	return errors.NotImplementedError()
}

func (v cView) Refresh() os.Error {
	return errors.NotImplementedError()
}

func (v cView) OpenFile(filename string) os.Error {
	return errors.NotImplementedError()
}

func (v cView) OpenBuffer(buffer *buffer.Buffer) os.Error {
	return errors.NotImplementedError()
}
