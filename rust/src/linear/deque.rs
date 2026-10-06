use std::collections::VecDeque;

pub struct Deque {
    pub data: VecDeque<String>,
}

impl Deque {
    // пустая deque
    pub fn new() -> Self {
        Deque { data: VecDeque::new() }
    }

    // положить в начало
    pub fn push_front(&mut self, value: String) {
        self.data.push_front(value);
    }

    // положить в конец
    pub fn push_back(&mut self, value: String) {
        self.data.push_back(value);
    }

    // достать из начала
    pub fn pop_front(&mut self) -> String {
        match self.data.pop_front() {
            Some(v) => v,
            None => { println!("no"); String::new() }
        }
    }

    // достать из конца
    pub fn pop_back(&mut self) -> String {
        match self.data.pop_back() {
            Some(v) => v,
            None => { println!("no"); String::new() }
        }
    }

    // прочитать голову
    pub fn get_head(&self) {
        match self.data.front() {
            Some(v) => println!("Head: {}", v),
            None => println!("deque is empty"),
        }
    }

    // прочитать хвост
    pub fn get_tail(&self) {
        match self.data.back() {
            Some(v) => println!("Tail: {}", v),
            None => println!("deque is empty"),
        }
    }

    // печать
    pub fn print(&self) {
        println!("Deque:");
        for v in &self.data {
            println!("{}", v);
        }
    }

    // сериализация
    pub fn join_csv(&self) -> String {
        let mut s = String::new();
        for v in &self.data {
            s.push_str(v);
            s.push(',');
        }
        s
    }

    // из csv
    pub fn from_csv(csv: &str) -> Self {
        let mut d = Deque::new();
        for item in csv.split(',') {
            if !item.is_empty() {
                d.data.push_back(item.to_string());
            }
        }
        d
    }
}