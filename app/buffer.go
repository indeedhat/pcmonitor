package app

import (
	"io"
	"github.com/jacobsa/go-serial/serial"
)

var buffer []byte
var arduino io.ReadWriteCloser
var err error

func open() error {
	options      := serial.OpenOptions{}
	arduino, err = serial.Open(options)

	return err
}

func writeSection(line []byte) {
	line   = append(line, byte(";"))
	buffer = append(buffer, line...)
}

func flush() error {
	buffer = append(buffer, byte(";"))
	_, err := arduino.Write(buffer)
	buffer = []byte{}

	return err
}