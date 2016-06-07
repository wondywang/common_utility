package main

import (
	"log"
)

type Observer interface {
	Update(int)
}

type Subscribe interface {
	Attach(Observer)
	Detach(Observer)
	Notify()
}

type Blog struct {
	obs []Observer
}

func (self *Blog) Notify() {
	if len(self.obs) <= 0 {
		log.Println("no observer update!")
		return
	}
	for idx, item := range self.obs {
		item.Update(idx)
	}
}

func (self *Blog) Attach(ob Observer) {
	log.Println("attach observer addr: ", &ob)
	self.obs = append(self.obs, ob)
}

func (self *Blog) Detach(ob Observer) {
	log.Println("detach observer addr: ", &ob)
	for idx, item := range self.obs {
		if item == ob {
			self.obs = append(self.obs[0:idx], self.obs[idx+1:]...)
			break
		}
	}
}

type Publish struct {
	msg chan int
}

func (self *Publish) Update(idx int) {
	self.msg <- idx
}

func (self *Publish) Listen() {
	log.Println("listen publish!")
	go func() {
		for {
			select {
			case msg := <-self.msg:
				log.Println("get msg: ", msg)
			default:
	//			log.Println("default case!")
			}
		}
	}()
}

func main() {
	blog := &Blog{}

	publisher1 := &Publish{
		msg: make(chan int),
	}
	publisher1.Listen()

	publisher2 := &Publish{
		msg: make(chan int),
	}
	publisher2.Listen()

	blog.Attach(publisher1)
	blog.Attach(publisher2)
	blog.Notify()

	blog.Detach(publisher1)
	blog.Notify()

	blog.Detach(publisher2)
	blog.Notify()
}
