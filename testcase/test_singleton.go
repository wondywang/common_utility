//http://marcio.io/2015/07/singleton-pattern-in-go/
//https://gobyexample.com/
package main

import (
	"log"
	"sync"
)

type Singleton struct {
	var1 int
	var2 string
}

var once sync.Once
var _instance *Singleton

func Instance() *Singleton {
	once.Do(func() {
		log.Println("new singleton")
		_instance = &Singleton{}
		_instance.init()
	})
	return _instance
}

func (self *Singleton) init() {
	log.Println("init once!")
	self.var1 = 0
	self.var2 = "singletion instance!"
}

func (self *Singleton) Process() {
	self.var1 += 1
	log.Println("todo something!")
}

func run(ch chan int) {
	singleton := Instance()
	singleton.Process()
	log.Println(&singleton, " running...var1=", singleton.var1)
	ch <- 1
}

func main() {
	log.SetFlags(log.Lshortfile | log.LstdFlags)
	chs := make([]chan int, 10)
	for i, _ := range chs {
		chs[i] = make(chan int)
		log.Println("index: ", i)
		go run(chs[i])
	}

	for i, _ := range chs {
		<-chs[i]
	}
}
