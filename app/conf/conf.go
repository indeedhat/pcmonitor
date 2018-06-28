package conf

import (
  "gopkg.in/yaml.v2"
  "io/ioutil"
)

type AppConfig struct {
  Port            string
  BaudRate        uint   `yaml:"baud"`
  DataBits        uint   `yaml:"data-bits"`
  StopBits        uint   `yaml:"stop-bits"`
  MinimumReadSize uint   `yaml:"min-read"`
}

func Load(path string) (AppConfig, error) {
  txt, err := ioutil.ReadFile(path)
  if nil != err {
    return nil, err
  }

  conf := AppConfig{}

  err = yaml.Unmarshal(txt, &conf)

  if nil != err {
    return nil, err
  }

  return conf, nil
}