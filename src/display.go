package display

import (
	"buffer"
)

// A Display represents a display of a buffer. Each buffer may have multiple
// displays, although they will all share the same content.
type Display struct {
	buffer     *buffer.Buffer // the buffer
	topLine    int            // the first visible line
	leftColumn int            // the leftmost visible column
	width      int            // the available width
	height     int            // the available height
	yPos       int            // the absolute Y-position of the cursor
	xPos       int            // the absolute X-position of the cursor
}

func (d *Display) Init(b *buffer.Buffer) {
	d.buffer = b
}

//
// Cursor movement
//

func (d *Display) CursorDown() {

}

func (d *Display) CursorUp() {

}

func (d *Display) CursorLeft() {

}

func (d *Display) CursorRight() {

}


//
// Page movement (separate from the cursor)
//

func (d *Display) PageDown() {

}

func (d *Display) PageUp() {

}

func (d *Display) PageLeft() {

}

func (d *Display) PageRight() {

}
