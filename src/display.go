package display

import (
	"buffer"
)

// A Display represents a display of a buffer. Each buffer may have multiple
// displays, although they will all share the same content.
type Display struct {
	Buffer *buffer.Buffer
}

func (d Display) Init(b *buffer.Buffer) {
	d.Buffer = b
}
