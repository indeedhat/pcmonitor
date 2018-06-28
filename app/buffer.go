package app

import (
	"io"
	"github.com/jacobsa/go-serial/serial"
	"./conf"
)

var buffer []byte
var arduino io.ReadWriteCloser
var err error

func open(config conf.AppConfig) error {
	options      := serial.OpenOptions{
		PortName: config.Port,
		BaudRate: config.BaudRate,
		DataBits: config.DataBits,
		StopBits: config.StopBits,
		MinimumReadSize: config.MinimumReadSize,
	}
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