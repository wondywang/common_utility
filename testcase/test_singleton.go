//http://marcio.io/2015/07/singleton-pattern-in-go/
//https://gobyexample.com/
package main

import (
	"fmt"
	//	"strings"
)

type Singleton struct {
	var1 int
	var2 string
}

var _instance *Singleton

func Instance(ch chan int) *Singleton {
	ch <- 1
	if _instance == nil {
		fmt.Println("new singleton")
		_instance = &Singleton{}
	}
	return _instance
}

func (self *Singleton) Instance(ch chan int) *Singleton {
	ch <- 1
	if _instance == nil {
		_instance = &Singleton{}
	}
	return _instance
}

func (self *Singleton) HelloWorld() {
	self.var1 = 100
	self.var2 = "hello hello"
	fmt.Println("func hello world!")
}

func run(ch chan int) {
	//      ch <- 1
	hello := Instance(ch)
	hello.HelloWorld()
	fmt.Println(hello, " running...")
}

func main() {
	chs := make([]chan int, 10)
	for i, _ := range chs {
		chs[i] = make(chan int)
		fmt.Println("index: ", i)
		go run(chs[i])
	}

	for _, ch := range chs {
		<-ch
	}
}
