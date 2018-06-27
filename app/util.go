package app

import "strconv"

func ftob(char float64) byte {
	i, _ := strconv.Atoi(string(int(char)))
	return byte(i)
}