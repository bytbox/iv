package buffer

import (
	"container/vector"
	"io/ioutil"
	"os"
	"strings"
)

// A Buffer stores a buffer of text data, along with highlighting information.
type Buffer struct {
	Filename string
	Lines    vector.StringVector
}

// NewBufferBlank creates a blank buffer
func NewBufferBlank() (*Buffer, os.Error) {
	return &Buffer{}, nil
}

// NewBufferFromFile creates a buffer from a file
func NewBufferFromFile(filename string) (*Buffer, os.Error) {
	contents, err := ioutil.ReadFile(filename)
	if err != nil {
		return nil, err
	}
	b, err := NewBufferBlank()
	if err != nil {
		return nil, err
	}
	b.Filename = filename
	lines := strings.Split(string(contents), "\n", -1)
	for _, line := range lines {
		b.Lines.Push(line)
	}
	return b, nil
}

// NewBufferDefault creates a default buffer
func NewBufferDefault() (*Buffer, os.Error) {
	b, err := NewBufferBlank()
	if err != nil {
		return nil, err
	}
	return b, nil
}
