use std::rc::Rc;
use std::cell::RefCell;

// узел двусвязного списка. используем Rc<RefCell<...>>
// чтобы можно было ходить в обе стороны
type Link = Option<Rc<RefCell<DNode>>>;

struct DNode {
    data: String,
    prev: Link,
    next: Link,
}

pub struct DoublyList {
    pub head: Link,
    pub tail: Link,
    pub size: usize,
}

impl DoublyList {
    // пустой список
    pub fn new() -> Self {
        DoublyList { head: None, tail: None, size: 0 }
    }

    // вставка в голову
    pub fn push_front(&mut self, value: String) {
        let new_node = Rc::new(RefCell::new(DNode {
            data: value,
            prev: None,
            next: self.head.clone(),
        }));
        if let Some(old_head) = &self.head {
            old_head.borrow_mut().prev = Some(new_node.clone());
        } else {
            self.tail = Some(new_node.clone());
        }
        self.head = Some(new_node);
        self.size += 1;
    }

    // вставка в хвост
    pub fn push_back(&mut self, value: String) {
        let new_node = Rc::new(RefCell::new(DNode {
            data: value,
            prev: self.tail.clone(),
            next: None,
        }));
        if let Some(old_tail) = &self.tail {
            old_tail.borrow_mut().next = Some(new_node.clone());
        } else {
            self.head = Some(new_node.clone());
        }
        self.tail = Some(new_node);
        self.size += 1;
    }

    // найти узел по значению
    fn find_node(&self, value: &str) -> Link {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == value {
                return Some(node);
            }
            let next = node.borrow().next.clone();
            cur = next;
        }
        None
    }

    // вставка после узла с данным значением
    pub fn push_after(&mut self, anchor: &str, value: String) -> bool {
        if let Some(node) = self.find_node(anchor) {
            let next = node.borrow().next.clone();
            let new_node = Rc::new(RefCell::new(DNode {
                data: value,
                prev: Some(node.clone()),
                next: next.clone(),
            }));
            if let Some(n) = &next {
                n.borrow_mut().prev = Some(new_node.clone());
            } else {
                self.tail = Some(new_node.clone());
            }
            node.borrow_mut().next = Some(new_node);
            self.size += 1;
            true
        } else {
            false
        }
    }

    // вставка перед узлом с данным значением
    pub fn push_before(&mut self, anchor: &str, value: String) -> bool {
        if let Some(node) = self.find_node(anchor) {
            let prev = node.borrow().prev.clone();
            let new_node = Rc::new(RefCell::new(DNode {
                data: value,
                prev: prev.clone(),
                next: Some(node.clone()),
            }));
            if let Some(p) = &prev {
                p.borrow_mut().next = Some(new_node.clone());
            } else {
                self.head = Some(new_node.clone());
            }
            node.borrow_mut().prev = Some(new_node);
            self.size += 1;
            true
        } else {
            false
        }
    }

    // удалить голову
    pub fn del_front(&mut self) {
        if self.head.is_none() { println!("no"); return; }
        let new_head = self.head.as_ref().unwrap().borrow().next.clone();
        if let Some(h) = &new_head {
            h.borrow_mut().prev = None;
        } else {
            self.tail = None;
        }
        self.head = new_head;
        self.size -= 1;
    }

    // удалить хвост
    pub fn del_back(&mut self) {
        if self.tail.is_none() { println!("no"); return; }
        let new_tail = self.tail.as_ref().unwrap().borrow().prev.clone();
        if let Some(t) = &new_tail {
            t.borrow_mut().next = None;
        } else {
            self.head = None;
        }
        self.tail = new_tail;
        self.size -= 1;
    }

    // удалить узел после anchor
    pub fn del_after(&mut self, anchor: &str) -> bool {
        if let Some(node) = self.find_node(anchor) {
            let next = node.borrow().next.clone();
            if next.is_none() { return false; }
            let nextnext = next.as_ref().unwrap().borrow().next.clone();
            node.borrow_mut().next = nextnext.clone();
            if let Some(nn) = &nextnext {
                nn.borrow_mut().prev = Some(node.clone());
            } else {
                self.tail = Some(node.clone());
            }
            self.size -= 1;
            true
        } else {
            false
        }
    }

    // удалить узел перед anchor
    pub fn del_before(&mut self, anchor: &str) -> bool {
        if let Some(node) = self.find_node(anchor) {
            let prev = node.borrow().prev.clone();
            if prev.is_none() { return false; }
            let prevprev = prev.as_ref().unwrap().borrow().prev.clone();
            node.borrow_mut().prev = prevprev.clone();
            if let Some(pp) = &prevprev {
                pp.borrow_mut().next = Some(node.clone());
            } else {
                self.head = Some(node.clone());
            }
            self.size -= 1;
            true
        } else {
            false
        }
    }

    // удалить по значению
    pub fn del_by_value(&mut self, value: &str) {
        if let Some(node) = self.find_node(value) {
            // определяем крайние случаи
            let is_head = node.borrow().prev.is_none();
            let is_tail = node.borrow().next.is_none();
            if is_head && is_tail {
                self.head = None;
                self.tail = None;
            } else if is_head {
                let next = node.borrow().next.clone();
                if let Some(n) = &next { n.borrow_mut().prev = None; }
                self.head = next;
            } else if is_tail {
                let prev = node.borrow().prev.clone();
                if let Some(p) = &prev { p.borrow_mut().next = None; }
                self.tail = prev;
            } else {
                let prev = node.borrow().prev.clone();
                let next = node.borrow().next.clone();
                if let Some(p) = &prev { p.borrow_mut().next = next.clone(); }
                if let Some(n) = &next { n.borrow_mut().prev = prev.clone(); }
            }
            self.size -= 1;
        } else {
            println!("not found");
        }
    }

    // найти по значению
    pub fn find(&self, value: &str) -> bool {
        self.find_node(value).is_some()
    }

    // вывести голову
    pub fn get_head(&self) {
        match &self.head {
            Some(h) => println!("Head: {}", h.borrow().data),
            None => println!("list is empty"),
        }
    }

    // вывести хвост
    pub fn get_tail(&self) {
        match &self.tail {
            Some(t) => println!("Tail: {}", t.borrow().data),
            None => println!("list is empty"),
        }
    }

    // вывести с конца
    pub fn get_reverse(&self) {
        let mut cur = self.tail.clone();
        while let Some(node) = cur {
            println!("{}", node.borrow().data);
            let prev = node.borrow().prev.clone();
            cur = prev;
        }
    }

    // печать
    pub fn print(&self) {
        println!("DoublyList:");
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            println!("{}", node.borrow().data);
            let next = node.borrow().next.clone();
            cur = next;
        }
    }

    // сериализация "a,b,c,"
    pub fn join_csv(&self) -> String {
        let mut s = String::new();
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            s.push_str(&node.borrow().data);
            s.push(',');
            let next = node.borrow().next.clone();
            cur = next;
        }
        s
    }

    // восстановление из "a,b,c,"
    pub fn from_csv(csv: &str) -> Self {
        let mut list = DoublyList::new();
        for item in csv.split(',') {
            if !item.is_empty() {
                list.push_back(item.to_string());
            }
        }
        list
    }
}