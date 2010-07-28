package cursesview

import (
	"curses"
	"buffer"
	"errors"
	"os"
)

type cView struct{}

func NewCursesView() *cView {
	return &cView{}
}

func (v cView) Init() os.Error {
	curses.Initscr() /* start ncurses */
	if curses.Stdwin == nil {
		v.Shutdown()
		return &errors.IVError{"Could not initialize curses"}
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

// Start() starts the control loop for the curses view.
func (v cView) Start() os.Error {
	// for now, don't make it a seperate goroutine
	return v.control()
}

// The control loop for the curses view
func (v cView) control() os.Error {
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
