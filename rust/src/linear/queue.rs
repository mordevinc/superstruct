use std::collections::VecDeque;

pub struct Queue {
    pub data: VecDeque<String>,
}

impl Queue {
    // пустая очередь
    pub fn new() -> Self {
        Queue { data: VecDeque::new() }
    }

    // положить в конец
    pub fn push(&mut self, value: String) {
        self.data.push_back(value);
    }

    // достать из начала
    pub fn pop(&mut self) -> String {
        match self.data.pop_front() {
            Some(v) => v,
            None => {
                println!("no");
                String::new()
            }
        }
    }

    // печать: от головы к хвосту
    pub fn print(&self) {
        println!("Queue (head -> tail):");
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
        let mut q = Queue::new();
        for item in csv.split(',') {
            if !item.is_empty() {
                q.data.push_back(item.to_string());
            }
        }
        q
    }
}