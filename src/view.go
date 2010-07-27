package main

import (
	"curses"
	"os"
)

type View interface {
	Init() os.Error
	Shutdown() os.Error
}

type cView struct {

}

func NewCursesView() View {
	return &cView{}
}

func (v cView) Init() os.Error {
	// TODO FIXME proper error checking
	curses.Initscr(); /* start ncurses */
	curses.Start_color(); /* enable the use of color */
	curses.Cbreak(); /* don't wait for RETURN or ENTER to get input */
	curses.Noecho(); /* don't immediately echo characters */
	//curses.Nonl(); /* don't display newlines as newlines */
	//curses.Intrflush(stdscr,0);
	//curses.Keypad(stdscr,1); /* enable the keypad */
	//curses.Scrollok(stdscr,0); /* don't scroll off the bottom */
	return nil
}

func (v cView) Shutdown() os.Error {
	curses.Endwin()
	return nil
}

