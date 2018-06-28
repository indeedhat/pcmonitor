package app

import (
	"github.com/shirou/gopsutil/cpu"
	"github.com/shirou/gopsutil/mem"
	"time"
	"./conf"
	"log"
)

func processor() {
	var section []byte
	whole, err  := cpu.Percent(time.Millisecond * 500, false)
	cores, err1 := cpu.Percent(time.Millisecond * 500, true)

	if nil == err && nil == err1 {
		whole = append(whole, cores...)

		for i := 0; i < len(whole); i++ {
			section = append(section, ftob(whole[i]))
		}

		writeSection(section)
	}
}

func memory() error {
	vmem, err := mem.VirtualMemory()

	if nil != err {
		return err
	}

	writeSection([]byte{
		ftob(vmem.UsedPercent),
	})

	return nil
}

func main() {
	config, err := conf.Load("./conf.yaml")
	if nil != err {
		log.Fatal("Failed to load config\nExiting...")
	}

	open(config)

	for {
		func() {
			defer flush()

			processor()
			memory()

			time.Sleep(10)
		}()
	}
}