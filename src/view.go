package main

import (
	"curses"
	"os"
)

type View interface {
	Init() os.Error
	Shutdown() os.Error
	Open(filename string) os.Error
}

type cView struct{}

func NewCursesView() View {
	return &cView{}
}

func (v cView) Init() os.Error {
	curses.Initscr()     /* start ncurses */
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

func (v cView) Open(filename string) os.Error {
	return nil
}
